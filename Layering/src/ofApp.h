#pragma once

#include "ofMain.h"
#include "visuals\Axes.h"
#include "gui/SharedGui.h"
#include "gui/FontManager.h"
#include "visuals/MeshController.h"

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
	ofLight light;
	ofMaterial material;

	bool bShowGui = true;


	float getBillboardAngle(ofVec3f globalPos);
	ofVec3f getBillboardVec(ofVec3f globalPos);

};
