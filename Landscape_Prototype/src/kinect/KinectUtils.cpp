#include "KinectUtils.h"

//--------------------------------------------------------------
void KinectUtils::drawColorAndBody(ofxKFW2::Device* kinect){

	int previewWidth = 640;
	int previewHeight = 480;

	// Color is at 1920x1080 instead of 512x424 so we should fix aspect ratio
	float colorHeight = previewWidth * (kinect->getColorSource()->getHeight() / kinect->getColorSource()->getWidth());
	float colorTop = (previewHeight - colorHeight) / 2.0;

	ofPushMatrix();
	ofTranslate(250, 0);
	ofScale(2.0, 2.0);

	ofSetColor(255);
	kinect->getColorSource()->draw(0, 0 + colorTop, previewWidth, colorHeight);
	kinect->getBodySource()->drawProjected(0, 0 + colorTop, previewWidth, colorHeight);

	//draw stuff
	//Getting joint positions (skeleton tracking)
	ofVec2f head, rightHand, rightShoulder, leftHand, leftShoulder;
	bool handsClosed = false;
	
	//auto bodies = kinect->getBodySource()->getBodies();
	//for (auto body : bodies) {
	//	for (auto joint : body.joints) {

	//		//cout << "First: " << joint.first << ", Second: " << joint.second.getPosition() << endl;
	//		if (joint.first == JointType_Head) {
	//			head = joint.second.getPositionInDepthMap();
	//		}
	//		else if (joint.first == JointType_HandLeft) {
	//			leftHand = joint.second.getPositionInDepthMap();
	//		}
	//		else if (joint.first == JointType_HandRight) {
	//			rightHand = joint.second.getPositionInDepthMap();
	//		}
	//		else if (joint.first == JointType_ShoulderLeft) {
	//			leftShoulder = joint.second.getPositionInDepthMap();
	//		}
	//		else if (joint.first == JointType_ShoulderRight) {
	//			rightShoulder = joint.second.getPositionInDepthMap();
	//		}
	//	}

	//	if (body.leftHandState == HandState_Closed && body.rightHandState == HandState_Closed) handsClosed = true;

	//}

	// Draw IR Image + skeleton
	//kinect->getInfraredSource()->draw(previewWidth, 0);// , previewWidth, previewHeight);
	//kinect->getBodySource()->drawProjected(previewWidth, 0, 512, 424, ofxKFW2::ProjectionCoordinates::DepthCamera);

	//ofPushMatrix();
	//ofTranslate(640, 0);
	//ofSetColor(255, 0, 255);
	//ofDrawCircle(head.x, head.y, 20, 20);
	//ofDrawLine(leftShoulder.x, leftShoulder.y, rightShoulder.x, rightShoulder.y);

	//if (handsClosed) {
	//	ofSetLineWidth(5);
	//	ofDrawLine(leftHand.x, leftHand.y, rightHand.x, rightHand.y);
	//}

	//ofPopMatrix();

	ofPopMatrix();


}