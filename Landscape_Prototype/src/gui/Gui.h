#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxEasing.h"

class Gui{
public:
	Gui();
	~Gui();

	void setup();
	void draw(int x, int y);

	void saveSettings();
	void loadSettings();

	float lastSave, lastLoad;

	string guiName;
	string guiFileName;

	ofxPanel gui;
	ofxLabel binocularLabel;
	ofxFloatSlider binocularAlphaLerpSpeed;
	ofxFloatSlider binZoomLerp;
	ofxFloatSlider binPosLerp;

	ofxFloatSlider binMinMag;
	ofxFloatSlider binMaxMag;

	ofxFloatSlider binCenterBodyAngle;
	ofxFloatSlider binBodyAngleDev;
	ofxFloatSlider binLeftBinX;
	ofxFloatSlider binRightBinX;

	ofxFloatSlider binScale;
	ofxFloatSlider binPosY;
	ofxFloatSlider binDisplayYShift;

	//ofxLabel shaderLabel;
	//ofxIntSlider fogLayers;
	//ofxFloatSlider driftSpeed;
	//ofxFloatSlider noiseSpeed;
	//ofxFloatSlider noiseScale;
	//ofxFloatSlider ySquish;
	//ofxFloatSlider octaveDiff;
	//ofxFloatSlider fogTrans;
	//ofxFloatSlider fogMidpoint;
	//ofxFloatSlider fogBoundSharpness;

	//ofxLabel clothSettingsLabel;
	//ofxFloatSlider restDistSlider;
	//ofxIntSlider constraintAccuracySlider;
	//ofxFloatSlider stiffnessSlider;
	//ofxFloatSlider mouseStrengthSlider;
	//ofxFloatSlider mouseRadSlider;
	//ofxFloatSlider mouseTearRadSlider;
	//ofxFloatSlider tearDistanceSlider;
	//ofxFloatSlider dampingSlider;
	//ofxToggle showStressColorsToggle;
	//ofxToggle drawTexturedToggle;


	//ofxLabel forcesLabel;
	//ofxToggle useGravity;
	//ofxFloatSlider gravitySlider;
	//ofxToggle useWind;
	//ofxFloatSlider windStrengthSlider;
	//ofxFloatSlider windVariationSlider;
	//ofxFloatSlider windVariationSpeed;
	//ofxFloatSlider windScaleSlider;

	ofxLabel kinectLabel;
	ofxFloatSlider headHandDistThreshold;




};

