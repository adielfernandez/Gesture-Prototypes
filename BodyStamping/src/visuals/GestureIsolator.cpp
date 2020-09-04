#include "GestureIsolator.h"



GestureIsolator::GestureIsolator() {

}

void GestureIsolator::setup(int width, int height) {

	
	//SharedGui::get()->getGroup("KINECT")->add(mNearClip.set("Near Clip", 0, 0, MAX_DEPTH));


	mFboSize.set(width, height);
	mFbo.allocate(mFboSize.x, mFboSize.y, GL_RGBA);
	//mFboPix.allocate(mFboSize.x, mFboSize.y, OF_IMAGE_GRAYSCALE);

	mFbo.begin();
	ofClear(255, 0, 0, 255);
	mFbo.end();

	mShader.load("shaders/isolator");
	mShader.setMillisBetweenFileCheck(200);
	

}

void GestureIsolator::isolate(ofImage & accumulated, ofColor color) {
	if (!accumulated.isAllocated()) return;
	if (!mFbo.isAllocated()) return;

	mFbo.begin(); {

		ofClear(0, 0, 0, 0);
		mShader.begin(); {


			accumulated.getTexture().bind();

			//we want to pass in some varrying values to animate our type / color
			mShader.setUniform1f("uTime", ofGetElapsedTimef());

			//we also pass in the mouse position
			mShader.setUniform2f("uMouse", ofGetMouseX(), ofGetHeight() - ofGetMouseY());
			mShader.setUniform2f("uResolution", mFboSize.x, mFboSize.y);
			ofFloatColor f;
			f.set(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
			mShader.setUniform4f("uColor", f);

			ofSetColor(255);
			ofFill();
			ofDrawRectangle(0, 0, mFboSize.x, mFboSize.y);

			accumulated.getTexture().unbind();

		}mShader.end();

	}mFbo.end();

}
