#include "Axes.h"



Axes::Axes() {

	float rad = 5;
	float height = 30;

	//set up the heads of the axis cones
	coneX.setRadius(rad);
	coneX.setHeight(height);
	coneX.setResolution(8, 1, 0);
	coneX.setTopColor(ofColor(255, 0, 0));

	coneY.setRadius(rad);
	coneY.setHeight(height);
	coneY.setResolution(8, 1, 0);
	coneY.setTopColor(ofColor(0, 255, 0));

	coneZ.setRadius(rad);
	coneZ.setHeight(height);
	coneZ.setResolution(8, 1, 0);
	coneZ.setTopColor(ofColor(0, 0, 255));

}

void Axes::drawAxes() {

	ofVec3f axisLengths = ofVec3f(50);

	ofPushMatrix();
	ofTranslate(0, 0, 300);
	

	ofSetLineWidth(2);

	//X-Axis (Red)
	ofSetColor(255, 0, 0);
	ofDrawLine(0, 0, 0, axisLengths.x, 0, 0);


	ofPushMatrix();
	ofTranslate(axisLengths.x, 0);
	ofRotate(90, 0, 0, 1);

	coneX.draw();

	ofPopMatrix();

	//Y-Axis (Green)
	ofSetColor(0, 255, 0);
	ofDrawLine(0, 0, 0, 0, axisLengths.y, 0);


	ofPushMatrix();
	ofTranslate(0, axisLengths.y);
	ofRotate(180, 0, 0, 1);

	coneY.draw();

	ofPopMatrix();

	//Z-Axis (Blue)
	ofSetColor(0, 128, 255);
	ofDrawLine(0, 0, 0, 0, 0, axisLengths.z);


	ofPushMatrix();
	ofTranslate(0, 0, axisLengths.z);
	ofRotate(-90, 1, 0, 0);

	coneZ.draw();

	ofPopMatrix();

	ofPopMatrix();

}
