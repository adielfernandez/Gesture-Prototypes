#pragma once

#include "ofMain.h"
#include "../gui/SharedGui.h"
#include "../gui/FontManager.h"
#include "ofxCv.h"
#include "../Utils/Math.h"

class CVProcessor {

public:

	CVProcessor();
	~CVProcessor() {};


	void setup(int width, int height);
	void update();
	void process(ofPixels &pix);
	void process(ofShortPixels &pix);

	// for processing short pixels to 8 bit ofPixels
	void updateDepthLookupTable();

	void drawRawMapped(int x, int y, int w, int h);
	void drawBlurred(int x, int y, int w, int h);
	void drawBackground(int x, int y, int w, int h);
	void drawForeground(int x, int y, int w, int h);
	
	void drawContours();
	

private:

	void performCV(ofPixels &pix);

	ofVec2f mSize;

	//CV settings
	ofParameter<int>	mBlurAmt;
	ofParameter<int>	mNumErosions;
	ofParameter<int>	mNumDilations;
	ofParameter<int>	mThreshold;
	ofParameter<int>	mMinBlobArea;
	ofParameter<int>	mMaxBlobArea;
	ofParameter<int>	mPersistence;
	ofParameter<int>	mMaxBlobDist;

	ofParameter<short>	mNearClip;
	ofParameter<short>	mFarClip;
	
	// CV containers
	ofxCv::ContourFinder		mContours;
	ofImage						mRawImg;
	ofImage						mBlurredImg;
	ofImage						mThreshImg;
	ofImage						mForegroundImg;
	ofImage						mBackgroundImg;

	const short MAX_DEPTH = 4500;
	std::vector<char> depthLookupTable;

	unsigned short mPrevFarClip;
	unsigned short mNearFarClip;

};