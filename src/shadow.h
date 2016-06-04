#pragma once

#include "ofMain.h"
//#include "ofxAutoReloadedShader.h"
#include "ofxGui.h"
#include "ofxSvg.h"
#include "ofx2DCam.h"
#include "ofxLayerMask.h"

class shadow{

	public:
		shadow(){};
		~shadow() {};

		void setup(ofxSVG *svgShadowTopIn, ofxSVG *svgShadowBackIn);
		//void update();
		void drawBackShadow(ofVec2f posLight,float shadowSpread);
		void drawShadowTop(ofVec2f posLight,float shadowSpread);//, float levelsShadow
		void exit();

		void drawShadow(float exentricity=40.0f, float exentricity2=0.0f, ofColor color=ofColor(0,0,0), int index=0);
		void drawShadowShape( float spread);//int levels,

		//ofxAutoReloadedShader shader;
		ofShader blurVShader;
		ofShader blurHShader;
		ofxPanel gui;
		ofxVec3Slider posLight;
		ofxVec3Slider lookAtLight;

		//Poco::Timestamp lastFragTimestampPost, lastVertTimestampPost;

		ofxFloatSlider blurFactor;
		ofxFloatSlider texelSize;

		ofx2DCam cam2;

		ofxSVG *svgShadowTop;
		ofxSVG *svgShadowBack;		//ofEasyCam cam;

		vector<ofPath>paths;
		//vector<vector<ofPath>>pathsShader;
		vector<vector<ofPath>>pathsShaderTopS;
		vector<ofPath>pathsShaderBackS;
		vector<float>zPosition;
		bool isGray(ofColor cGray);

		ofFbo fbo;
		ofFbo fbo2;

		ofVec3f pos;
		
		ofxLayerMask masker;
		//vector<vector<int>>maskLayersI;
		vector<int>maskLayersI;
		int maskLayer;

		int fadeIndex;
		

};
