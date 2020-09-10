#pragma once

#include "ofMain.h"
#include "../gui/SharedGui.h"
#include "../gui/FontManager.h"
#include "ofxAutoReloadedShader.h"
#include "../utils/Math.h"


class MeshController {

public:

	MeshController();
	~MeshController() {};

	void setup();
	void update();

	void createMesh();

	void setClippingPlane(ofVec3f pos, ofVec3f norm) { pPos = pos; pNormal = norm; }

	void draw();

	void toggleWireframe() { bWireframe = !bWireframe; }

private:

	ofImage mDepthImg;
	ofImage mColorPalette;
	ofImage mSampleTex;

	ofVboMesh mTopMesh;
	ofVboMesh mTopMeshOriginal;
	ofVboMesh mBottomMesh;
	
	ofVboMesh mFrontMesh;
	ofVboMesh mBackMesh;
	ofVboMesh mLeftMesh;
	ofVboMesh mRightMesh;

	ofVboMesh mCrossSection;

	// overall XY size of mesh
	ofVec2f mMeshSize;

	// spacing of XY mesh pts
	ofVec2f mGridSpacing;
	// spacing of sample points within pixel space of image
	ofVec2f mImgSpacing;

	float getGrayValFromImg(int x, int y);
	float getHeightForVal(float val);
	ofFloatColor getColorForHeight(float height);

	ofParameter<float>	mPctWaterLevel;
	ofParameter<float>	mMaxHeight;
	ofParameter<float>	mBottomZ;
	ofParameter<int>	mCrossSamples;
	ofParameter<int>	mResX;
	ofParameter<int>	mResY;
	ofParameter<bool>	bResetMesh;
	ofParameter<bool>	bShouldCrossSection;
	float				mButtonPressTime = 0;
	bool bLastShouldCrossSection = false;

	bool bWireframe = false;

	int getTotalVerts();

	// reveal plane
	ofVec3f pNormal;
	ofVec3f pPos;
	
	// returns the pixel coordinate for sampling an image
	// based on where the mesh/space it is
	ofVec2f getImgCoord(ofVec3f pos);


	float mMinImgDepthVal;
	float mMaxImgDepthVal;

};