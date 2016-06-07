#pragma once
#include "ofMain.h"
#include "ofxSvg.h"


class svgDrawSimple {
public:

	svgDrawSimple();
	~svgDrawSimple();
    void setup(string svgString);
    //void update();
    void draw(ofColor color1,ofColor color2,float noiseLevel=0.3);

    
private:
	//ofxSVG svg;
	//ofEasyCam cam;
	ofShader shader;
	vector<ofPath>paths;
};
