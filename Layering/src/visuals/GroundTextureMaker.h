#pragma once

#include "ofMain.h"
#include "../gui/SharedGui.h"
#include "../gui/FontManager.h"
#include "ofxAutoReloadedShader.h"


// takes accumulated gesture shape and isolates it
// using an FBO and shader to cut out black pixels and add alpha

class GroundTextureMaker {

public:

	GroundTextureMaker();
	~GroundTextureMaker() {};

	void setup(int width, int height);

	void update(ofVec2f leftEdge, ofVec2f rightEdge);

	ofFbo & getFbo() { return mFbo; }
	ofTexture & getTexture() { return mFbo.getTexture(); }
	bool isAllocated() { return mFbo.isAllocated(); }
private:


	ofFbo		mFbo;
	ofVec2f		mFboSize;

	ofxAutoReloadedShader mShader;

};