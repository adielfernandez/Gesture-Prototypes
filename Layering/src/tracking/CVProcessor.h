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
	void processDepth(ofPixels &pix);
	void processDepth(ofShortPixels &pix);

	// for processing short pixels to 8 bit ofPixels
	void updateDepthLookupTable();

	void drawRawMapped(int x, int y, int w, int h);
	void drawThresholdedAndBlurred(int x, int y, int w, int h);
	void drawAccumulated(int x, int y, int w, int h);

	void setRecording(bool record);
	bool isRecording() { return bRecording;  }

	ofImage & getAccumulatedImg() { return mAccumulatedImg; }

private:

	void performCV(ofPixels &pix);

	ofVec2f mSize;

	//CV settings
	ofParameter<int>	mBlurAmt;
	ofParameter<int>	mNumErosions;
	ofParameter<int>	mNumDilations;
	ofParameter<int>	mThreshold;

	ofParameter<short>	mNearClip;
	ofParameter<short>	mFarClip;

	ofParameter<int>	mDecay;
	
	// CV containers
	ofxCv::ContourFinder		mContours;
	ofImage						mRawImg;
	ofImage						mBlurredImg;
	ofImage						mThreshImg;

	ofImage						mAccumulatedImg;

	ofPixels					mDecayPix;

	const short MAX_DEPTH = 4500;
	std::vector<char> depthLookupTable;

	unsigned short mPrevFarClip;
	unsigned short mNearFarClip;
	int mPrevDecay = 0;

	bool bRecording = false;

};