#pragma once

#include "ofMain.h"
//#include "ofxShadowSimple.h"

#include "ofxSvg.h"
#include "ofx2DCam.h"
#include "shaderShape.h"

class svgDraw {
public:

	svgDraw();
	~svgDraw();
    void setup(string svgString);
    void update();
    void draw(vector<ofFloatColor>colorsInput);

    
private:
	ofxSVG svg;
	//ofEasyCam cam;

	vector<ofPath>paths;
	vector<vector<ofPath>>pathsShader;
	vector<float>zPosition;
	bool isGray(ofColor cGray);

	vector<ofColor>colors;
	vector<shaderShape>shaderShapes;
};
