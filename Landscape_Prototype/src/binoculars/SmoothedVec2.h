#pragma once

#include "ofMain.h"

struct SmoothedVec2 {

	deque<ofVec2f> positions;
	ofVec2f pos = ofVec2f(0);

	void update(ofVec2f newPos, int num) {
		positions.push_back(newPos);
		while (positions.size() > num) {
			positions.pop_front();
		}
		pos.set(0);
		for (int i = 0; i < positions.size(); i++) {
			pos += positions[i];
		}
		pos /= positions.size();
	}
};

