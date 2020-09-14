#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	//ofSetupOpenGL(1920,1080,OF_WINDOW);			// <-------- setup the GL context


	ofGLFWWindowSettings settings;
	settings.width = 1924;
	settings.height = 1084;

	if (true) {
		// FAKE FULLSCREEN
		settings.setPosition(ofVec2f(-1, -1));
		settings.decorated = false;
	} else {
		// WINDOW
		settings.setPosition(ofVec2f(50, 50));
		settings.windowMode = OF_WINDOW;
		settings.decorated = true;
	}

	settings.setGLVersion(2, 1); /// < select your GL Version here
	ofCreateWindow(settings);


	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
