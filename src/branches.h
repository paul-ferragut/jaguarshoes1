#pragma once
#include "particleFlower.h"


#define STATE_UPDATE 0
#define STATE_ENDED 1
#define STATE_INACTIVE 2

struct branch {

	vector<particleFlower>p;
	int currentLevel;
	int maxBranches;
	ofVec2f directionPt;

};
class branches{

	public:
		branches();
		~branches();
   


        void setup(ofVec2f pos,float w);
		void update();
		void draw(ofColor colStart,ofColor colEnd);		

		vector<branch>b;
		int wP;
		int hP;

		ofVec2f position; 
		float width;
		
};