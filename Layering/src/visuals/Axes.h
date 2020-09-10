#pragma once

#include "ofMain.h"


class Axes {

public:

	Axes();
	~Axes() {};

	void drawAxes();

private:



	ofConePrimitive coneX;
	ofConePrimitive coneY;
	ofConePrimitive coneZ;


};