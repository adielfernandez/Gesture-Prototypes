#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    ofEnableAlphaBlending();

	FontManager::get()->addFont("fonts/futura_light.ttf", "futura_light");
	FontManager::get()->addFont("fonts/futura.ttf", "futura");

	SharedGui::get()->setup();
	SharedGui::get()->setPosition(10, 80);
	SharedGui::get()->setColor(ofColor(0, 160, 255));

	SharedGui::get()->getGroup("UX")->add(mPlaneMovementScale.set("Plane Movement Scaling", 1000.0, 0, 2000.0));
	SharedGui::get()->getGroup("UX")->add(mCameraPanMult.set("Cam Pan Mult", 300.0, 0, 900.0));
	

	mKinect.open();
	mKinect.initDepthSource();
	mKinect.initColorSource();
	mKinect.initInfraredSource();
	mKinect.initBodySource();
	mKinect.initBodyIndexSource();

	mMeshController.setup();

	SharedGui::get()->finishSetup();

	mMeshController.createMesh();


	// Clipping plane
	mPlaneNormal = ofVec3f(0, 1, 0);
	mPlaneNormal.normalize();

	

	mPlanePos.set(0);


	// camera setup
	float angleAboveHoriz = 30;
	mGlobalCamPos = ofVec3f(0, 1100, 0);
	mGlobalCamPos.rotate(angleAboveHoriz, ofVec3f(1, 0, 0));
	mCamStartPos = mGlobalCamPos;

	cam.setGlobalPosition(mGlobalCamPos);
	cam.lookAt(mCamLookAt);

	bSetupCamera = false;

	mDinoImg.load("images/skull-small.png");

	ofVec3f hm = mMeshController.getMaxSize() / 2.0f;

	for (int i = 0; i < 10; i++) {
		
		float x = ofRandom(-hm.x, hm.x) * 0.7f; // scale down to make sure it's inside the box
		float y = ofRandom(-hm.y, hm.y) * 0.7f;
		float z = ofRandom(mMeshController.getMaxBottom(), -250);

		//cout << z << endl;

		mDinoPositions.push_back(ofVec3f(x, y, z));

	}

}

void ofApp::resetCamera() {
	mGlobalCamPos = mCamStartPos;
	cam.setGlobalPosition(mCamStartPos);
	cam.lookAt(mCamLookAt, ofVec3f(0, 0, 1));
}

