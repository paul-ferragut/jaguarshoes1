#pragma once
#include "ofMain.h"

#define STATE_UPDATE 0
#define STATE_ENDED 1
#define STATE_INACTIVE 2
#define STATE_DECONSTRUCT 3
#define STATE_READY 4

class particleFlower{

	public:
		particleFlower();
		~particleFlower();
   


        void setup(ofPoint origin, ofVec2f direction, int subDivision);
		
        ofVec2f getPos();
        int update();
		void draw(ofColor colStart,ofColor colEnd);		
		
		ofPoint pos;
		ofPoint vel;
		ofPoint frc;

		ofPoint dir;
    
      //  ofPoint endPoint;
    
		int duration;
    int durationEnd;
	int breakEnd;
	int state;
	//int destructionEnd;
    
	bool readyDeconstruct;
    
   // float attraction;
    
    
    vector<ofVec2f>trail;
    
		float drag; 
		float uniqueVal;
		float scale;
		
};