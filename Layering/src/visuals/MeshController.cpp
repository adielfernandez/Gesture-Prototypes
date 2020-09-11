#include "MeshController.h"



MeshController::MeshController() {

}

void MeshController::setup() {

	SharedGui::get()->getGroup("MESH")->add(mResX.set("Resolution X", 100, 0, 800));
	SharedGui::get()->getGroup("MESH")->add(mResY.set("Resolution Y", 80, 0, 800));
	SharedGui::get()->getGroup("MESH")->add(mMaxHeight.set("Max Terrain Height", 250, 0, 600));
	SharedGui::get()->getGroup("MESH")->add(mBottomZ.set("Bottom Depth", -250, -1000, 0));
	SharedGui::get()->getGroup("MESH")->add(mCrossSamples.set("Cross Section Samples", 200, 2, 400));
	SharedGui::get()->getGroup("MESH")->add(bResetMesh.set("Reset Mesh", false));
	SharedGui::get()->getGroup("MESH")->add(bShouldCrossSection.set("Perform Cross Section", true));
	SharedGui::get()->getGroup("MESH")->add(mPctWaterLevel.set("Water Level Pct", 0.2, 0, 1.0));

	std::string imagePath = "images/grandcanyon.png";

	mDepthImg.clear();

	mDepthImg.load(imagePath);
	mDepthImg.setImageType(OF_IMAGE_GRAYSCALE);

	// process the image and find the lows and highs
	auto & pix = mDepthImg.getPixels();

	mMinImgDepthVal = std::numeric_limits<float>::max();
	mMaxImgDepthVal = std::numeric_limits<float>::min();


	for (int i = 0; i < pix.getWidth() * pix.getHeight(); i++) {
		float val = pix.getColor(i).r;
		if (val < mMinImgDepthVal) mMinImgDepthVal = val;
		if (val > mMaxImgDepthVal) mMaxImgDepthVal = val;
	}

	ofLogNotice("Image Min Depth: " + ofToString(mMinImgDepthVal) + ", Max Depth: " + ofToString(mMaxImgDepthVal));

	mColorPalette.load("images/colorPalette.png");

	mSampleTex.load("images/tex.png");

	mGround.setup(1000, 1000);

}

void MeshController::createMesh() {

	mTopMesh.clear();
	mBottomMesh.clear();
	mFrontMesh.clear();
	mBackMesh.clear();
	mLeftMesh.clear();
	mRightMesh.clear();
	mCrossSection.clear();



	float width = 1000;
	mMeshSize = ofVec2f(width, width * (mDepthImg.getHeight() / mDepthImg.getWidth()));

	ofPlanePrimitive planeXY;
	planeXY.set(mMeshSize.x, mMeshSize.y, mResX, mResY);
	mTopMesh = planeXY.getMesh();
	//mBottomMesh = planeXY.getMesh();

	//auto idxs = planeXY.getMesh().getIndices();
	//cout << planeXY.getMesh().getMode() << endl;
	//for (int i = 0; i < idxs.size(); i++) {
	//	cout << idxs[i] << endl;
	//}

	mGridSpacing = mMeshSize / ofVec2f(mResX - 1, mResY - 1);
	mImgSpacing = ofVec2f(mDepthImg.getWidth() / (float)mResX, mDepthImg.getHeight() / (float)mResY);

	mFrontMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mBackMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mLeftMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mRightMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	mCrossSection.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

	ofFloatColor boxGray = ofFloatColor(0.0f, 0.2f, 0.0f, 1.0f);


	// go through the plane and move each vertex to where we need it
	for (int i = 0; i < mTopMesh.getNumVertices(); i++) {

		int x = i % mResX;
		int y = i / (float)mResX;

		float gray = getGrayValFromImg(x * mImgSpacing.x, y * mImgSpacing.y);
		//ofFloatColor imgColor = ofFloatColor(gray / 255.0f, 1.0f);

		ofVec2f pos = ofVec2f(x * mGridSpacing.x, y * mGridSpacing.y) - mMeshSize / 2.0f;
		
		// cap the top of the mesh to make a fake water level
		float height = getHeightForVal(gray);

		ofVec3f top = ofVec3f(pos.x, pos.y, height);
		ofVec3f bottom = ofVec3f(pos.x, pos.y, mBottomZ);

		ofFloatColor topColor = getColorForHeight(height);

		mTopMesh.setVertex(i, top);
		//mTopMesh.addColor(ofFloatColor(gray / 255.0f, 1.0f));
		mTopMesh.addColor(topColor);

		// set the bottom depth at a gray color
		//mBottomMesh.setVertex(i, bottom);
		//mBottomMesh.addColor(boxGray);

		// tex coord of top of side mesh (0 at bottom, 1 at top
		float topTexCoordY = (mBottomZ + height) / (mMaxHeight + mBottomZ);
		ofVec2f hm = mMeshSize / 2.0f;

		// build the sides
		if (y == 0) {
			// build Front

			// add verts for top and bottom
			mFrontMesh.addVertex(top);
			mFrontMesh.addVertex(bottom);

			//float texCoordX = ofMap(pos.x, -hm.x, hm.x, 0.0f, 1.0f);
			//mFrontMesh.addTexCoord(ofVec2f(texCoordX, topTexCoordY));	// top
			//mFrontMesh.addTexCoord(ofVec2f(texCoordX, 0));				// bottom

			mFrontMesh.addColor(topColor);
			mFrontMesh.addColor(boxGray);

		} else if (y == (mResY - 1)) {
			// build back
			mBackMesh.addVertex(top);
			mBackMesh.addVertex(bottom);

			//float texCoordX = ofMap(pos.x, -hm.x, hm.x, 0.0f, 1.0f);
			//mFrontMesh.addTexCoord(ofVec2f(texCoordX, topTexCoordY));	// top
			//mFrontMesh.addTexCoord(ofVec2f(texCoordX, 0));				// bottom

			mBackMesh.addColor(topColor);
			mBackMesh.addColor(boxGray);
		} else if (x == 0) {
			// RIGHT mesh
			mRightMesh.addVertex(top);
			mRightMesh.addVertex(bottom);

			//float texCoordX = ofMap(pos.y, -hm.y, hm.y, 0.0f, 1.0f);
			//mFrontMesh.addTexCoord(ofVec2f(texCoordX, topTexCoordY));	// top
			//mFrontMesh.addTexCoord(ofVec2f(texCoordX, 0));				// bottom
			mRightMesh.addColor(topColor);
			mRightMesh.addColor(boxGray);

		} else if (x == (mResX - 1)) {
			// LEFT mesh
			mLeftMesh.addVertex(top);
			mLeftMesh.addVertex(bottom);

			//float texCoordX = ofMap(pos.y, -hm.y, hm.y, 0.0f, 1.0f);
			//mFrontMesh.addTexCoord(ofVec2f(texCoordX, topTexCoordY));	// top
			//mFrontMesh.addTexCoord(ofVec2f(texCoordX, 0));				// bottom
			mLeftMesh.addColor(topColor);
			mLeftMesh.addColor(boxGray);
		}

	}

	mTopMeshOriginal.clear();
	mTopMeshOriginal = mTopMesh;

	mFrontMeshOriginal.clear();
	mBackMeshOriginal.clear();
	mLeftMeshOriginal.clear();
	mRightMeshOriginal.clear();

	mFrontMeshOriginal = mFrontMesh;
	mBackMeshOriginal = mBackMesh;
	mLeftMeshOriginal = mLeftMesh;
	mRightMeshOriginal = mRightMesh;

	ofLogNotice("Total Verts: " + ofToString(getTotalVerts()));

}