//--------------------------------------------------------------
void ofApp::update(){

	//Cheap camera Hack: ofEasyCam resets itself after the 2nd frame to
	//whatever default distance it wants so we'll set it back after that bug
	if (ofGetFrameNum() == 2 && !bSetupCamera) {

		resetCamera();
		bSetupCamera = true;

	}


	mKinect.update();

	// Look for hands crossing the break plane
	bool bodyFound = false;
	ofVec3f head, leftHand, rightHand, hip;

	bool leftGrab = false;
	bool rightGrab = false;

	auto bodies = mKinect.getBodySource()->getBodies();
	for (auto body : bodies) {

		if (body.tracked) {
			bodyFound = true;
			for (auto joint : body.joints) {

				//cout << "First: " << joint.first << ", Second: " << joint.second.getPosition() << endl;
				if (joint.first == JointType_Head) {
					head = joint.second.getPositionInWorld();
				}
				else if (joint.first == JointType_HandLeft) {
					leftHand = joint.second.getPositionInWorld();
				}
				else if (joint.first == JointType_HandRight) {
					rightHand = joint.second.getPositionInWorld();
				}
				else if (joint.first == JointType_HipLeft) {
					hip = joint.second.getPositionInWorld();
				}
			}

			if (body.leftHandState == HandState::HandState_Closed) leftGrab = true;
			if (body.rightHandState == HandState::HandState_Closed) rightGrab = true;

			break;
		}
	}


	bool leftAboveHip = leftHand.y > hip.y;
	bool rightAboveHip = rightHand.y > hip.y;

	if (leftGrab && rightGrab && leftAboveHip && rightAboveHip) {

		// cancel one hand gesture
		bOneHandGestureActive = false;
		bFirstGrab = true;

		// get angle between hands from left to right, 
		// flatted into the XZ plane (in camera space), and transpose XY
		ofVec3f leftToRight = rightHand - leftHand;		
		leftToRight = ofVec3f(leftToRight.x, leftToRight.z, 0);

		leftToRight.normalize();
		//cout << "Left to Right: " << leftToRight << endl;

		ofVec3f currentHandCenter = (rightHand + leftHand) / 2.0f;

		if (!bTwoHandGestureActive) {
			// first frame, initiate gesture
			bTwoHandGestureActive = true;
			mInitialHandVec = leftToRight;
			mInitialHandCenter = currentHandCenter;

			// get the vector from the origin to the camera pos to set the plane normal
			ofVec3f norm = mGlobalCamPos;
			norm.z = 0; 
			mPlaneNormal = norm.normalized();

			
			// set plane at cam pos then just clamp it
			ofVec2f halfMesh = mMeshController.getMeshSizeXY() / 2.0f;
			mPlanePos = ofClampVec(mGlobalCamPos, -halfMesh, halfMesh);
			
			mInitialPlanePos = mPlanePos;

		
		}

		float handAngle = ofRadToDeg( getAngleBetween(leftToRight, mInitialHandVec) );

		//safety check
		if ( isnan(handAngle) ) handAngle = 0;

		//cout << "Initial Hand Vec: " << mInitialHandVec << ", Current Hand Vec: " << leftToRight << ", Angle: " << handAngle << endl;

		mPlaneRotNorm = mPlaneNormal.getRotated(handAngle, ofVec3f(0, 0, 1));

		// We only really care about the hands movement to and from the camera, i.e. Z axis
		ofVec3f rawDisp = currentHandCenter - mInitialHandCenter;
		float displacement = rawDisp.z * mPlaneMovementScale;

		ofVec2f dir = ofVec2f(mGlobalCamPos.x, mGlobalCamPos.y).normalize();
		mPlanePos = mInitialPlanePos + dir * displacement;

		ofVec2f halfMesh = mMeshController.getMeshSizeXY() / 2.0f;

		mPlanePos = ofClampVec( mPlanePos, -halfMesh, halfMesh);

		// set the position of the plane based on the difference 
		// of where the hands are now vs where they started

		mMeshController.setClippingPlane(mPlanePos, mPlaneRotNorm);
		mMeshController.setShouldCrossSection(true);


	} else {
		bTwoHandGestureActive = false;

		mMeshController.setShouldCrossSection(false);


		// SINGLE HAND GESTURE
		if ((leftGrab && leftAboveHip) || (rightGrab && rightAboveHip)) {

			ofVec3f handPos = bUseLeft ? leftHand : rightHand;
			bUseLeft = leftGrab ? true : false;

			if (bFirstGrab) {
				mFirstSingleGrabTime = ofGetElapsedTimef();
				bFirstGrab = false;
			}
			

			float timeSinceFirstGrab = ofGetElapsedTimef() - mFirstSingleGrabTime;
			if (!bOneHandGestureActive && timeSinceFirstGrab > 0.15f) {
				bOneHandGestureActive = true;
				mInitialOneHandPos = handPos;
				mInitialCamPos = mGlobalCamPos;
			}

			if (bOneHandGestureActive) {
				// invert Y and Z
				ofVec3f rawDisp = (handPos - mInitialOneHandPos);

				float displacement = -rawDisp.x;

				float angle = displacement * mCameraPanMult;

				mGlobalCamPos = mInitialCamPos.getRotated(angle, ofVec3f(0,0,1));

				cam.setGlobalPosition(mGlobalCamPos);
				cam.lookAt(mCamLookAt, ofVec3f(0, 0, 1));
			}


		} else {
			bOneHandGestureActive = false;
			bFirstGrab = true;
		}


	}


	


	mMeshController.update();

}

