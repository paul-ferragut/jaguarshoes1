#include "glow.h"

glow::glow() {

}

glow::~glow() {
	path.clear();
}
//--------------------------------------------------------------
void glow::setup(string svgString){
	ofxSVG svg;
	svg.load(svgString);

	int largerPath = 0;

	for (int i = 0;i < svg.getNumPath();i++) {
		glowPath tGlowPath;
		tGlowPath.path = svg.getPathAt(i);

		tGlowPath.path.setPolyWindingMode(OF_POLY_WINDING_ODD);
		tGlowPath.path.setColor(ofColor(255, 255, 255,255));

		vector<ofPolyline>poly = tGlowPath.path.getOutline();
		ofVec2f pathCenter(0, 0);
		ofVec2f pathPos(0, 0);	
		int tSize = 0;
		for (int k = 0;k < poly.size();k++) {
			pathCenter += poly[k].getCentroid2D();
		
			pathPos.x += poly[k].getBoundingBox().x;
			pathPos.y += poly[k].getBoundingBox().y;

			tSize += poly[k].getBoundingBox().width;
		
		}
		pathCenter.x = pathCenter.x / poly.size();
		pathCenter.y = pathCenter.y / poly.size();

		pathPos.x = pathPos.x / poly.size();
		pathPos.y = pathPos.y / poly.size();

		tSize = tSize / poly.size();

		tGlowPath.centroid = pathCenter;
		tGlowPath.pos = pathPos;

		if (tSize > largerPath) {
			largerPath = tSize;
		}

		tGlowPath.size = tSize;

		
		int iteration = 15;
		for (int k = 0;k < iteration;k++) {
			tGlowPath.iterationOpacity.push_back(255);
		}


		path.push_back(tGlowPath);
	}

	for (int i = 0;i < path.size();i++) {
		path[i].size = ofMap(path[i].size, 0, largerPath, 0.0, 1.0, true);
		//cout << "size" << path[i].size << endl;
	}

}

//--------------------------------------------------------------
void glow::update(ofVec2f position){
	
	for (int i = 0;i < path.size();i++) {

		path[i].offset = position - path[i].centroid;
		path[i].offset.normalize();

	}
}

//--------------------------------------------------------------
void glow::draw(bool pulse,ofColor col){
	ofEnableAlphaBlending();
	ofSetLineWidth(3.0);
	if(pulse){
		for (int i = 0;i < path.size();i++) {
			for (int j = 0;j < path[i].iterationOpacity.size();j++) {
				path[i].iterationOpacity[0] = 255;
			}
		}
		//cout << "pulse" << endl;
	}
	int decrease=15;
	for (int i = 0;i < path.size();i++) {

		for (int j = 0;j <  path[i].iterationOpacity.size();j++) {

			//if (j > 0) {
				//cout <<"j"<<j<< " opacity"<<path[i].iterationOpacity[j - 1]<< endl;

			//}

			if (path[i].iterationOpacity[j] > 15 ) {
				path[i].iterationOpacity[j]-= decrease;
			}
		}

		int iterations = path[i].iterationOpacity.size();

		for (int j = 0;j < iterations;j++) {
			float scale = ofMap(j, 0, iterations,0.2,1.0  , true);
			float scaleI = ofMap(j, 0, iterations,130.0*path[i].size,  0.0, true);
			ofPushMatrix();
			//ofScale(scale, scale);
			//ofTranslate(scale, scale);path[i].centroid+	
			
			ofTranslate((path[i].centroid));//pathPos

			ofScale(scale, scale);
			//

			ofTranslate(-(path[i].centroid));
			ofTranslate((path[i].offset*(scaleI)));
			//ofTranslate(-(path[i].centroid /2));
			/*
			ofTranslate(pos);
			ofScale(width / hP, width / hP);
			ofTranslate(-(wP / 2), -(hP / 2));*scale
			*/
			ofNoFill();
			path[i].path.setStrokeColor(ofColor(col.r, col.g, col.b, path[i].iterationOpacity[j]));
			path[i].path.setStrokeWidth(4);
			path[i].path.setFilled(false);
			path[i].path.draw();
			ofPopMatrix();
		}
	}


	for (int i = 0;i < path.size();i++) {
		for (int j = 0;j < path[i].iterationOpacity.size()-1;j++) {
			if (path[i].iterationOpacity[j] == 255- decrease) {
				path[i].iterationOpacity[j + 1] = 255;
			}
		}
	}


}

