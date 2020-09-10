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


	FontManager::get()->addFont("fonts/futura_light.ttf", "futura_light");
	FontManager::get()->addFont("fonts/futura.ttf", "futura");

	SharedGui::get()->setup();
	SharedGui::get()->setPosition(10, 80);
	SharedGui::get()->setColor(ofColor(0, 160, 255));

	SharedGui::get()->getGroup("UX")->add(mBreakPlaneDist.set("Break Plane Dist", 1.5, 0, 4));


	mCV.setup(depthW, depthH);

	mGestureIsolator.setup(depthW, depthH);
	mCanvas.setup(depthW, depthH);

	// Call this after other classes have added all their params
	SharedGui::get()->finishSetup();


	bShowGui = false;

	//ofSetFullscreen(true);
	bIsFullscreen = false;


	// create color palette
	int num = 10;
	for (int i = 0; i < num; i++) {

		float pct = i / (float)num;
		ofColor c;
		c.setHsb(ofRandom(pct * 255), 128, 255, 255);

		mColors.push_back(c);
	}

	std::random_shuffle(mColors.begin(), mColors.end());

}

//--------------------------------------------------------------
void ofApp::update(){
	mKinect.update();

	mCV.update();

	if (mKinect.getDepthSource()->isFrameNew()) {
		mCV.processDepth(mKinect.getDepthSource()->getPixels());
	}

	mGestureIsolator.isolate(mCV.getAccumulatedImg(), getCurrentColor());

	// Look for hands crossing the break plane
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


	if (bodyFound && (rightHand.z < mBreakPlaneDist || leftHand.z < mBreakPlaneDist || head.z < mBreakPlaneDist)) {
		mCV.setRecording(true);
		bUserPresent = true;
	} else {

		if (bUserPresent) {
			// first frame with no user, change the color, 
			// grab the current gesture pixels and draw them
			mCanvas.add(mGestureIsolator.getFbo().getTexture(), getCurrentColor());
			getNextColor();

			bUserPresent = false;
		}

		mCV.setRecording(false);

	}

	mCanvas.update();

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackgroundGradient(ofColor(40), ofColor(0));


	// ----- draw gesture and canvas side by side ----- 
	//ofVec2f size(depthW, depthH);
	//size *= 1.0;
	//ofVec2f pos = ofGetWindowSize() / 2.0f - ofVec2f(size.x, size.y / 2.0f);
	//ofSetColor(getCurrentColor());
	//mGestureIsolator.getFbo().draw(pos, size.x, size.y);
	//ofSetColor(255);
	//mCanvas.getFbo().draw(pos + ofVec2f(size.x, 0), size.x, size.y);
	
	// ----- draw gesture and canvas overlayed -----
	ofVec2f size(depthW, depthH);
	size *= 2.5;
	ofVec2f pos = ofGetWindowSize() / 2.0f - size / 2.0f;
	ofSetColor(255);
	mCanvas.getFbo().draw(pos, size.x, size.y);
	mGestureIsolator.getFbo().draw(pos, size.x, size.y);


	if (bDrawKinect) {

		float left = 250;
		float top = 20;
		KinectUtils::drawDebugView(&mKinect, left, top);

		float yGap = 60;
		mCV.drawRawMapped(left, top + depthH + yGap, depthW, depthH);
		mCV.drawThresholdedAndBlurred(left + depthW, top + depthH + yGap, depthW, depthH);
		mCV.drawAccumulated(left + depthW * 2, top + depthH + yGap, depthW, depthH);
	}



	if (mCV.isRecording()) {
		//ofSetColor(255, 0, 0);
		//ofRect(ofGetWidth() - 50, 0, 50, 50);
	}

}


void ofApp::setFakeFullscreen() {
	ofSetWindowPosition(-1, -1);
	ofSetWindowShape(ofGetScreenWidth() + 1, ofGetScreenHeight() + 1);
}

ofColor ofApp::getNextColor() {
	colorIdx++;
	if (colorIdx >= mColors.size()) colorIdx = 0;
	return getCurrentColor();
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

	if (key == 's') SharedGui::get()->save();
	if (key == 'l') SharedGui::get()->load();
	if (key == 'g') bShowGui = !bShowGui;

	if (key == ' ') mCanvas.clear();

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
