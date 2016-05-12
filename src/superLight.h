#pragma once

#include "ofMain.h"


class superLight{
		
	public:
	superLight();
		~superLight();
	
		
		void setup(ofRectangle posBnds,ofRectangle lookAtBnds, ofVec2f speedPos, ofVec2f speedLookat, float zLight);//,ofVec2f freqPos,ofVec2f freqLookAt
		
		void update();
		void updatePos();
		void updateLookAt();

		void setPosition(ofPoint pos);
		void setLookAt(ofPoint pos);

		void begin(bool useMaterial);
		void end();

		void drawDebug();

		ofVec2f getShadowOffset();
		ofVec2f getShadowOffsetMapped(float lenghtRatio);

	private:
		ofRectangle positionBounds;
		ofRectangle lookAtBounds;

		ofVec2f speedPosition;
		ofVec2f speedLookAt;
		ofVec2f frequencyPosition;
		ofVec2f frequencyLookAt;

		ofPoint lookAt;
		ofPoint position;
		ofPoint rotation;
		ofLight light;
		
		bool useMaterialB;
		ofMaterial material;

		vector<ofPoint>debugTrail;


		ofVec2f uniqueStartPos;
		ofVec2f uniqueStartLookAt;

		float zLightPos;

};