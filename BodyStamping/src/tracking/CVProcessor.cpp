#include "CVProcessor.h"



CVProcessor::CVProcessor() {

}

void CVProcessor::setup(int width, int height) {

	mSize.x = width;
	mSize.y = height;


	SharedGui::get()->getGroup("VISION")->add(mBlurAmt.set("Blur Amount", 0, 0, 30));
	SharedGui::get()->getGroup("VISION")->add(mNumErosions.set("Num Erosions", 0, 0, 20));
	SharedGui::get()->getGroup("VISION")->add(mNumDilations.set("Num Dilations", 0, 0, 20));
	SharedGui::get()->getGroup("VISION")->add(mThreshold.set("Threshold", 0, 0, 255));
	SharedGui::get()->getGroup("VISION")->add(mMinBlobArea.set("Min Blob Area", 100, 0, 1000));
	SharedGui::get()->getGroup("VISION")->add(mMaxBlobArea.set("Max Blob Area", 60000, 0, 200000));
	SharedGui::get()->getGroup("VISION")->add(mPersistence.set("Blob Persistence", 20, 0, 100));
	SharedGui::get()->getGroup("VISION")->add(mMaxBlobDist.set("Max Blob Travel Dist", 50, 0, 200));


	SharedGui::get()->getGroup("VISION")->add(mNearClip.set("Near Clip", 0, 0, MAX_DEPTH));
	SharedGui::get()->getGroup("VISION")->add(mFarClip.set("Far Clip", MAX_DEPTH, 0, MAX_DEPTH));

	mRawImg.allocate(mSize.x, mSize.y, OF_IMAGE_GRAYSCALE);
	mRawImg.setColor(ofColor(0));

	mBlurredImg.allocate(mSize.x, mSize.y, OF_IMAGE_GRAYSCALE);
	mBlurredImg.setColor(ofColor(0));

	mThreshImg.allocate(mSize.x, mSize.y, OF_IMAGE_GRAYSCALE);
	mThreshImg.setColor(ofColor(0));

	mForegroundImg.allocate(mSize.x, mSize.y, OF_IMAGE_GRAYSCALE);
	mForegroundImg.setColor(ofColor(0));

	mBackgroundImg.allocate(mSize.x, mSize.y, OF_IMAGE_GRAYSCALE);
	mBackgroundImg.setColor(ofColor(0));

	updateDepthLookupTable();
}

