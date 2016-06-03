#pragma once

#include "ofMain.h"
#include "ofxSvg.h"

struct glowPath {
	ofPath path;
	ofVec2f pos;
	ofVec2f centroid;
	ofVec2f offset;
	ofColor color;
	float size;
	vector<int>iterationOpacity;
};

class glow{

	public:

		glow();
		~glow();
		void setup(string svgString);
		void update(ofVec2f position);
		void draw(bool pulse, ofColor col);

		//ofxSVG svg;
		vector<glowPath>path;
		
};