//--------------------------------------------------------------
void ofApp::draw(){


    ofEnableDepthTest();
	//ofEnableLighting();

    
    ofBackgroundGradient(ofColor(80), ofColor(0));
    
    ofSetColor(255);
    

    cam.begin();
	//light.enable();
	//material.begin();




	mMeshController.drawCrossSection();

	//if (bTwoHandGestureActive) {
	//	// draw plane rotation axis
	//	ofPushStyle();

	//	ofPushMatrix();

	//	float angle = ofMap(ofGetMouseX(), 0, ofGetWindowWidth(), -180, 180);
	//	ofRotate(angle, 0, 0, 1);
	//	ofRotate(90, 0, 1, 0);
	//
	//	float w = 500;
	//	float h = 1200;
	//	ofSetColor(255);
	//	ofNoFill();
	//	ofDrawRectangle(-w / 2, -h / 2, w, h);

	//	ofPopMatrix();
	//
	//
	//	ofSetLineWidth(3);
	//	ofSetColor(255, 0, 0);
	//	ofDrawLine(mPlanePos.x, mPlanePos.y, w/2, mPlanePos.x, mPlanePos.y, -w/2);

	//	ofPopStyle();
	//}



	ofSetColor(255);
	for (auto & pos : mDinoPositions) {

		float angle = getBillboardAngle(pos);
		ofVec3f vec = getBillboardVec(pos);


		ofPushMatrix();
		ofTranslate(pos.x, pos.y, pos.z);
		ofRotate(angle, vec.x, vec.y, vec.z);
		//ofRotate(180, 0, 1, 0);
		mDinoImg.draw(0, 0);
		ofPopMatrix();
	}



	mMeshController.draw();



	//material.end();
	//light.disable();

	//ofDisableLighting();

	if (bShowGui) mAxes.drawAxes();

    cam.end();

    

	if (bShowGui) {
		ofDisableDepthTest();
		ofSetColor(255);
		ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate(), 2), 10, 20);

		SharedGui::get()->draw();

		float w = 400;
		float m = 20;
		ofSetColor(255);
		mMeshController.mGround.getFbo().draw(m, ofGetHeight() - w - m, w, w);
	}

	if (bDrawKinect) {

		float left = 250;
		float top = 20;
		KinectUtils::drawDebugView(&mKinect, left, top);
	}

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == 'w') mMeshController.toggleWireframe();
	if (key == 'k') bDrawKinect = !bDrawKinect;
	if (key == 's') SharedGui::get()->save();
	if (key == 'l') SharedGui::get()->load();
	if (key == 'g') bShowGui = !bShowGui;

	if (key == ' ') resetCamera();
    
}


//--------------------------------------------------------------
//Billboarding code adapted from Zach Lieberman's Sample code:
//https://forum.openframeworks.cc/t/billboard-using-ofnode/14142

float ofApp::getBillboardAngle(ofVec3f globalPos) {

	ofNode posNode;
	posNode.setGlobalPosition(globalPos);
	posNode.lookAt(cam.getGlobalPosition(), cam.getUpDir());
	ofQuaternion posQuat = posNode.getGlobalOrientation();

	float ang = 0;
	ofPoint vec;
	posQuat.getRotate(ang, vec);

	return ang;

}

//--------------------------------------------------------------
ofVec3f ofApp::getBillboardVec(ofVec3f globalPos) {

	ofNode posNode;
	posNode.setGlobalPosition(globalPos);
	posNode.lookAt(cam.getGlobalPosition(), cam.getUpDir());
	ofQuaternion posQuat = posNode.getGlobalOrientation();

	float ang = 0;
	ofPoint vec;
	posQuat.getRotate(ang, vec);

	return vec;

}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
