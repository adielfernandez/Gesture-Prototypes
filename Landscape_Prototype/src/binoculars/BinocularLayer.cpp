#include "BinocularLayer.h"

BinocularLayer::BinocularLayer() {

}

void BinocularLayer::setup() {


	shader.load("shaders/binocularLayer");
	shader.setMillisBetweenFileCheck(200);

}

void BinocularLayer::update(ofVec2f position) {
	mPosition = position;

	float lerpSpeed = mGui->binocularAlphaLerpSpeed;

	if (bShouldShow) {
		mAlpha = ofLerp(mAlpha, 1.0f, lerpSpeed);
	} else {
		mAlpha = ofLerp(mAlpha, 0.0f, lerpSpeed);
	}
	//cout << "Alpha: " << mAlpha << endl;
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
		shader.setUniform1f("uMagnification", mMagnification);
		shader.setUniform1f("uAlpha", mAlpha);
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
