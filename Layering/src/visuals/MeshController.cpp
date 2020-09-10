#include "MeshController.h"



MeshController::MeshController() {

}

void MeshController::setup() {

	SharedGui::get()->getGroup("MESH")->add(mMaxHeight.set("Max Terrain Height", 250, 0, 600));
	SharedGui::get()->getGroup("MESH")->add(mBottomZ.set("Bottom Depth", -250, -1000, 0));

}

void MeshController::creatMesh(std::string imagePath) {

	mTopMesh.clear();
	mBottomMesh.clear();
	mFrontMesh.clear();
	mBackMesh.clear();
	mLeftMesh.clear();
	mRightMesh.clear();

	mDepthImg.clear();


	mDepthImg.load(imagePath);
	mDepthImg.setImageType(OF_IMAGE_GRAYSCALE);


	float width = 1000;
	mMeshSize = ofVec2f(width, width * (mDepthImg.getHeight() / mDepthImg.getWidth()));

	ofPlanePrimitive planeXY;
	planeXY.set(mMeshSize.x, mMeshSize.y, resX, resY);
	mTopMesh = planeXY.getMesh();
	mBottomMesh = planeXY.getMesh();

	//auto idxs = planeXY.getMesh().getIndices();
	//cout << planeXY.getMesh().getMode() << endl;
	//for (int i = 0; i < idxs.size(); i++) {
	//	cout << idxs[i] << endl;
	//}

	mGridSpacing = mMeshSize / ofVec2f(resX - 1, resY - 1);
	mImgSpacing = ofVec2f(mDepthImg.getWidth() / (float)resX, mDepthImg.getHeight() / (float)resY);

	mFrontMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mBackMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mLeftMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mRightMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

	ofFloatColor boxGray = ofFloatColor(0.6, 1.0f);

	// go through the plane and move each vertex to where we need it
	for (int i = 0; i < mTopMesh.getNumVertices(); i++) {

		int x = i % resX;
		int y = i / (float)resX;

		float gray = getGrayValFromImg(x * mImgSpacing.x, y * mImgSpacing.y);
		ofFloatColor imgColor = ofFloatColor(gray / 255.0f, 1.0f);
		float height = getHeightForVal(gray);

		ofVec2f pos = ofVec2f(x * mGridSpacing.x, y * mGridSpacing.y) - mMeshSize / 2.0f;
		
		ofVec3f top = ofVec3f(pos.x, pos.y, height);
		ofVec3f bottom = ofVec3f(pos.x, pos.y, mBottomZ);

		mTopMesh.setVertex(i, top);
		mTopMesh.addColor(ofFloatColor(gray / 255.0f, 1.0f));


		// set the bottom depth at a gray color
		mBottomMesh.setVertex(i, bottom);
		mBottomMesh.addColor(boxGray);


		// build the sides
		if (y == 0) {
			// build Front

			// add verts for top and bottom
			mFrontMesh.addVertex(top);
			mFrontMesh.addVertex(bottom);
			mFrontMesh.addColor(imgColor);
			mFrontMesh.addColor(boxGray);

		} else if (y == (resY - 1)) {
			// build back
			mBackMesh.addVertex(top);
			mBackMesh.addVertex(bottom);
			mBackMesh.addColor(imgColor);
			mBackMesh.addColor(boxGray);
		} else if (x == 0) {
			// RIGHT mesh
			mRightMesh.addVertex(top);
			mRightMesh.addVertex(bottom);
			mRightMesh.addColor(imgColor);
			mRightMesh.addColor(boxGray);

		} else if (x == (resX - 1)) {
			// LEFT mesh
			mLeftMesh.addVertex(top);
			mLeftMesh.addVertex(bottom);
			mLeftMesh.addColor(imgColor);
			mLeftMesh.addColor(boxGray);
		}

	}

	mTopMeshOriginal.clear();
	mTopMeshOriginal = mTopMesh;

	ofLogNotice("Total Verts: " + ofToString(getTotalVerts()));

}

void MeshController::update() {

	// TEST: set alpha to zero if vertices have y greater than mousey
	float hw = mMeshSize.y / 2.0f;
	float y = ofMap(ofGetMouseX(), 0, ofGetWindowWidth(), hw + 100, -hw - 100);


	ofFloatColor boyGray = ofFloatColor(0.6, 1.0f);
	ofFloatColor boyClear = ofFloatColor(0.6, 0.0f);

	//for (int i = 0; i < mTopMesh.getNumVertices(); i++) {
	//	auto pos = mTopMesh.getVertex(i);
	//	if (pos.y < y) {
	//		mTopMesh.setColor(i, boyClear);
	//	} else {
	//		mTopMesh.setColor(i, mTopMeshOriginal.getColor(i));
	//	}
	//}

	//for (int i = 0; i < mBottomMesh.getNumVertices(); i++) {
	//	if (mBottomMesh.getVertex(i).y < y) mBottomMesh.setColor(i, boyClear);
	//	else								mBottomMesh.setColor(i, boyGray);
	//}

	//for (int i = 0; i < mFrontMesh.getNumVertices(); i++) {
	//	if (mFrontMesh.getVertex(i).y < y)  mFrontMesh.setColor(i, boyClear);
	//	else								mFrontMesh.setColor(i, boyGray);
	//}
	//for (int i = 0; i < mBackMesh.getNumVertices(); i++) {
	//	if (mBackMesh.getVertex(i).y < y)	mBackMesh.setColor(i, boyClear);
	//	else								mBackMesh.setColor(i, boyGray);
	//}
	//for (int i = 0; i < mLeftMesh.getNumVertices(); i++) {
	//	if (mLeftMesh.getVertex(i).y < y)	mLeftMesh.setColor(i, boyClear);
	//	else								mLeftMesh.setColor(i, boyGray);
	//}
	//for (int i = 0; i < mRightMesh.getNumVertices(); i++) {
	//	if (mRightMesh.getVertex(i).y < y)	mRightMesh.setColor(i, boyClear);
	//	else								mRightMesh.setColor(i, boyGray);
	//}

}

void MeshController::draw() {

	if (bWireframe) {
		mBottomMesh.drawWireframe();
		mFrontMesh.drawWireframe();
		mBackMesh.drawWireframe();
		mLeftMesh.drawWireframe();
		mRightMesh.drawWireframe();
		mTopMesh.drawWireframe();
	} else {
		mBottomMesh.draw();
		mFrontMesh.draw();
		mBackMesh.draw();
		mLeftMesh.draw();
		mRightMesh.draw();
		mTopMesh.draw();
	}


}

float MeshController::getGrayValFromImg(int x, int y) {
	if (!mDepthImg.isAllocated()) return -1;
	return mDepthImg.getColor(x, y).r;
}

float MeshController::getHeightForVal(float val) {
	return ofMap(val, 0, 255, 0, mMaxHeight);
}

int MeshController::getTotalVerts()
{
	int total = mTopMesh.getNumVertices()
				+ mBottomMesh.getNumVertices()
				+ mFrontMesh.getNumVertices()
				+ mBackMesh.getNumVertices()
				+ mLeftMesh.getNumVertices()
				+ mRightMesh.getNumVertices();
	return total;
}
