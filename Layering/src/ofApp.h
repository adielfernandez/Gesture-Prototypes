#pragma once

#include "ofMain.h"
#include "visuals\Axes.h"
#include "gui/SharedGui.h"
#include "gui/FontManager.h"
#include "visuals/MeshController.h"
#include "ofxKinectForWindows2.h"
#include "kinect/KinectUtils.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
	Axes mAxes;

	MeshController mMeshController;

    ofEasyCam cam;
	bool bSetupCamera = false;
	ofVec3f mGlobalCamPos;
	ofVec3f mCamStartPos;
	void resetCamera();

	//ofLight light;
	//ofMaterial material;

	bool bShowGui = true;


	float getBillboardAngle(ofVec3f globalPos);
	ofVec3f getBillboardVec(ofVec3f globalPos);

	// reveal plane
	ofVec3f mPlaneNormal;
	ofVec3f mPlaneRotNorm;
	ofVec3f mPlanePos;


	bool bTwoHandGestureActive = false;
	bool bOneHandGestureActive = false;
	bool bUseLeft = false;

	
	// Vector from left to right hand, flattened into XZ plane (in camera space)
	ofVec3f mInitialHandVec;  
	ofVec3f mInitialHandCenter;
	ofVec3f mInitialPlanePos;
	ofParameter<float>	mPlaneMovementScale;

	ofVec3f mInitialOneHandPos;
	ofVec3f mInitialCamPos;
	ofParameter<float>	mCameraPanMult;


	ofxKFW2::Device mKinect;
	bool bDrawKinect = false;

};
