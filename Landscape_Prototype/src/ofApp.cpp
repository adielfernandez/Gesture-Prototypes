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


	mBinoculars.setShowing(ofGetMousePressed());
	mBinoculars.update(ofVec2f(ofGetMouseX(), ofGetMouseY()));


	auto bodies = mKinect.getBodySource()->getBodies();


	bool bodyFound = false;

	for (int i = 0; i < bodies.size(); i++) {
		if (bodies[i].tracked) {
			bodyFound = true;

		}
		else {


		}
	}


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
