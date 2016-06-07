#pragma once



#define COLOR_IN_PALETTE 5
#define NUM_LIGHT 2
#define NUM_SCREEN 4
#define LIGHT_NUM 5
#define USE_STRUCT
//#define USE_SAVESCREEN
//#define USE_WEATHER

#include "ofMain.h"

#ifdef USE_WEATHER
#include "ofxSunCalc.h"
#include "ofxYahooWeather.h"
#endif

#include "ofxAssimpModelLoader.h"
#include "ofxUI.h"
#include "ofx2DCam.h"
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
		int peakCounter;
		//AUDIO -->


		#ifdef USE_WEATHER
		//<--  SUN
		ofxSunCalc sun_calc;
		SunCalcDayInfo todayInfo;
		float lat;
		float lon;
		void setupWeather();
		void drawWeatherDebug();
		bool drawWeatherDebugB;
		//SUN -->		
		
		//<--  WEATHER
		ofxYahooWeather weather;
		int londonWOEID;
		// WEATHER -->
		#endif

		//<-- CAM
		ofx2DCam cam;
		ofEasyCam easyCam;
		bool bUseEasyCam;		
		//CAM -->

		//<-- GUI
		void setGUI();
		void guiEvent(ofxUIEventArgs &e);
		ofxUISuperCanvas *gui1;
		ofxUISuperCanvas *gui2;
		ofxUISuperCanvas *gui3;
		// GUI -->

		void drawBackground();

		ofImage roomImage;
		ofImage roomImageDebug;

		//<-- LIGHTS 
		superLight light[LIGHT_NUM];
		float lightZ[LIGHT_NUM];
		bool useLightB[LIGHT_NUM];
		bool useShadowB[LIGHT_NUM];
		float shadowSpread[LIGHT_NUM];
		bool useTopShadowB[LIGHT_NUM];
		float cutoff[LIGHT_NUM];
		float concentration[LIGHT_NUM];
		int lightIndex1[LIGHT_NUM];
		int lightIndex2[LIGHT_NUM];

		bool useMaterialB;
		float shininess;
		ofMaterial material;
		float zExtrusionShapes;
		//LIGHTS -->

		bool useMappingB;
		bool use2DCamB;
		bool useSoundB;
		bool drawRoomB;
		bool drawRoomDebugB;
		bool drawBackgroundB;
		bool drawIllustrationB;
		bool drawStructureB;
		bool drawVoroB;
		bool showGuiPSystemLeftB;
		bool showGuiPSystemRightB;
		bool drawFlatShapesB;
		bool drawPulsingShapesB;
		bool drawGenerativeFlowersB;
		bool drawDebugB;
		bool flipYB;

		bool usePostShaderB;
		float postRadius;
		float postIntensity;
		distort distortI;

		#ifdef USE_SAVESCREEN
		bool saveScreenB;
		bool saveScreenVideoB;
		ofFbo saveFbo;
		//
		ofSoundPlayer player;
		float 				* fftSmoothed;


		int nBandsToGet;
		#endif

		//<-- COLORS
		bool useTimeColorB;
		bool useSimulatedTimeColorB;
		float simulatedTime;
		ofColor colorP[COLOR_IN_PALETTE];
		float rC[COLOR_IN_PALETTE];
		float gC[COLOR_IN_PALETTE];
		float bC[COLOR_IN_PALETTE];
		ofRectangle colDebugRect;
		ambientColor color;
		//COLORS -->
		
		ofxSVG * shadowBack;
		ofxSVG * shadowFront;

		branches branchesFlower[4];
		glow glowingShapes;
		svgDrawSimple svgShapeSimple;

		voronoi voro;
		svgDraw svgDrawing;

		ParticleSystemSpawnTexture pSystemLeft[3];
		ParticleSystemSpawnTexture pSystemRight[2];
		int currentPSystemLeft;
		int currentPSystemRight;


		

		   private:
			   ofxMtlMapping2D* _mapping;
};