void MeshController::update() {

	if (bResetMesh && ofGetElapsedTimef() - mButtonPressTime > 0.25f) {
		createMesh();
		mButtonPressTime = ofGetElapsedTimef();
		bResetMesh = false;
	}


	if (bShouldCrossSection) {

		// TEST: set alpha to zero if vertices have y greater than mousey
		float hw = mMeshSize.y / 2.0f;
		float y = ofMap(ofGetMouseX(), 0, ofGetWindowWidth(), hw + 100, -hw - 100);


		ofFloatColor boyGray = ofFloatColor(0.6f, 1.0f);
		ofFloatColor boxClear = ofFloatColor(0, 0, 0, 0.0f);


		for (int i = 0; i < mTopMesh.getNumVertices(); i++) {
			auto pos = mTopMesh.getVertex(i);
			if (checkWhichSideOfPlane(pos, planeNormal, planePos) > 0) {
				mTopMesh.setColor(i, boxClear);
			}
			else {
				mTopMesh.setColor(i, mTopMeshOriginal.getColor(i));
			}
		}

		//for (int i = 0; i < mBottomMesh.getNumVertices(); i++) {
		//	auto pos = mBottomMesh.getVertex(i);
		//	if (checkWhichSideOfPlane(pos, pNormal, pPos) > 0)  mBottomMesh.setColor(i, boxClear);
		//	else								mBottomMesh.setColor(i, boyGray);
		//}

		mFrontMesh = mFrontMeshOriginal;
		mBackMesh = mBackMeshOriginal;
		mLeftMesh = mLeftMeshOriginal;
		mRightMesh = mRightMeshOriginal;

		for (int i = 0; i < mFrontMesh.getNumVertices(); i++) {
			auto pos = mFrontMesh.getVertex(i);
			if (checkWhichSideOfPlane(pos, planeNormal, planePos) > 0)  mFrontMesh.setColor(i, boxClear);
		}
		for (int i = 0; i < mBackMesh.getNumVertices(); i++) {
			auto pos = mBackMesh.getVertex(i);
			if (checkWhichSideOfPlane(pos, planeNormal, planePos) > 0) 	mBackMesh.setColor(i, boxClear);
		}
		for (int i = 0; i < mLeftMesh.getNumVertices(); i++) {
			auto pos = mLeftMesh.getVertex(i);
			if (checkWhichSideOfPlane(pos, planeNormal, planePos) > 0) 	mLeftMesh.setColor(i, boxClear);
		}
		for (int i = 0; i < mRightMesh.getNumVertices(); i++) {
			auto pos = mRightMesh.getVertex(i);
			if (checkWhichSideOfPlane(pos, planeNormal, planePos) > 0) 	mRightMesh.setColor(i, boxClear);
		}

		// build the cross section mesh (ONLY WORKS FOR PLANES WITH Z = 0)

		// first get the far XY edges of the cross section
		ofVec2f dir = ofVec2f(planeNormal).getRotated(-90);

		ofVec2f left = getRectIntersection(dir, planePos, mMeshSize) + planePos;
		ofVec2f right = getRectIntersection(-dir, planePos, mMeshSize) + planePos;
		ofVec2f leftToRight = right - left;

		//assemble mesh
		ofFloatColor green(0.0, 1.0, 0.0, 1.0);
		mCrossSection.clear();



		// Starting at left, go through the image and get the heights of the top mesh
		// then add them in a triangle strip (adding bottom of cross section every other point
		for (int i = 0; i <= mCrossSamples; i++) {

			// walk a line across the mesh and sample the image to create the top of the cross section
			float pct = i / (float)mCrossSamples;

			ofVec2f lengthAlongSection = leftToRight * pct;
			ofVec2f meshCoord = left + lengthAlongSection;
			ofVec2f sampleCoord = getImgCoord(meshCoord);
			float imgVal = getGrayValFromImg(sampleCoord.x, sampleCoord.y);
			float height = getHeightForVal(imgVal);

			mCrossSection.addVertex(ofVec3f(meshCoord.x, meshCoord.y, height));
			mCrossSection.addVertex(ofVec3f(meshCoord.x, meshCoord.y, mBottomZ));

			float xCoord = pct * mGround.getFbo().getWidth();
			mCrossSection.addTexCoord(ofVec2f(xCoord, 1000));
			mCrossSection.addTexCoord(ofVec2f(xCoord, 0.0f));
			
			//mCrossSection.addTexCoord(ofVec2f(pct, 1.0f));
			//mCrossSection.addTexCoord(ofVec2f(pct, 0.0f));

			//mCrossSection.addColor(green);
			//mCrossSection.addColor(green);
		}

		mGround.update(left, right);

		//ofLogNotice("LEFT: " + ofToString(left) + "RIGHT: " + ofToString(right));


	} else {

		// if we just turned off the cross section, reset everything
		if (bLastShouldCrossSection) {
			createMesh();

		}


	}


	bLastShouldCrossSection = bShouldCrossSection;

}

