#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(60);
	ofSetVerticalSync(true);



	mKinect.open();
	mKinect.initDepthSource();
	mKinect.initColorSource();
	mKinect.initInfraredSource();
	mKinect.initBodySource();
	mKinect.initBodyIndexSource();


	
	mBackground.load("images/backgrounds/alps16x9.jpg");


	mGui = make_shared<Gui>();
	mGui->setup();


	bShowGui = false;

	//ofSetFullscreen(true);
	bIsFullscreen = false;

	mBinoculars.setup();
	mBinoculars.setTexture(mBackground.getTexture());
	mBinoculars.setGui(mGui);

}

//--------------------------------------------------------------
void ofApp::update(){
	mKinect.update();

	ofVec2f binPos(ofGetWindowWidth() / 2.0f, ofGetWindowHeight() * 0.57);

	bool bodyFound = false;
	ofVec3f head, leftHand, rightHand, leftElbow, rightElbow;

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
				else if (joint.first == JointType_ElbowLeft) {
					leftElbow = joint.second.getPositionInWorld();
				}
				else if (joint.first == JointType_ElbowRight) {
					rightElbow = joint.second.getPositionInWorld();
				}
			}
			break;
		}
	}


	bool showBin = false;

	if (bodyFound) {

		float toLeftDistSq = ofDistSquared(head.x, head.y, leftHand.x, leftHand.y);
		float toRightDistSq = ofDistSquared(head.x, head.y, rightHand.x, rightHand.y);

		//cout << "Left Dist: " << toLeftDistSq << "\t\t" << "Right Dist: " << toRightDistSq << endl;

		float thresh = mGui->headHandDistThreshold;
		if (toLeftDistSq < thresh && toRightDistSq < thresh) {
			showBin = true;

			// get 3D vector normal to the plane created by two elbow points and avg of two hands
			ofVec3f avgHands = (leftHand + rightHand) / 2.0f;

			ofVec3f toLeftElbow = (leftElbow - leftHand).getNormalized();
			ofVec3f toRightElbow = (rightElbow - rightHand).getNormalized();


			// Cross product doesn't work for some reason
			//ofVec3f planeNormal = toLeftElbow.getCrossed(toRightElbow);
			


			// ZOOM
			float zoomAngle = getAngleBetween(toLeftElbow, toRightElbow);
			float mag = ofClamp(ofMap(zoomAngle, 1.7, 0.3, mGui->binMinMag, mGui->binMaxMag), mGui->binMinMag, mGui->binMaxMag);
			mBinoculars.setMagnification(mag);
			//cout << "To Left: " << toLeftElbow << "\t\t" << "To Right: " << toRightElbow << "\t\t" << "Angle: " << angle << endl;

			// ORIENTATION
			// get vector between elbows in XZ plane and find angle between -Z vector
			ofVec3f leftElbowToRight = (rightElbow - leftElbow).getNormalized();

			float orientAngle = getAngleBetween(leftElbowToRight, ofVec3f(0, 0, -1));
			cout << "Orientation Angle: " << orientAngle << endl;

			float leftAngle = mGui->binCenterBodyAngle - mGui->binBodyAngleDev;
			float rightAngle = mGui->binCenterBodyAngle + mGui->binBodyAngleDev;
			float xPos = ofMap(orientAngle, leftAngle, rightAngle, mGui->binLeftBinX, mGui->binRightBinX) * ofGetWidth();
			
			binPos.x = xPos;

		}

	}
	else {

	}

	//mBinoculars.setShowing(ofGetMousePressed());
	//mBinoculars.update(ofVec2f(ofGetMouseX(), ofGetMouseY()));



	mBinoculars.setShowing(showBin);
	mBinoculars.update(binPos);

}

float ofApp::getAngleBetween(ofVec3f a, ofVec3f b) {
	return acos(a.dot(b) / (a.length() * b.length()));
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(255);
	mBackground.draw(0, 0, 1920, 1080);

	mBinoculars.draw();

	if (bDrawKinect) {
		KinectUtils::drawColorAndBody(&mKinect);
	}


	if (bShowGui) {

		ofSetColor(255);
		ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate(), 2), 10, 20);

		mGui->draw(10, 50);
	}

}


void ofApp::setFakeFullscreen() {
	ofSetWindowPosition(-1, -1);
	ofSetWindowShape(ofGetScreenWidth() + 1, ofGetScreenHeight() + 1);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){


	if (key == 'f') {

		if (bIsFullscreen) {
			ofSetWindowPosition(100, 100);
			ofSetWindowShape(1920, 1080);
		} else {
			setFakeFullscreen();
		}

		bIsFullscreen != bIsFullscreen;

	}

	if (key == 'k') bDrawKinect = !bDrawKinect;

	if (key == 's') mGui->saveSettings();
	if (key == 'l') mGui->loadSettings();
	if (key == 'g') bShowGui = !bShowGui;

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
