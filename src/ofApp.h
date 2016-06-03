#pragma once

#include "ofMain.h"
#include "ofxSunCalc.h"
#include "ofxYahooWeather.h"
//#include "ofxSunCalc.h"
#include "ofxAssimpModelLoader.h"
#include "ofxUI.h"
#include "ofx2DCam.h"
//#include "ofxPostProcessing.h"
//#include "ofxVboParticles.h"
#include "ofxMtlMapping2D.h"
#include "superLight.h"
#include "ambientColor.h"
#include "voronoi.h"
#include "ParticleSystemSpawnTexture.h"
#include "svgDraw.h"
#include "distort.h"


#include "glow.h"
#include "branches.h"
#include "svgDrawSimple.h"

//test
//test
//#define VAR_SHADER 6
#define COLOR_IN_PALETTE 5
#define NUM_LIGHT 2
#define NUM_SCREEN 4
#define LIGHT_NUM 5
#define USE_STRUCT
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

		float soundAverage;
		float averageDuration;
		bool peak;
		//int 	bufferCounter;
		//int 	drawCounter;
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

		//ofxAssimpModelLoader modelStructure;
		//ofxAssimpModelLoader modelFoliage1;
		//ofxAssimpModelLoader modelFoliage2;

		//<-- CAM
		ofx2DCam cam;
		ofEasyCam easyCam;
		bool bUseEasyCam;		
		//ofVec3f positionCamera;
		//ofVec3f rotationCamera;
		//float modelFoliageScale;
		//float cameraFov;
		//CAM -->

		//<-- GUI
		void setGUI();
		void guiEvent(ofxUIEventArgs &e);
		ofxUISuperCanvas *gui1;
		ofxUISuperCanvas *gui2;
		ofxUISuperCanvas *gui3;
		// GUI -->

		//ofFbo fbo;


		//float drawingScale;


	//	void drawOverlayImage();
	//	bool drawOverlayImageB;
	//	ofImage image;


	//	void drawLightBegin();
	//	void drawLightEnd();
	//	bool drawLightDebugB;
		//lightStruct lights[NUM_LIGHT];
	//	ofMaterial material;

		//<-- LEAVES
	//	void drawUnderneath();
	//	bool drawUnderneathB;
		//TEXTURES
		/*
		bool useTextureB;
		ofShader shaderTexture;
		Poco::Timestamp lastFragTimestampTexture, lastVertTimestampTexture;
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
		*/
		//LEAVES -->

		//<-- STRUCTURES
		/*
		void drawBackgroundStructure();
		bool drawBackgroundStructureB;
		bool structureToDustB;
		ofxVboParticles *vboParticles;
		ofMesh mesh;*/
		//STRUCTURES -->

		//<-- BG 
		void drawBackground();
		//BG -->

		//<-- ROOM 

		ofImage roomImage;
		ofImage roomImageDebug;
		//BG -->

		//colorPalette colorP;

		//<-- DISTORTIONS
		/*
		void drawPostFlatBegin();
		void drawPostFlatEnd();
		ofShader shaderPost;
		Poco::Timestamp lastFragTimestampPost, lastVertTimestampPost;
		float uniformFloatShader[VAR_SHADER];
		float opacityShader;*/
		
		//bool usePostWithSoundB;
		
		// DISTORTIONS -->

		//GODRAYS
		//void drawPost3dBegin();
		//void drawPost3dEnd();
		//bool usePost3dShaderB;
		//ofShader shaderGodrays;


		//
		superLight light[LIGHT_NUM];
		float lightType[LIGHT_NUM];
		float shadowSpread[LIGHT_NUM];
		float lightZ[LIGHT_NUM];
		bool useLightB[LIGHT_NUM];
		bool useTopShadowB[LIGHT_NUM];
		float topShadowResolution[LIGHT_NUM];

		bool useMaterialB;
		float shininess;
		ofMaterial material;

		ambientColor color;
		voronoi voro;
		svgDraw svgDrawing;
		ParticleSystemSpawnTexture pSystemLeft[3];
		ParticleSystemSpawnTexture pSystemRight[2];
		int currentPSystemLeft;
		int currentPSystemRight;


		//MAPPING
		bool usePostShaderB;
		float postRadius;
		float postIntensity;
		distort distortI;

		bool flipYB;


		bool useMappingB;
		bool use2DCamB;
		//bool useLightB;
		bool drawDebugGridB;
		bool drawRoomB;
		bool drawRoomDebugB;
		bool drawBackgroundB;
		bool drawStructureB;
		bool drawVoroB;
		bool useSoundB;
		bool drawIllustrationB;
		
		bool showGuiPSystemLeftB;
		bool showGuiPSystemRightB;
		
		bool useTimeColorB;
		bool useSimulatedTimeColorB;
		float simulatedTime;
		ofColor colorP[COLOR_IN_PALETTE];
		float rC[COLOR_IN_PALETTE];
		float gC[COLOR_IN_PALETTE];
		float bC[COLOR_IN_PALETTE];
		ofRectangle colDebugRect;

		float zExtrusionShapes;


		ofxSVG * shadowBack;
		ofxSVG * shadowFront;

		//float x;
		//float y;
		//float z;

		branches branchesFlower[4];
		glow glowingShapes;
		svgDrawSimple svgShapeSimple;

		bool tLensoffset;
		   private:
			   ofxMtlMapping2D* _mapping;
};