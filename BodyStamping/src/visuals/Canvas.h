#pragma once

#include "ofMain.h"
#include "../gui/SharedGui.h"
#include "../gui/FontManager.h"
#include "ofxCv.h"
#include "ofxAutoReloadedShader.h"


class Canvas {

public:

	Canvas();
	~Canvas() {};

	void setup(int width, int height);
	void add(ofTexture & gestureTex, ofColor color);
	void update();

	void clear();

	ofFbo & getFbo() { return mFbo; }

private:

	ofFbo		mFbo;
	ofVec2f		mFboSize;

	ofxAutoReloadedShader mAddShader;
	ofxAutoReloadedShader mUpdateShader;

};