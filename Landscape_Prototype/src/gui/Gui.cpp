#include "Gui.h"



Gui::Gui(){

}


Gui::~Gui(){

}

void Gui::setup(){


	guiName = "settings";
	guiFileName = guiName + ".xml";
	gui.setup(guiName, guiFileName);
	gui.add(binocularLabel.setup("   BINOCULAR SETTINGS", ""));
	gui.add(binScale.setup("Bin. Scale", 1.0, 0.1, 4.0));
	gui.add(binPosY.setup("Bin. Pos. Y", 0.57, 0.0, 1.0));
	gui.add(binDisplayYShift.setup("Bin. Display Y Shift", 0.57, -1.0, 1.0));
	gui.add(binocularAlphaLerpSpeed.setup("Bin. Lerp Speed", 0.02, 0.0, 0.2));
	gui.add(binZoomLerp.setup("Bin. Zoom Lerp Speed", 0.05, 0.0, 0.2));
	gui.add(binPosLerp.setup("Bin. Pos Lerp Speed", 0.05, 0.0, 0.2));
	gui.add(binMinMag.setup("Bin. Min Mag", 1.0, 0.0, 2.0));
	gui.add(binMaxMag.setup("Bin. Max Mag", 4.0, 0.0, 10.0));

	gui.add(binCenterBodyAngle.setup("Bin. Center Body Angle", 1.5, 0.0, 3.0));
	gui.add(binBodyAngleDev.setup("Bin. Body Angle Dev", 0.5, 0.0, 1.5));
	gui.add(binLeftBinX.setup("Bin. Left Pos X", 0.2, 0.0, 1.0));
	gui.add(binRightBinX.setup("Bin. Right Pos X", 0.8, 0.0, 1.0));

	//shader settings
	//gui.add(shaderLabel.setup("   FOG SHADER", ""));
	//gui.add(fogLayers.setup("Fog Layers", 3, 1, 10));
	//gui.add(driftSpeed.setup("Drift Speed", 0.1, 0.0, 1.0));
	//gui.add(noiseSpeed.setup("Noise Speed", 0.08, 0.00001, 0.5));
	//gui.add(noiseScale.setup("Noise Scale", 0.0008, 0.00001, 0.005));
	//gui.add(ySquish.setup("Squish Y", 5.0, 0.0, 20.0));
	//gui.add(octaveDiff.setup("Octave Diff", 0.1, 0.000001, 0.005));
	//gui.add(fogTrans.setup("Fog Trans", 0.5, 0.0, 1.0));
	//gui.add(fogMidpoint.setup("Fog Midpoint", 0.5, 0.0, 1.0));
	//gui.add(fogBoundSharpness.setup("Boundary Sharpness", 6.0, 0.0, 20.0));

	//cloth settings
	//gui.add(clothSettingsLabel.setup("   CLOTH SETTINGS", ""));
	//gui.add(constraintAccuracySlider.setup("Solving Iterations", 3, 1, 10));
	//gui.add(stiffnessSlider.setup("Link Stiffness", 1.0, 0.0, 1.2));
	//gui.add(restDistSlider.setup("Rest Distance", 10.0, 1.0, 100.0));
	//gui.add(mouseRadSlider.setup("Mouse Rad", 40, 0, 200));
	//gui.add(mouseStrengthSlider.setup("Mouse Strength", 1.0, 0.0, 5.0));
	//gui.add(dampingSlider.setup("Damping", 0.99, 0.85, 1.0));
	//gui.add(showStressColorsToggle.setup("Show Stresses", false));
	//gui.add(drawTexturedToggle.setup("Draw Textured", false));



	//Kinect vars
	gui.add(kinectLabel.setup("   KINECT", ""));
	gui.add(headHandDistThreshold.setup("Head to Hand Dist Thresh", 0.07, 0.0, 0.5));


	//formatting
	gui.setHeaderBackgroundColor(255);



	//color applies to gui title only
	gui.setDefaultTextColor(ofColor(255));

	//color of the labels
	binocularLabel.setDefaultTextColor(ofColor(0));
	binocularLabel.setBackgroundColor(255);

	//shaderLabel.setDefaultTextColor(ofColor(0));
	//shaderLabel.setBackgroundColor(255);

	//clothSettingsLabel.setDefaultTextColor(ofColor(0));
	//clothSettingsLabel.setBackgroundColor(255);

	//forcesLabel.setDefaultTextColor(ofColor(0));
	//forcesLabel.setBackgroundColor(255);

	kinectLabel.setDefaultTextColor(ofColor(0));
	kinectLabel.setBackgroundColor(255);


	lastSave = -10.0f;
	lastLoad = -10.0f;

	loadSettings();



}


void Gui::draw(int x, int y){

	gui.setPosition(x, y);
	gui.draw();

	if (ofGetElapsedTimef() - lastSave < 1.0f) {
		ofSetColor(0, 255, 0);
		ofDrawBitmapString("Settings Saved", ofGetWidth() - 150, 20 );
	}
	if (ofGetElapsedTimef() - lastLoad < 1.0f) {
		ofSetColor(0, 128, 255);
		ofDrawBitmapString("Settings Loaded", ofGetWidth() - 150, 20);
	}
}

void Gui::saveSettings() {
	gui.saveToFile(guiFileName);
	lastSave = ofGetElapsedTimef();
}

void Gui::loadSettings() {
	gui.loadFromFile(guiFileName);
	lastLoad = ofGetElapsedTimef();
}