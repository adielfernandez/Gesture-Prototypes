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

	img1.load("images/granite.jpg");
	img2.load("images/schist.jpg");
	img3.load("images/sedimentary.jpg");
	img4.load("images/clay.jpg");
	img5.load("images/soil.jpg");

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

			mShader.setUniformTexture("tex1", img1.getTexture(), 1);
			mShader.setUniformTexture("tex2", img2.getTexture(), 2);
			mShader.setUniformTexture("tex3", img3.getTexture(), 3);
			mShader.setUniformTexture("tex4", img4.getTexture(), 4);
			mShader.setUniformTexture("tex5", img5.getTexture(), 5);

			ofSetColor(255);
			ofFill();
			ofDrawRectangle(0, 0, mFboSize.x, mFboSize.y);

			//accumulated.getTexture().unbind();

		}mShader.end();

	}mFbo.end();

}
