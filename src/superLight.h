#pragma once

#include "ofMain.h"
#include "shadow.h"

#define LIGHT_TYPE_POINT 0
#define LIGHT_TYPE_SPOT 1
#define LIGHT_TYPE_DIRECTIONAL 2

class superLight {

public:
	superLight();
	~superLight();


	void setup(ofRectangle posBnds, ofRectangle lookAtBnds, ofVec2f speedPos, ofVec2f speedLookat, float zLight);//,ofVec2f freqPos,ofVec2f freqLookAt

	void update();
	void updatePos();
	void updateLookAt();

	void setPosition(ofPoint pos);
	void setZ(float posZ);
	//void setMaterial(int shine);
	void setLookAt(ofPoint pos);
	void setLightType(int lightType);
	void setColor(ofColor specular,ofColor diffuse);
	void begin();//bool useMaterial ofColor materialCol,
	void end();

	void drawShadowBack(float spread);
	void drawShadowTop(float spread,int levels);

	void drawDebug();

	shadow lightShadow;

	//bool shadowIsInitiated;
	bool useShadow;
	bool useShadowTop;
	
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
		
		//bool useMaterialB;
		//ofMaterial material;

		vector<ofPoint>debugTrail;


		ofVec2f uniqueStartPos;
		ofVec2f uniqueStartLookAt;

		float zLightPos;

};