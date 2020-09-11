#include "GroundTextureMaker.h"



GroundTextureMaker::GroundTextureMaker() {

}

void GroundTextureMaker::setup(int width, int height) {

	
	//SharedGui::get()->getGroup("KINECT")->add(mNearClip.set("Near Clip", 0, 0, MAX_DEPTH));
	mFbo.clear();

	mFboSize.set(width, height);
	mFbo.allocate(mFboSize.x, mFboSize.y, GL_RGBA);

	mFbo.begin();
	ofClear(255, 0, 0, 255);
	mFbo.end();

	mShader.load("shaders/ground");
	mShader.setMillisBetweenFileCheck(200);
	

}

void GroundTextureMaker::update(ofVec2f leftEdge, ofVec2f rightEdge) {
	if (!mFbo.isAllocated()) return;

	mFbo.begin(); {

		ofClear(0, 0, 0, 1);
		mShader.begin(); {


			//accumulated.getTexture().bind();

			//we want to pass in some varrying values to animate our type / color
			mShader.setUniform1f("uTime", ofGetElapsedTimef());

			//we also pass in the mouse position
			mShader.setUniform2f("uMouse", ofGetMouseX(), ofGetHeight() - ofGetMouseY());
			mShader.setUniform2f("uResolution", mFboSize.x, mFboSize.y);
			mShader.setUniform2f("uLeft", leftEdge.x, leftEdge.y);
			mShader.setUniform2f("uRight", rightEdge.x, rightEdge.y);

			ofSetColor(255);
			ofFill();
			ofDrawRectangle(0, 0, mFboSize.x, mFboSize.y);

			//accumulated.getTexture().unbind();

		}mShader.end();

	}mFbo.end();

}
