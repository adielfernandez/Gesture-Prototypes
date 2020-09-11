#pragma once

#include "ofMain.h"
#include "../gui/SharedGui.h"
#include "../gui/FontManager.h"
#include "ofxAutoReloadedShader.h"
#include "../utils/Math.h"
#include "GroundTextureMaker.h"

class MeshController {

public:

	MeshController();
	~MeshController() {};

	void setup();
	void update();

	void createMesh();

	void setClippingPlane(ofVec3f pos, ofVec3f norm) { planePos = pos; planeNormal = norm; }

	void draw();
	void drawCrossSection();

	void toggleWireframe() { bWireframe = !bWireframe; }

	void setShouldCrossSection(bool should) { bShouldCrossSection = should; }

	ofVec2f getMeshSizeXY() { return mMeshSize; }

	ofVec3f getMaxSize() {
		return ofVec3f(mMeshSize.x, mMeshSize.y, mMaxHeight + abs(mBottomZ));
	}

	float getMaxBottom() { return mBottomZ; }
	
	GroundTextureMaker mGround;

private:


	ofImage mDepthImg;
	ofImage mColorPalette;

	ofVboMesh mTopMesh;
	ofVboMesh mTopMeshOriginal;
	ofVboMesh mBottomMesh;
	
	ofVboMesh mFrontMesh;
	ofVboMesh mBackMesh;
	ofVboMesh mLeftMesh;
	ofVboMesh mRightMesh;

	ofVboMesh mFrontMeshOriginal;
	ofVboMesh mBackMeshOriginal;
	ofVboMesh mLeftMeshOriginal;
	ofVboMesh mRightMeshOriginal;

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
	ofVec3f planeNormal;
	ofVec3f planePos;
	
	// returns the pixel coordinate for sampling an image
	// based on where the mesh/space it is
	ofVec2f getImgCoord(ofVec3f pos);


	float mMinImgDepthVal;
	float mMaxImgDepthVal;

};