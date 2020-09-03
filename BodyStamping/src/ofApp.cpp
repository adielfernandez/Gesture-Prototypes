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


	mCV.setup(depthW, depthH);




	// Call this after other classes have added all their params
	SharedGui::get()->finishSetup();


	bShowGui = false;

	//ofSetFullscreen(true);
	bIsFullscreen = false;


}

//--------------------------------------------------------------
void ofApp::update(){
	mKinect.update();

	mCV.update();

	if (mKinect.getDepthSource()->isFrameNew()) {
		mCV.process(mKinect.getDepthSource()->getPixels());
	}


}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackgroundGradient(ofColor(50), ofColor(0));

	ofPushMatrix();


	ofPopMatrix();



	if (bDrawKinect) {

		float left = 250;
		float top = 20;
		KinectUtils::drawDebugView(&mKinect, left, top);

		mCV.drawRawMapped(left, top + depthH + 60, depthW, depthH);
		mCV.drawBlurred(left + depthW, top + depthH + 60, depthW, depthH);
	}


	if (bShowGui) {

		ofSetColor(255);
		ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate(), 2), 10, 20);

		SharedGui::get()->draw();
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

	if (key == 's') SharedGui::get()->save();
	if (key == 'l') SharedGui::get()->load();
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
