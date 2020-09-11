#pragma once

#include "ofMain.h"


//method for getting normalized coordiantes from an
//irregular quadrilateral from:
//https://www.particleincell.com/2012/quad-interpolation/
static ofVec2f getNormCoords(float x, float y, const vector<ofVec2f> &points) {

	//we need a column matrix (i.e. a vec4)
	//of the x and y components separately
	ofVec4f xPts(points[0].x, points[1].x, points[2].x, points[3].x);
	ofVec4f yPts(points[0].y, points[1].y, points[2].y, points[3].y);

	//Matrices we'll use to find the
	//coefficients of the mapping function
	ofMatrix4x4 M;
	M.set(1, 0, 0, 0,
		1, 1, 0, 0,
		1, 1, 1, 1,
		1, 0, 1, 0);

	//column matrix of each of the x and y coefficients
	ofVec4f a = M.getInverse() * xPts;
	ofVec4f b = M.getInverse() * yPts;

	//quadratic equation coeffs, aa*mm^2+bb*m+cc=0
	float aa = a[3] * b[2] - a[2] * b[3];
	float bb = a[3] * b[0] - a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + x * b[3] - y * a[3];
	float cc = a[1] * b[0] - a[0] * b[1] + x * b[1] - y * a[1];

	//compute m (y value) = (-b+sqrt(b^2-4ac))/(2a)
	float det = sqrt(bb*bb - 4 * aa*cc);
	float m = (-bb + det) / (2 * aa);

	//compute l (x value)
	float l = (x - a[0] - a[2] * m) / (a[1] + a[3] * m);


	ofVec2f norm(l, m);

	return norm;

}

//does a bilinear interpolation between 4 points
//and returns the point mapped to the quad
static ofVec2f normCoordsToQuad(float x, float y, const vector<ofVec2f> &quad) {

	/*
			  x
		0     :     1
		 |    :    |
		 |____:____|....y
		 |         |
		 |         |
		3           2

	 */

	 //First find the coords of the left and right end points
	ofVec2f mid0to3, mid1to2;

	mid0to3 = quad[0].getInterpolated(quad[3], y);
	mid1to2 = quad[1].getInterpolated(quad[2], y);

	ofVec2f mid = mid0to3.getInterpolated(mid1to2, x);

	return mid;

}


//returns angle in radians between start and finish, if not set to full rotation,
//negative value = counterclockwise, otherwise, values are out of 
static double getAngleBetween(ofVec2f start, ofVec2f end, bool fullRotation = false) {

	start.normalize();
	end.normalize();
	double angle = acos(end.dot(start));

	//to get the direction, use cross product to find what side it's on
	ofVec3f crossProd = ofVec3f(start.x, start.y, 0.0f).getCrossed(ofVec3f(end.x, end.y, 0.0f));

	if (crossProd.z < 0.0f) {
		if (fullRotation) {
			//convert angle to 0 to TWP_PI going clockwise
			angle = PI + (PI - angle);
		} else {
			//ranges from 0 to PI with negative indicating counter clockwise
			angle *= -1.0f;
		}
	}

	return angle;

}

