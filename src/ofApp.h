#pragma once

#include "ofMain.h"
#include "ofxSunCalc.h"
#include "ofxYahooWeather.h"
#include "ofxSunCalc.h"
#include "ofxAssimpModelLoader.h"
#include "ofxUI.h"
#include "ofx2DCam.h"
//test
//test
#define VAR_SHADER 6
//local change


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void drawWeatherDebug();
		bool drawWeatherDebugB;

		void drawOverlayImage();
		bool drawOverlayImageB;

		void drawUnderneath();
		bool drawUnderneathB;
		bool useTextureB;
		bool useLightsB;

		void drawBackgroundStructure();
		bool drawBackgroundStructureB;
		bool structureToDustB;

		void drawBackground();
		bool drawBackgroundB;
		float bgRed1, bgGreen1, bgBlue1;
		float bgRed2, bgGreen2, bgBlue2;
	
		
		void drawPostBegin();
		void drawPostEnd();

		bool usePostShaderB;
		bool usePostWithSoundB;


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

		void audioIn(float * input, int bufferSize, int nChannels);
		float smoothedVol;
		float scaledVol;
		int 	bufferCounter;
		int 	drawCounter;
		vector <float> left;
		vector <float> right;
		vector <float> volHistory;
		ofSoundStream soundStream;


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
		ofxAssimpModelLoader modelFoliage1;
		ofxAssimpModelLoader modelFoliage2;

		//ofEasyCam camEasy;
		//ofCamera camStatic;
		ofx2DCam cam;
		ofEasyCam easyCam;
		bool bUseEasyCam;
		

		void setGUI();
		void guiEvent(ofxUIEventArgs &e);
		ofxUISuperCanvas *gui;

		bool showWeather;

		ofVec3f positionCamera;
		ofVec3f rotationCamera;
		float modelFoliageScale;
		float cameraFov;


		ofShader texture;
		ofShader post;

		ofImage image;

		ofFbo fbo;

		ofShader shader;

		float uniformFloatShader[VAR_SHADER];
		float opacityShader;

};