#pragma once

#include "ofMain.h"
#include "ofxSunCalc.h"
#include "ofxYahooWeather.h"
#include "ofxSunCalc.h"
#include "ofxAssimpModelLoader.h"
#include "ofxUI.h"
//test
//test

//local change


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit();

		ofxYahooWeather weather;
		int londonWOEID;


		ofxSunCalc sun_calc;
		SunCalcDayInfo todayInfo;

		string min_info_str;
		string max_info_str;
		string pos_str;
		string date_str;
		string latlon_str;

		vector<ofFbo> timelines;
		vector<string> labels;

		float lat;
		float lon;


		ofxAssimpModelLoader modelStructure;
		ofxAssimpModelLoader modelFoliage;

		ofEasyCam camEasy;
		ofCamera camStatic;
		

		void setGUI();
		void guiEvent(ofxUIEventArgs &e);
		ofxUISuperCanvas *gui;

		bool showWeather;

		ofVec3f positionCamera;
		ofVec3f rotationCamera;
		float modelFoliageScale;
		float cameraFov;

		ofColor bg1;
		ofColor bg2;

		ofShader texture;
		ofShader post;

		ofImage image;
};