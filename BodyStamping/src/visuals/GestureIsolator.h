#pragma once

#include "ofMain.h"
#include "../gui/SharedGui.h"
#include "../gui/FontManager.h"
#include "ofxCv.h"
#include "ofxAutoReloadedShader.h"


// takes accumulated gesture shape and isolates it
// using an FBO and shader to cut out black pixels and add alpha

class GestureIsolator {

public:

	GestureIsolator();
	~GestureIsolator() {};

	void setup(int width, int height);
	void isolate(ofImage & accumulated, ofColor color);

	ofFbo & getFbo() { return mFbo; }

private:

	ofFbo		mFbo;
	ofVec2f		mFboSize;

	ofxAutoReloadedShader mShader;

};