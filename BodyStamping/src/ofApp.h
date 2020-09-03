#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ofxAutoReloadedShader.h"
#include "ofxGui.h"
#include "kinect/KinectUtils.h"
#include "gui/SharedGui.h"
#include "gui/FontManager.h"
#include "tracking/CVProcessor.h"

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
	
		ofxKFW2::Device mKinect;

		const int depthW = 512;
		const int depthH = 424;

		CVProcessor mCV;

		bool bShowGui;

		bool bIsFullscreen;
		void setFakeFullscreen();

		bool bDrawKinect = false;
		

};
