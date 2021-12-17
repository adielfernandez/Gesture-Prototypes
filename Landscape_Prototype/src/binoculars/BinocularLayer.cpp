#include "BinocularLayer.h"

BinocularLayer::BinocularLayer() {

}

void BinocularLayer::setup() {


	shader.load("shaders/binocularLayer");
	shader.setMillisBetweenFileCheck(200);

}

void BinocularLayer::update(ofVec2f position) {
	
	mPosition.x = ofLerp(mPosition.x, position.x, mGui->binPosLerp);
	mPosition.y = position.y;



	float lerpSpeed = mGui->binocularAlphaLerpSpeed;

	if (bShouldShow) {
		mAlpha = ofLerp(mAlpha, 1.0f, lerpSpeed);
	} else {
		mAlpha = ofLerp(mAlpha, 0.0f, lerpSpeed);
	}
	//cout << "Alpha: " << mAlpha << endl;


	mMagnification = ofLerp(mMagnification, mMagTarget, mGui->binZoomLerp);

}

void BinocularLayer::setShowing(bool show) {
	if (bShouldShow == show) return;


	bShouldShow = show;

}

void BinocularLayer::draw() {


	shader.begin(); {


		mBgTex.bind();

		//we want to pass in some varrying values to animate our type / color
		shader.setUniform1f("uTime", ofGetElapsedTimef());

		//we also pass in the mouse position
		shader.setUniform2f("uMouse", ofGetMouseX(), ofGetHeight() - ofGetMouseY());
		shader.setUniform2f("uPosition", mPosition.x, ofGetHeight() - mPosition.y);
		shader.setUniform2f("uResolution", ofGetWidth(), ofGetHeight());
		shader.setUniform2f("uRawImageSize", mRawImgSize.x, mRawImgSize.y);
		shader.setUniform1f("uMagnification", mMagnification);
		shader.setUniform1f("uDisplayYShift", mGui->binDisplayYShift);
		shader.setUniform1f("uAlpha", mAlpha);

		// shader is set to display on a 1920x1080 window.
		// scale it down if we're not at that size
		float scale = mGui->binScale * ofGetWidth() / 1920.0;

		shader.setUniform1f("uBinScale", scale);
		//shader.setUniformTexture("uImageTex", mBgTex, 0);
		//shader.setUniform1f("fogLayers", gui->fogLayers);
		//shader.setUniform1f("driftSpeed", gui->driftSpeed);
		//shader.setUniform1f("noiseSpeed", gui->noiseSpeed);
		//shader.setUniform1f("noiseScale", gui->noiseScale);
		//shader.setUniform1f("ySquish", gui->ySquish);
		//shader.setUniform1f("fogTrans", gui->fogTrans);
		//shader.setUniform1f("fogMidpoint", gui->fogMidpoint);
		//shader.setUniform1f("fogSharpness", gui->fogBoundSharpness);
		//shader.setUniform1f("octaveDiff", gui->octaveDiff);


		ofSetColor(255);
		ofFill();
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

		mBgTex.unbind();

	}shader.end();


}
