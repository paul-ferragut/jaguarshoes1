#include "voronoi.h"

//------------------------------------------------------------------
voronoi::voronoi(){

}

//------------------------------------------------------------------
voronoi::~voronoi() {
	
}


//------------------------------------------------------------------
void voronoi::setup(){


//	ofSoundStreamSetup(0, 1, this, 44100, beat.getBufferSize(), 4);

	ofSetFrameRate(60);
	ofBackground(0, 0, 0);

	bounds.set(0, 0, 1527, 1033);
	voro.setup(bounds);


	int n = 100;

	for (int i = 0; i<n; i++) {
		ofPoint pt(bounds.x + ofRandom(bounds.width), bounds.y + ofRandom(bounds.height));// ofRandomWidth(), ofRandomHeight());
		pts.push_back(pt);
	}

	//cout << "open" << endl;
	svg.load("voronoi.svg");
	//generateTheVoronoi();



	//cout << "numpath" << svg.getNumPath() << endl;
	for (int i = 0;i < svg.getNumPath();i++) {


		ofPath p = svg.getPathAt(i);
		p.setPolyWindingMode(OF_POLY_WINDING_ODD);//IMPORTANT

												  //vector<ofPolyline>p = path.getOutline();
		vector<ofPolyline>&lines = const_cast<vector<ofPolyline>&>(p.getOutline());

		for (int j = 0;j<(int)lines.size();j++) {
			//ofPolyline poly = lines[j];
			ofPolyline poly = lines[j];//.getResampledBySpacing(1);
									   //cout << "lines" << j << endl;

			vector<ofPoint>ptP = poly.getVertices();
			for (int k = 0;k < ptP.size();k++) {

				pts.push_back(ptP[k]);
				//cout << "poly[k]" << poly[k]<<endl;
			}


		}

		//for (int j = 0;j < lines.size();j++) {
		//	lines[j].get
		//	vector<ofPoint>ptss = &lines[j].getVertices;

		//	for (int k = 0;k < ptss.size();k++) {
		//		pts.push_back(ptss[k]);
		//	}

		//	}
	}


	voro.clear();

	//cout << "start voronoi" << pts.size() << endl;
	for (int i = 0; i<pts.size(); i++) {
		voro.addPoint(pts[i]);
	}

	voro.generateVoronoi();


	attractPoints.clear();
	for (int i = 0; i < 4; i++) {
		attractPoints.push_back(ofPoint(ofMap(i, 0, 4, 100, ofGetWidth() - 100), ofRandom(100, ofGetHeight() - 100)));
	}
	attractPointsWithMovement = attractPoints;

	for (int i = 0;i < voro.cells.size();i++) {

		for (int j = 0; j<voro.cells[i].pts.size(); j++) {
			particleVoronoi pT;
			pT.reset();

			pT.pos = voro.cells[i].pts[j];
			pT.originPos = voro.cells[i].pts[j];
			pT.setAttractPoints(&attractPointsWithMovement);
			pT.cellSize = ofMap(voro.cells[i].cellBounds.getArea(), 0, 20000, 0.3, 1.0, true);
			//1.0;
			//cout << pT.cellSize << endl;
			for (int k = 0;k < p.size();k++) {
				if (pT.pos == p[k].pos) {
					pT.isDouble = true;
					pT.doubleIndex = k;
				}
			}

			p.push_back(pT);
		}

	}

	//currentMode = PARTICLE_MODE_PAUL;
	//currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";	

	//p.assign(num, demoParticle());
	//resetParticles();

//	over.load("over.png");



	positionBounds=bounds;
	//positionBounds.y; += (bounds.height / 5);
	positionBounds.height /= 2;
	uniqueStartPos.set(ofRandom(0,1000),ofRandom(0,1000));
	speedPosition = ofVec2f(0.16, 0.16);
}

//------------------------------------------------------------------
void voronoi::update(float noise){
	//beat.update(ofGetElapsedTimeMillis());
	//voronoi.cells[0].pts[0].set(mouseX, mouseY);

	for (unsigned int i = 0; i < p.size(); i++) {
		if (p[i].isDouble == false) {
			p[i].update();
		}
		else {
			p[i].pos = p[p[i].doubleIndex].pos;
		}

	}
	//lets add a bit of movement to the attract points
	//for (unsigned int i = 0; i < attractPointsWithMovement.size(); i++) {
		//	attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
		//	attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
	//}

	/*beat.kick() == 1.0&& beat.snare() == 1.0&&beat.hihat() == 1.0*/
	if (noise==1.0) {
		for (unsigned int i = 0; i < p.size(); i++) {
			p[i].soundPt=soundPoint;
			p[i].bassTap = true;
		}
	}
	



		float xFactor = positionBounds.width;
		float yFactor = positionBounds.height;
		//*frequencyPosition.x*frequencyPosition.y
		

		soundPoint.x = positionBounds.x + ofNoise(((ofGetElapsedTimef()*speedPosition.x) + uniqueStartPos.x))*xFactor;
		soundPoint.y = positionBounds.y + ofNoise(((ofGetElapsedTimef()*speedPosition.y) + uniqueStartPos.y))*yFactor;
		




	//cout << beat.kick() << "," << beat.snare() << "," << beat.hihat() << endl;
}
/*
void voronoi::audioReceived(float* input, int bufferSize, int nChannels) {
	beat.audioReceived(input, bufferSize, nChannels);
}
*/
//------------------------------------------------------------------
void voronoi::draw(){

	//ofSetColor(255, 0, 0);
	//svg.draw();
	//ofSetColor(255);
	ofNoFill();
	ofRect(bounds);


	// draw the raw points
	//for (int i = 0; i<pts.size(); i++) {
	//	ofSetColor(255);
		//ofCircle(pts[i], 2);
	//}

	int indexV = 0;
	for (int i = 0;i < voro.cells.size();i++) {

		for (int j = 0; j<voro.cells[i].pts.size(); j++) {
			voro.cells[i].pts[j] = p[indexV].pos;
			indexV++;
		}
		//ofDrawRectangle(voronoi.cells[i].cellBounds);

	}


	for (unsigned int i = 0; i < p.size(); i++) {
		//p[i].draw();
	}
	ofSetLineWidth(0.5);
	voro.draw();
	//

	/*
	for (int i = 0;i < svg.getNumPath();i++) {


		ofSetColor(ofColor::orange);
		ofPath p = svg.getPathAt(i);
		p.setPolyWindingMode(OF_POLY_WINDING_ODD);

		//vector<ofPolyline>p = path.getOutline();
		vector<ofPolyline>&lines = const_cast<vector<ofPolyline>&>(p.getOutline());

		for (int j = 0;j<(int)lines.size();j++) {
			ofPolyline poly = lines[j];//.getResampledBySpacing(1);
									   //cout << "lines" << j << endl;

			vector<ofPoint>ptP = poly.getVertices();
			for (int k = 0;k < ptP.size();k++) {
				//	ofDrawRectangle(ptP[k], 3, 3);
				//	pts.push_back(poly[k]);
				//cout << "poly[k]" << poly[k]<<endl;
			}


		}


	}
	*/
//	over.draw(0, 0);
	ofDrawCircle(soundPoint, 10);
}

