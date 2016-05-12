#pragma once
#include "ofMain.h"

#include "ofxVoronoi.h"
#include "ofxSvg.h"
#include "particleVoronoi.h"
//#include "ofxBeat.h"

class voronoi {

	public:
		voronoi();
		~voronoi();

		void setup();
		void update(float noise);
		void draw();	


		ofRectangle      bounds;
		vector <ofPoint> pts;
		ofxVoronoi       voro;

		ofxSVG svg;

		//void resetParticles();
		vector <particleVoronoi> p;
		vector <ofPoint> attractPoints;
		vector <ofPoint> attractPointsWithMovement;

		//ofxBeat beat;
		//void audioReceived(float*, int, int);
	//	ofImage over;

		ofPoint soundPoint;

		ofRectangle positionBounds;
		ofVec2f uniqueStartPos;
		ofVec2f speedPosition;
	
};