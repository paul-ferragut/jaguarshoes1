#pragma once

#include "ofMain.h"
//#include "ofxShadowSimple.h"

//#include "ofxSvg.h"
#include "ofxGui.h"
#define SHADER_PARAM_NUM 10
class shaderShape {
public:

	shaderShape();
	~shaderShape();
    void setup(string shaderString, string settingsVars, vector<ofPath>p);
	//, vector<float>v
    //void update();, vector<ofFloatColor>floatColors
    void draw(bool differentTime, vector<ofFloatColor>floatColors);
	bool showGui;
	void saveGui();
	ofxPanel gui;
	
    
private:
	//ofxSVG svg;
	//ofEasyCam cam;
	ofShader shader;
	vector<ofPath>paths;
	vector<float>uniqueTime;
	//vector<ofFloatColor>fColors;
	//vector<ofPath>vars;
	ofParameter<float>vars[SHADER_PARAM_NUM];
	string saveSettingsString;
	ofParameter<bool>saveGuiB;
	float uniqueTimeGroup;
	float redF[6];
	float greenF[6];
	float blueF[6];
};
