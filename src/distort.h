#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Timespan.h"
#include "particleShader.h"

class distort{

	public:
		distort() {};
		~distort() {};

		void setup(ofRectangle distortRect1, ofRectangle distortRect2);//ofRectangle distortRect
		void update(float intensity, float radiusEffect);
		void begin();
		void end();
		void draw();	
		void drawDebug();



		ofShader shaderPost;
		Poco::Timestamp lastFragTimestampPost, lastVertTimestampPost;

		ofxPanel gui;

		ofxFloatSlider val[3];

		static const int arrayLength = 10;
		ofVec2f arrayUniform[arrayLength];
		float arrayParticleSize[arrayLength];

		ofxIntSlider fpsSave;
		int lastSave;
		ofFbo fbo;

		//ofFbo fbo2;

		ofxToggle saveFrame;

		vector<particleShader> p;


		ofRectangle fboRect;

};