#pragma once

#include "ofMain.h"
#include "../gui/SharedGui.h"
#include "../gui/FontManager.h"
#include "ofxAutoReloadedShader.h"


class MeshController {

public:

	MeshController();
	~MeshController() {};

	void setup();
	void update();

	void creatMesh(std::string imagePath = "images/everest.png");

	void draw();

	void toggleWireframe() { bWireframe = !bWireframe; }

private:

	ofImage mDepthImg;

	ofVboMesh mTopMesh;
	ofVboMesh mTopMeshOriginal;
	ofVboMesh mBottomMesh;
	
	ofVboMesh mFrontMesh;
	ofVboMesh mBackMesh;
	ofVboMesh mLeftMesh;
	ofVboMesh mRightMesh;

	const int resX = 200;
	const int resY = 160;

	// overall XY size of mesh
	ofVec2f mMeshSize;

	// spacing of XY mesh pts
	ofVec2f mGridSpacing;
	// spacing of sample points within pixel space of image
	ofVec2f mImgSpacing;

	float getGrayValFromImg(int x, int y);
	float getHeightForVal(float val);


	ofParameter<float>	mMaxHeight;
	ofParameter<float>	mBottomZ;

	bool bWireframe = false;

	int getTotalVerts();

};