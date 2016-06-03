#include "svgDrawSimple.h"

svgDrawSimple::svgDrawSimple()
{

}

//--------------------------------------------------------------
svgDrawSimple::~svgDrawSimple()
{

}

//--------------------------------------------------------------
void svgDrawSimple::setup(string svgString) {
    
	ofxSVG svg;
	svg.load(svgString);

	for (int i = 0; i < svg.getNumPath(); i++) {

		ofPath p = svg.getPathAt(i);
		p.setPolyWindingMode(OF_POLY_WINDING_ODD);
		paths.push_back(p);

	}
	
	shader.load("shaders/noiseWater");
	
}


//--------------------------------------------------------------
void svgDrawSimple::draw(ofColor color1, ofColor color2){
    
	//ofSetColor(255, 255, 255);
	//ofPushMatrix();
	ofFloatColor fC1 = color1;
	ofFloatColor fC2 = color2;
	ofSetColor(255, 255, 255);
	for (int i = 0; i < paths.size();i++) {
		shader.begin();
		shader.setUniform3f("color1", fC1.r, fC1.g, fC1.b);
		shader.setUniform3f("color2", fC2.r, fC2.g, fC2.b);
		shader.setUniform1f("time", ofGetElapsedTimeMillis()*0.001+(i*1000));
		shader.setUniform2f("resolution",ofGetWidth(),ofGetHeight());
		paths[i].draw();
		shader.end();
	}

}