//NOTE: method tests for the intersection of two lines defined by the incoming points.
//Hence, it may return a point that is not on the defined line segments, but on an extension of one of them
static ofVec2f getIntersectionPoint(ofVec2f line1Start, ofVec2f line1End, ofVec2f line2Start, ofVec2f line2End, bool &intersectionExists) {

	ofVec2f intersectionPt;

	//find equation of line 1
	float m1, b1, m2, b2;

	//Must check if the lines are both horizontal or both vertical first

	//-1 = vertical, 1 = horizontal, 0 = something else
	int firstLineState = 0;
	int secondLineState = 0;

	if (line1Start.x - line1End.x == 0.0) {
		firstLineState = -1;
	} else if (line1Start.y - line1End.y == 0.0) {
		firstLineState = 1;
	}

	if (line2Start.x - line2End.x == 0.0) {
		secondLineState = -1;
	} else if (line2Start.y - line2End.y == 0.0) {
		secondLineState = 1;
	}

	//either both are horiz or both are vert, return an error
	if (firstLineState == secondLineState && firstLineState != 0 && firstLineState != 0) {
		intersectionExists = false;
		return ofVec2f(-1);
	}


	//Vertical lines break the algorithm so handle those first

	//first line is vert
	if (firstLineState == -1) {

		//we know the x right away since the line segment is vertical (all has the same x)
		intersectionPt.x = line1Start.x;

		//find slope 2
		m2 = (line2Start.y - line2End.y) / (line2Start.x - line2End.x);
		//find yIntercept 2
		b2 = line2Start.y - m2 * line2Start.x;

		intersectionPt.y = m2 * intersectionPt.x + b2;

	} else if (secondLineState == -1) {   //second line is vert

		//we know the x right away since the line segment is vertical (all has the same x)
		intersectionPt.x = line2Start.x;

		//find slope 1
		m1 = (line1Start.y - line1End.y) / (line1Start.x - line1End.x);
		//find yIntercept 1
		b1 = line1Start.y - m1 * line1Start.x;

		intersectionPt.y = m1 * intersectionPt.x + b1;


	} else {        //normal scenario

		//slope (y diff)/(x diff)
		m1 = (line1Start.y - line1End.y) / (line1Start.x - line1End.x);
		m2 = (line2Start.y - line2End.y) / (line2Start.x - line2End.x);

		//if the slope is the same, lines are parallel
		if (m1 == m2) {
			intersectionExists = false;
			return ofVec2f(-1);
		}


		//Y = mX + b   ==> b = Y - mX
		b1 = line1Start.y - m1 * line1Start.x;
		b2 = line2Start.y - m2 * line2Start.x;

		//X intersection point
		intersectionPt.x = (b2 - b1) / (m1 - m2);

		//sub X back into one of y = mx + b to get Y intersect
		intersectionPt.y = m1 * intersectionPt.x + b1;

	}

	intersectionExists = true;
	return intersectionPt;

}


//returns a position on the edge of the rect
//in the given direction position specified within the rect
// ----- NOTE: ASSUMES RECT IS CENTERED AT ORIGIN
// ----- NOTE: DOES NOT CHECK THAT POINT IS WITHIN RECT
static ofVec2f getRectIntersection(ofVec2f dir, ofVec2f pos, ofVec2f rectSize) {

	float w = rectSize.x;
	float h = rectSize.y;

	dir.normalize();

	// get vectors from declared point to vertices
	ofVec2f vert0 = ofVec2f(w / 2, -h / 2) - pos;
	ofVec2f vert1 = ofVec2f(w / 2, h / 2) - pos;
	ofVec2f vert2 = ofVec2f(-w / 2, h / 2) - pos;
	ofVec2f vert3 = ofVec2f(-w / 2, -h / 2) - pos;

	//zeroth vertex is 0.0f, the starting point
	float vert1Angle = getAngleBetween(vert0, vert1, true); //angle from first vert to second
	float vert2Angle = getAngleBetween(vert0, vert2, true); //angle from first vert to third
	float vert3Angle = getAngleBetween(vert0, vert3, true); //angle from first vert to fourth

	float angle = getAngleBetween(vert0, dir, true);

	ofVec2f toEdge = ofVec2f(-1);
	bool hasIntersection = false;

	if (angle <= vert1Angle) {
		//right segment
		toEdge = getIntersectionPoint(ofVec2f(0), dir, vert0, vert1, hasIntersection);
	} else if (angle <= vert2Angle) {
		//bottom segment
		toEdge = getIntersectionPoint(ofVec2f(0), dir, vert1, vert2, hasIntersection);
	} else if (angle <= vert3Angle) {
		//left segment
		toEdge = getIntersectionPoint(ofVec2f(0), dir, vert2, vert3, hasIntersection);
	} else {
		//top segment 
		toEdge = getIntersectionPoint(ofVec2f(0), dir, vert3, vert0, hasIntersection);
	}

	//Shift the position so it isn't centered around the origin
	//toEdge += ofVec2f(w / 2, h / 2);

	return toEdge;

}


// return value will be positive if test point is on same side of plane as normal points
// negative if on opposite side, zero if on the plane.
// plane defined by normal and position of normal (must be normalized)
static float checkWhichSideOfPlane(ofVec3f pt, ofVec3f planeNomral, ofVec3f planePos) {
	// first get vector from plane to testPt
	ofVec3f planeToPt = pt - planePos;
	return planeToPt.dot(planeNomral);
}

static ofVec3f ofClampVec(ofVec3f & val, ofVec3f min, ofVec3f max) {
	return ofVec3f(	ofClamp(val.x, min.x, max.x),
					ofClamp(val.y, min.y, max.y),
					ofClamp(val.z, min.z, max.z));
}

static ofVec2f ofClampVec(ofVec2f & val, ofVec2f min, ofVec2f max) {
	return ofVec2f(	ofClamp(val.x, min.x, max.x),
					ofClamp(val.y, min.y, max.y) );
}