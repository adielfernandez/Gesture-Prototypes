#include "KinectUtils.h"

//--------------------------------------------------------------
void KinectUtils::drawDebugView(ofxKFW2::Device* kinect, int x, int y){

	int previewWidth = 640;
	int previewHeight = 480;

	// Color is at 1920x1080 instead of 512x424 so we should fix aspect ratio
	float colorHeight = previewWidth * (kinect->getColorSource()->getHeight() / kinect->getColorSource()->getWidth());
	float colorTop = (previewHeight - colorHeight) / 2.0;

	ofPushStyle();

	ofPushMatrix();
	ofTranslate(0, 0);
	//ofScale(1.5, 1.5);

	ofSetColor(255);
	kinect->getColorSource()->draw(x, y + colorTop, previewWidth, colorHeight);
	kinect->getBodySource()->drawProjected(x, y + colorTop, previewWidth, colorHeight);

	//draw stuff
	//Getting joint positions (skeleton tracking)
	ofVec2f head, rightHand, rightShoulder, leftHand, leftShoulder, leftElbow, rightElbow;
	bool handsClosed = false;
	
	auto bodies = kinect->getBodySource()->getBodies();
	for (auto body : bodies) {
		for (auto joint : body.joints) {

			//cout << "First: " << joint.first << ", Second: " << joint.second.getPosition() << endl;
			if (joint.first == JointType_Head) {
				head = joint.second.getPositionInDepthMap();
			}
			else if (joint.first == JointType_HandLeft) {
				leftHand = joint.second.getPositionInDepthMap();
			}
			else if (joint.first == JointType_HandRight) {
				rightHand = joint.second.getPositionInDepthMap();
			}
			else if (joint.first == JointType_ShoulderLeft) {
				leftShoulder = joint.second.getPositionInDepthMap();
			}
			else if (joint.first == JointType_ShoulderRight) {
				rightShoulder = joint.second.getPositionInDepthMap();
			}
			else if (joint.first == JointType_ElbowLeft) {
				leftElbow = joint.second.getPositionInDepthMap();
			}
			else if (joint.first == JointType_ElbowRight) {
				rightElbow = joint.second.getPositionInDepthMap();
			}
		}

		if (body.leftHandState == HandState_Closed && body.rightHandState == HandState_Closed) handsClosed = true;

	}

	// Draw IR Image + skeleton
	kinect->getInfraredSource()->draw(x + previewWidth, y);// , previewWidth, previewHeight);
	kinect->getBodySource()->drawProjected(x + previewWidth, y, 512, 424, ofxKFW2::ProjectionCoordinates::DepthCamera);
	
	// box outlines
	ofPushStyle();
	ofSetColor(255, 150);
	ofNoFill();
	ofSetLineWidth(1);
	ofDrawRectangle(x, y + colorTop, previewWidth, colorHeight);
	ofDrawRectangle(x + previewWidth, y, 512, 424);
	ofPopStyle();

	//ofSetColor(255, 0, 255);
	//ofDrawCircle(leftHand.x, leftHand.y, 20, 20);
	//ofDrawCircle(leftElbow.x, leftElbow.y, 20, 20);
	//ofSetColor(0, 255, 255);
	//ofDrawCircle(rightHand.x, rightHand.y, 20, 20);
	//ofDrawCircle(rightElbow.x, rightElbow.y, 20, 20);

	//if (handsClosed) {
	//	ofSetLineWidth(5);
	//	ofSetColor(0);
	//	ofDrawLine(leftHand.x, leftHand.y, rightHand.x, rightHand.y);
	//}


	ofPopMatrix();
	ofPopStyle();

}