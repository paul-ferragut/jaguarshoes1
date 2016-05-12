#pragma once
#include "ofMain.h"


class particleVoronoi {

	public:
		particleVoronoi();
		~particleVoronoi();
		
		void setAttractPoints( vector <ofPoint> * attract );

		void reset();
		void update();
		void draw();		
		
		ofPoint pos;
        ofPoint originPos;
		ofPoint vel;
		ofPoint frc;


		ofPoint soundPt;
		bool isDouble;
		int doubleIndex;
    
        bool bassTap;
		
		float drag;
		float uniqueVal;
		float scale;

		float cellSize;
    
    float bassCounter;
		
		
		
		vector <ofPoint> * attractPoints; 
};