#include "Canvas.h"



Canvas::Canvas() {

}

void Canvas::setup(int width, int height) {

	
	//SharedGui::get()->getGroup("KINECT")->add(mNearClip.set("Near Clip", 0, 0, MAX_DEPTH));


	mFboSize.set(width, height);
	mFbo.allocate(mFboSize.x, mFboSize.y, GL_RGBA32F);
	//mFboPix.allocate(mFboSize.x, mFboSize.y, OF_IMAGE_GRAYSCALE);

	mFbo.begin();
	ofClear(0, 0, 0, 0);
	mFbo.end();

	mAddShader.load("shaders/addToCanvas");
	mAddShader.setMillisBetweenFileCheck(200);

	mUpdateShader.load("shaders/updateCanvas");
	mUpdateShader.setMillisBetweenFileCheck(200);

}

void Canvas::update() {

	// take previous texture from canvas and pass it into the shader within the FBO
	ofTexture prevTex = mFbo.getTexture();

	mFbo.begin(); {

		mUpdateShader.begin(); {

			prevTex.bind();

			//we want to pass in some varrying values to animate our type / color
			mUpdateShader.setUniform1f("uTime", ofGetElapsedTimef());

			//we also pass in the mouse position
			mUpdateShader.setUniform2f("uMouse", ofGetMouseX(), ofGetHeight() - ofGetMouseY());
			mUpdateShader.setUniform2f("uResolution", mFboSize.x, mFboSize.y);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			ofSetColor(255);
			ofFill();
			ofDrawRectangle(0, 0, mFboSize.x, mFboSize.y);

			prevTex.unbind();

		}mUpdateShader.end();

	}mFbo.end();

}

void Canvas::clear() {
	mFbo.begin();
	ofClear(0, 0, 0, 0);
	mFbo.end();
}

void Canvas::add(ofTexture & gestureTex, ofColor color) {
	if (!gestureTex.isAllocated()) return;
	if (!mFbo.isAllocated()) return;

	ofLog(OF_LOG_NOTICE, "ADDING TEX TO CANVAS");

	mFbo.begin(); {

		mAddShader.begin(); {

			gestureTex.bind();

			//we want to pass in some varrying values to animate our type / color
			mAddShader.setUniform1f("uTime", ofGetElapsedTimef());

			//we also pass in the mouse position
			mAddShader.setUniform2f("uMouse", ofGetMouseX(), ofGetHeight() - ofGetMouseY());
			mAddShader.setUniform2f("uResolution", mFboSize.x, mFboSize.y);
			ofFloatColor f;
			f.set(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
			mAddShader.setUniform4f("uColor", f);

			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

			ofSetColor(255);
			ofFill();
			ofDrawRectangle(0, 0, mFboSize.x, mFboSize.y);

			gestureTex.unbind();

		}mAddShader.end();

	}mFbo.end();

}
