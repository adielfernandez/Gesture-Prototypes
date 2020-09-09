#include "CVProcessor.h"



CVProcessor::CVProcessor() {

}

void CVProcessor::setup(int width, int height) {

	mSize.x = width;
	mSize.y = height;


	SharedGui::get()->getGroup("KINECT")->add(mNearClip.set("Near Clip", 0, 0, MAX_DEPTH));
	SharedGui::get()->getGroup("KINECT")->add(mFarClip.set("Far Clip", MAX_DEPTH, 0, MAX_DEPTH));

	SharedGui::get()->getGroup("VISION")->add(mBlurAmt.set("Blur Amount", 0, 0, 30));
	SharedGui::get()->getGroup("VISION")->add(mNumErosions.set("Num Erosions", 0, 0, 20));
	SharedGui::get()->getGroup("VISION")->add(mNumDilations.set("Num Dilations", 0, 0, 20));
	SharedGui::get()->getGroup("VISION")->add(mThreshold.set("Threshold", 0, 0, 255));
	SharedGui::get()->getGroup("VISION")->add(mDecay.set("Decaay", 1, 0, 20));

	mRawImg.allocate(mSize.x, mSize.y, OF_IMAGE_GRAYSCALE);
	mRawImg.setColor(ofColor(0));

	mBlurredImg.allocate(mSize.x, mSize.y, OF_IMAGE_GRAYSCALE);
	mBlurredImg.setColor(ofColor(0));

	mThreshImg.allocate(mSize.x, mSize.y, OF_IMAGE_GRAYSCALE);
	mThreshImg.setColor(ofColor(0));

	mAccumulatedImg.allocate(mSize.x, mSize.y, OF_IMAGE_GRAYSCALE);
	mAccumulatedImg.setColor(ofColor(0));

	mDecayPix.allocate(mSize.x, mSize.y, OF_IMAGE_GRAYSCALE);
	mDecayPix.setColor(ofColor(mDecay));
	

	updateDepthLookupTable();
}

void CVProcessor::update() {

	if (mPrevFarClip != mFarClip || mNearFarClip != mNearClip) {
		ofLog(OF_LOG_NOTICE, "UPDATING DEPTH LUT");
		updateDepthLookupTable();
	}

	if (mPrevDecay != mDecay) {
		ofLog(OF_LOG_NOTICE, "RESETTING DECAY PIXELS");
		mDecayPix.setColor(ofColor(mDecay));
		mPrevDecay = mDecay;
	}

	if (mAccumulatedImg.isAllocated() && mDecayPix.isAllocated() && isRecording()) {
		ofxCv::subtract(mAccumulatedImg.getPixels(), mDecayPix, mAccumulatedImg.getPixels());
		mAccumulatedImg.update();
	}
}

void CVProcessor::setRecording(bool record) {
	if (record == bRecording) return;

	bRecording = record;

	if (bRecording == false && mAccumulatedImg.isAllocated()) {
		mAccumulatedImg.getPixels().setColor(ofColor(0));
		mAccumulatedImg.update();
	}
}

void CVProcessor::updateDepthLookupTable() {
	depthLookupTable.resize(MAX_DEPTH);

	// Depth values of 0 should be discarded, so set the LUT value to 0 as well
	depthLookupTable[0] = 0;

	// Set the rest
	for (int i = 1; i < depthLookupTable.size(); i++) {
		depthLookupTable[i] = ofMap(i, mNearClip, mFarClip, 255, 0, true);
	}

	mPrevFarClip = mFarClip;
	mNearFarClip = mNearClip;
}

void CVProcessor::processDepth(ofShortPixels &shortPix) {
	if (!shortPix.isAllocated()) return;

	for (int y = 0; y < shortPix.getHeight(); y++) {
		for (int x = 0; x < shortPix.getWidth(); x++) {
			int index = (y * shortPix.getWidth()) + x;
			float depth = ofClamp(shortPix[index], 0, depthLookupTable.size() - 1);
			float val = depthLookupTable[depth];
			mRawImg.setColor(x, y, ofColor(val));
		}
	}

	mRawImg.update();

	performCV(mRawImg.getPixels());

}

void CVProcessor::processDepth(ofPixels &pix) {
	
	mRawImg.setFromPixels(pix);
	mRawImg.update();

	performCV(pix);

}

void CVProcessor::performCV(ofPixels &pix) {

	//threshold it
	ofxCv::threshold(pix, mThreshImg.getPixels(), mThreshold);
	mThreshImg.update();

	// transfer to new container
	mBlurredImg.setFromPixels(mThreshImg.getPixels());

	// DILATE it
	for (int d = 0; d < mNumDilations; d++) {
		ofxCv::dilate(mBlurredImg.getPixels());
	}

	// BLUR it
	ofxCv::GaussianBlur(mBlurredImg.getPixels(), mBlurAmt);

	// ERODE it
	for (int e = 0; e < mNumErosions; e++) {
		ofxCv::erode(mThreshImg.getPixels());
	}
	mBlurredImg.update();

	if (bRecording) {
		ofxCv::add(mBlurredImg.getPixels(), mAccumulatedImg.getPixels(), mAccumulatedImg.getPixels());
		mAccumulatedImg.update();
	}

	

}

void CVProcessor::drawRawMapped(int x, int y, int w, int h) {
	float scaleX = w / mSize.x;
	float scaleY = h / mSize.y;

	ofPushMatrix();
	ofTranslate(x, y);
	ofScale(scaleX, scaleY);

	ofPushStyle();
	ofNoFill();
	ofSetColor(255);
	mRawImg.draw(0, 0);
	ofSetLineWidth(1);
	ofDrawRectangle(0, 0, mSize.x, mSize.y);
	float titleSize = 10;
	FontManager::get()->drawString("Raw Mapped Image", 0, -5, titleSize, "futura_light");


	ofPopStyle();

	ofPopMatrix();

}

void CVProcessor::drawThresholdedAndBlurred(int x, int y, int w, int h) {

	float scaleX = w / mSize.x;
	float scaleY = h / mSize.y;

	ofPushMatrix();
	ofTranslate(x, y);
	ofScale(scaleX, scaleY);

	ofPushStyle();
	ofNoFill();
	ofSetColor(255);
	mBlurredImg.draw(0, 0);
	ofSetLineWidth(1);
	ofDrawRectangle(0, 0, mSize.x, mSize.y);
	float titleSize = 10;
	FontManager::get()->drawString("Blurred Image", 0, -5, titleSize, "futura_light");
	

	ofPopStyle();

	ofPopMatrix();
}
	
void CVProcessor::drawAccumulated(int x, int y, int w, int h) {

	float scaleX = w / mSize.x;
	float scaleY = h / mSize.y;

	ofPushMatrix();
	ofTranslate(x, y);
	ofScale(scaleX, scaleY);

	ofPushStyle();
	ofNoFill();
	ofSetColor(255);
	mAccumulatedImg.draw(0, 0);
	ofSetLineWidth(1);
	ofDrawRectangle(0, 0, mSize.x, mSize.y);
	float titleSize = 10;
	FontManager::get()->drawString("Accumulated Image", 0, -5, titleSize, "futura_light");

	ofPopStyle();

	ofPopMatrix();


}
