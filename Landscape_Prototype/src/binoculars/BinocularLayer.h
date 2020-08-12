#pragma once

#include "ofMain.h"
#include "../gui/Gui.h"
#include "ofxAutoReloadedShader.h"

class BinocularLayer {

public:

	BinocularLayer();

	void setup();
	void update(ofVec2f position);
	void draw();
	
	void setTexture(ofTexture tex) { mBgTex = tex; }
	void setShowing(bool show);
	void setGui(shared_ptr<Gui> gui) { mGui = gui; }


private:

	ofxAutoReloadedShader shader;


	ofVec2f mPosition;

	shared_ptr<Gui> mGui;

	ofTexture mBgTex;

	bool bShouldShow = false;

	float mMagnification = 1.0f;
	float mAlpha = 0.0f;

	float mTargetAlpha;
	float mChangeTime = 0;

};