#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ofxAutoReloadedShader.h"
#include "ofxGui.h"
#include "kinect/KinectUtils.h"
#include "gui/Gui.h"

#include "binoculars\BinocularLayer.h"



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


		ofImage mBackground;
	
		BinocularLayer mBinoculars;

		ofxKFW2::Device mKinect;

		shared_ptr<Gui> mGui;
		bool bShowGui;


		bool bIsFullscreen;
		void setFakeFullscreen();

		bool bDrawKinect = false;

		float getAngleBetween(ofVec3f a, ofVec3f b);

};