void CVProcessor::update() {

	if (mPrevFarClip != mFarClip || mNearFarClip != mNearClip) {
		ofLog(OF_LOG_NOTICE, "UPDATING DEPTH LUT");
		updateDepthLookupTable();
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

void CVProcessor::process(ofShortPixels &shortPix) {
	if (!shortPix.isAllocated()) return;

	for (int y = 0; y < shortPix.getHeight(); y++) {
		for (int x = 0; x < shortPix.getWidth(); x++) {
			int index = (y * shortPix.getWidth()) + x;
			float depth = shortPix[index];
			float val = depthLookupTable[depth];
			mRawImg.setColor(x, y, ofColor(val));
		}
	}

	mRawImg.update();

	performCV(mRawImg.getPixels());

}

void CVProcessor::process(ofPixels &pix) {
	
	mRawImg.setFromPixels(pix);
	mRawImg.update();

	performCV(pix);

}

void CVProcessor::performCV(ofPixels &pix) {

	//DILATE it
	for (int d = 0; d < mNumDilations; d++) {
		ofxCv::dilate(pix);
	}

	ofxCv::GaussianBlur(pix, mBlurredImg.getPixels(), mBlurAmt);

	mBlurredImg.update();


	//if we're not doing BG differencing, then put the blurred
	//pixels into the foreground
	mForegroundImg = mBlurredImg;

	//threshold it
	ofxCv::threshold(mBlurredImg.getPixels(), mThreshImg.getPixels(), mThreshold);



	//ERODE it
	for (int e = 0; e < mNumErosions; e++) {
		ofxCv::erode(mThreshImg.getPixels());
	}

	mThreshImg.update();




	mContours.setMinArea(mMinBlobArea);
	mContours.setMaxArea(mMaxBlobArea);
	mContours.setThreshold(254);  //only detect white

	// wait before forgetting something
	mContours.getTracker().setPersistence(mPersistence);

	// an object can move up to ___ pixels per frame
	mContours.getTracker().setMaximumDistance(mMaxBlobDist);

	//find dem blobs
	mContours.findContours(mThreshImg.getPixels());
	//ofLog() << "Num Contours: " << mContours.getContours().size();


	//go through the contours
	//for (int i = 0; i < mContours.getContours().size(); i++) {

	//	int id = mContours.getLabel(i);
	//	
	//	//check if we already have this blob
	//	int foundIndex = -1;

	//	for (int p = 0; p < mPeopleList.size(); p++) {
	//		if (mPeopleList[p].ID == id) {
	//			foundIndex = p;
	//			break;
	//		}
	//	}

	//	ofVec2f blobPos = ofxCv::toOf(mContours.getCentroid(i));
	//	ofRectangle blobRect = ofxCv::toOf(mContours.getBoundingRect(i));
	//	
	//	//use the centroid's X value, but the 
	//	//top bound of the blob for the Y
	//	ofVec2f raw = blobPos;
	//	if(mUseBlobFrontEdge) raw.y = blobPos.y - blobRect.getHeight() / 2.0f;

	//	//get the normalized space coordinates
	//	ofVec2f norm = getNormCoords(raw.x, raw.y, mControlPoints);

	//	if (foundIndex == -1) {

	//		//new blob, add it
	//		mPeopleList.emplace_back( Person() );
	//		auto &person = mPeopleList.back();

	//		person.ID = id;
	//		person.birthTime = ofGetElapsedTimef();
	//		person.rawPos = raw;
	//		person.setNewNormPosition(norm);
	//		//person.vel = ofxCv::toOf(mContours.getVelocity(i));

	//	} else {

	//		//old blob update it
	//		auto & person = mPeopleList[foundIndex];
	//		person.ID = id;
	//		person.rawPos = raw;

	//		if (person.age > mMinAgeToSend) person.bShouldSend = true;
	//		
	//		// given normalized position, blob will 
	//		// smooth it then calculate velocity 
	//		person.setNewNormPosition(norm);
	//		//person.vel = (person.vel + ofxCv::toOf(mContours.getVelocity(i))) / 2.0f;
	//		

	//	}
	//}




}

void CVProcessor::drawContours() {
	ofSetColor(255, 0, 0);
	mContours.draw();

	ofFill();
	for (int i = 0; i < mContours.getContours().size(); i++) {
		string s = "ID: " + ofToString(mContours.getLabel(i));
		ofDrawCircle(ofxCv::toOf(mContours.getCentroid(i)), 5);
		ofDrawBitmapString(s, ofxCv::toOf(mContours.getCentroid(i)) + ofVec2f(5, 0));
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

void CVProcessor::drawBlurred(int x, int y, int w, int h) {

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
	
void CVProcessor::drawBackground(int x, int y, int w, int h) {

	float scaleX = w / mSize.x;
	float scaleY = h / mSize.y;

	ofPushMatrix();
	ofTranslate(x, y);
	ofScale(scaleX, scaleY);

	ofPushStyle();
	ofNoFill();
	ofSetColor(255);
	mBackgroundImg.draw(0, 0);
	ofSetLineWidth(1);
	ofDrawRectangle(0, 0, mSize.x, mSize.y);
	float titleSize = 10;
	FontManager::get()->drawString("Background Image", 0, -5, titleSize, "futura_light");

	ofPopStyle();

	ofPopMatrix();


}

void CVProcessor::drawForeground(int x, int y, int w, int h) {

	float scaleX = w / mSize.x;
	float scaleY = h / mSize.y;

	ofPushMatrix();
	ofTranslate(x, y);
	ofScale(scaleX, scaleY);

	ofPushStyle();
	ofNoFill();
	ofSetColor(255);
	mForegroundImg.draw(0, 0);

	//draw threshold over it transparent
	ofSetColor(255, 0, 0, 150);
	mThreshImg.draw(0, 0);
	ofSetLineWidth(1);
	ofDrawRectangle(0, 0, mSize.x, mSize.y);

	drawContours();

	ofPopStyle();


	ofSetColor(255);
	float titleSize = 10;
	FontManager::get()->drawString("Foreground Image + Threshold + Contours", 0, -5, titleSize, "futura_light");

	ofPopMatrix();

}
