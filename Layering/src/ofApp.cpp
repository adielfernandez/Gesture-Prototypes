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

	//SharedGui::get()->getGroup("UX")->add(mBreakPlaneDist.set("Break Plane Dist", 1.5, 0, 4));


    

	//set up the different properties of the lighting
	light.setPosition(400, 0, 400);
	light.setup();

	light.setDiffuseColor(ofFloatColor::white);
	light.setAmbientColor(ofFloatColor::darkGray);

	//we also need to set a material property for our objects
	//so we know how the light will reflect off of them
	material.setDiffuseColor(ofFloatColor::white);
	material.setShininess(10.0);


	mMeshController.setup();

	SharedGui::get()->finishSetup();

	mMeshController.createMesh();


	// start plane
	pNormal = ofVec3f(1, 0, 0);
	pNormal.normalize();

	pPos.set(0);

}


//--------------------------------------------------------------
void ofApp::update(){
    
	// map plane to mouse pos
	float angle = ofMap(ofGetMouseX(), 0, ofGetWindowWidth(), -180, 180);

	ofVec3f rotatedNorm = pNormal.getRotated(angle, ofVec3f(0,0,1));

	//pPos.x = mMeshController.getMeshSizeXY().x * 0.25f;
	mMeshController.setClippingPlane(pPos, rotatedNorm);
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
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == 'w') mMeshController.toggleWireframe();

	if (key == 's') SharedGui::get()->save();
	if (key == 'l') SharedGui::get()->load();
	if (key == 'g') bShowGui = !bShowGui;
    
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