void MeshController::draw() {



	if (bWireframe) {
		ofSetLineWidth(1);
		mCrossSection.drawWireframe();
		mTopMesh.drawWireframe();
		mBottomMesh.drawWireframe();
		mFrontMesh.drawWireframe();
		mBackMesh.drawWireframe();
		mLeftMesh.drawWireframe();
		mRightMesh.drawWireframe();
	} else {

		if (mGround.isAllocated()) {
			//ofDisableArbTex();
			//ofEnableNormalizedTexCoords();
			mGround.getTexture().bind();
			mCrossSection.draw();
			mGround.getTexture().unbind();
			//ofDisableNormalizedTexCoords();
			//ofEnableArbTex();
		} else {
			mCrossSection.draw();
		}

			mTopMesh.draw();
			mBottomMesh.draw();
			mFrontMesh.draw();
			mBackMesh.draw();
			mLeftMesh.draw();
			mRightMesh.draw();
		

	}


}

float MeshController::getGrayValFromImg(int x, int y) {
	if (!mDepthImg.isAllocated()) return -1;
	return mDepthImg.getColor(x, y).r;
}

float MeshController::getHeightForVal(float val) {
	float raw = ofMap(val, mMinImgDepthVal, mMaxImgDepthVal, 0, mMaxHeight, true);
	return ofClamp(raw, mPctWaterLevel * mMaxHeight, mMaxHeight);
}

ofFloatColor MeshController::getColorForHeight(float height) {
	float heightPct = ofMap(height, mPctWaterLevel * mMaxHeight, mMaxHeight, 0.0f, 1.0f);
	int pixel = ofClamp(mColorPalette.getWidth() * heightPct, 0, mColorPalette.getWidth() - 1);

	ofColor c = mColorPalette.getColor(pixel, 0);
	//cout << "Height: " << height << ", HeightPct: " << heightPct << ", Pixel " << pixel << ", Color: " << c << endl;

	ofFloatColor outColor = ofFloatColor(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, 1.0f);

	return outColor;	
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

ofVec2f MeshController::getImgCoord(ofVec3f pos) {

	// convert mesh space to img space
	// de-center
	ofVec2f img = pos + mMeshSize / 2.0f;
	// normalize to mesh size
	img /= mMeshSize;
	// scale to image size
	float imgW = mDepthImg.getWidth();
	float imgH = mDepthImg.getHeight();
	img *= ofVec2f(imgW, imgH);

	return ofVec2f( ofClamp(img.x, 0, imgW), ofClamp(img.y, 0, imgH));
}
