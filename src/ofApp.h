#pragma once

#include "ofMain.h"
#include "ofxSunCalc.h"
#include "ofxYahooWeather.h"
#include "ofxSunCalc.h"
#include "ofxAssimpModelLoader.h"
#include "ofxUI.h"
#include "ofx2DCam.h"
//#include "ofxPostProcessing.h"
#include "ofxVboParticles.h"
#include "ofxMtlMapping2D.h"
//test
//test
#define VAR_SHADER 6
#define COLOR_IN_PALETTE 5
#define NUM_LIGHT 2
//local change

struct colorPalette {
	
	float r[COLOR_IN_PALETTE], g[COLOR_IN_PALETTE], b[COLOR_IN_PALETTE];
	float rFloat[COLOR_IN_PALETTE], gFloat[COLOR_IN_PALETTE], bFloat[COLOR_IN_PALETTE];


	ofColor getCol(int i) {
		return ofColor(r[i], g[i], b[i]);
	}
	void convertToFloatCol() {
		for (int i = 0;i < COLOR_IN_PALETTE;i++) {
			rFloat[i] = ofMap(r[i], 0, 255, 0.0, 1.0);
			gFloat[i] = ofMap(g[i], 0, 255, 0.0, 1.0);
			bFloat[i] = ofMap(b[i], 0, 255, 0.0, 1.0);
		}
	}

	void setCol(ofColor col,int i) {
		r[i]=col.r;
		g[i]=col.g;
		b[i]=col.b;
	}

};

struct lightStruct{

	ofLight light;
	ofVec3f position;
	ofVec3f orientation;
	bool useLight;

};


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


		//<--  AUDIO
		void audioIn(float * input, int bufferSize, int nChannels);
		float smoothedVol;
		float scaledVol;
		int 	bufferCounter;
		int 	drawCounter;
		vector <float> left;
		vector <float> right;
		vector <float> volHistory;
		ofSoundStream soundStream;
		//AUDIO -->

		//<--  WEATHER
		ofxYahooWeather weather;
		int londonWOEID;
		// WEATHER -->

		//<--  SUN
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
		void drawWeatherDebug();
		bool drawWeatherDebugB;
		//SUN -->

		ofxAssimpModelLoader modelStructure;
		ofxAssimpModelLoader modelFoliage1;
		ofxAssimpModelLoader modelFoliage2;

		//<-- CAM
		ofx2DCam cam;
		ofEasyCam easyCam;
		bool bUseEasyCam;		
		ofVec3f positionCamera;
		ofVec3f rotationCamera;
		float modelFoliageScale;
		float cameraFov;
		//CAM -->

		//<-- GUI
		void setGUI();
		void guiEvent(ofxUIEventArgs &e);
		ofxUISuperCanvas *gui1;
		ofxUISuperCanvas *gui2;
		ofxUISuperCanvas *gui3;
		// GUI -->



		

		ofFbo fbo;


		float drawingScale;


		void drawOverlayImage();
		bool drawOverlayImageB;
		ofImage image;


		void drawLightBegin();
		void drawLightEnd();
		bool drawLightDebugB;
		lightStruct lights[NUM_LIGHT];
		ofMaterial material;

		//<-- LEAVES
		void drawUnderneath();
		bool drawUnderneathB;
		//TEXTURES
		bool useTextureB;
		ofShader shaderTexture;
		int resolutionWidthTexture;
		int resolutionHeightTexture;
		float fluidity[3];
		float timeMotion;
		float scaleTexture;
		//SHADOWS		
		void drawUnderneathShadows();
		bool drawUnderneathShadowsB;
		//bool useDirectShadowB;
		ofVec3f shadowOffset;
		//LEAVES -->

		//<-- STRUCTURES
		void drawBackgroundStructure();
		bool drawBackgroundStructureB;
		bool structureToDustB;
		ofxVboParticles *vboParticles;
		ofMesh mesh;
		//STRUCTURES -->

		//<-- BG 
		void drawBackground();
		bool drawBackgroundB;
		//BG -->

		colorPalette colorP;

		//<-- DISTORTIONS
		void drawPostFlatBegin();
		void drawPostFlatEnd();
		ofShader shaderPost;
		float uniformFloatShader[VAR_SHADER];
		float opacityShader;
		bool usePostFlatShaderB;
		bool usePostWithSoundB;
		// DISTORTIONS -->

		//GODRAYS
		void drawPost3dBegin();
		void drawPost3dEnd();
		bool usePost3dShaderB;
		ofShader shaderGodrays;



		

		





		   private:
			   ofxMtlMapping2D* _mapping;
};