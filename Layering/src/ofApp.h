#pragma once

#include "ofMain.h"

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
		
    //Our mesh objects
    ofMesh mesh;
    ofMesh originalMesh;
    
        
    bool bWire;
    
    ofEasyCam cam;
    ofImage img;

	float getValFromImg(int x, int y);
	float getHeightForVal(float val);

	//XYZ Axis Arrows
	void drawAxes();
	float getBillboardAngle(ofVec3f globalPos);
	ofVec3f getBillboardVec(ofVec3f globalPos);
	ofConePrimitive coneX;
	ofConePrimitive coneY;
	ofConePrimitive coneZ;
};
