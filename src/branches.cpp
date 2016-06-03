#include "branches.h"

//------------------------------------------------------------------
branches::branches(){

}
//------------------------------------------------------------------
branches::~branches(){
   // attractPoints = NULL;
	b.clear();
}


//------------------------------------------------------------------

void branches::setup(ofVec2f pos, float w) {
	vector<particleFlower>tPV;
	wP = 1024;
	hP = 768;
	int branchNum = ofRandom(25, 15);

	for (int i = 0;i < branchNum;i++) {
		float angle = ofDegToRad(ofRandom(0, 360));
		ofVec2f attractP((wP / 2) + (cos(angle) * (hP / 2)), (hP / 2) + (sin(angle) * (hP / 2)));

		branch tB;
		tB.currentLevel = 0;
		tB.maxBranches = ofRandom(40, 55);
		tB.directionPt.set(attractP);

		particleFlower tP;
		tP.setup(ofVec2f(wP / 2, hP / 2), tB.directionPt, 0);
		cout << ofVec2f(wP / 2, hP / 2) << endl;
		tB.p.push_back(tP);
		b.push_back(tB);

	}

	position=pos;
	width = w;

}

//------------------------------------------------------------------
void branches::update(){
	//ofSetBackgroundAuto(false);
	for (int i = 0;i < b.size();i++) {
		bool allInactive = true;
		for (int j = 0;j < b[i].p.size();j++) {


			int createNew = 0;
			createNew = b[i].p[j].update();
			if (createNew == STATE_ENDED) {
				if (b[i].currentLevel < b[i].maxBranches) {
					b[i].currentLevel++;

					int newBranch = ofRandom(1, 4);

					for (int k = 0;k < newBranch;k++) {
						particleFlower tP;
						tP.setup(b[i].p[j].pos, b[i].directionPt, b[i].currentLevel);
						///cout << "b[i].p[j].pos" << b[i].p[j].pos << endl;
						b[i].p.push_back(tP);
					}
				}
			}
			//b[i].p.end();b[i].p[j].duration>b[i].p[b[i].p.size()-1].breakEnd
			if ((createNew == STATE_INACTIVE && b[i].p[j + 1].state == STATE_READY) || b[i].p[b[i].p.size() - 1].state == STATE_INACTIVE) {
				b[i].p[j].readyDeconstruct = true;
			}

			if (createNew != STATE_READY) {
				allInactive = false;
			}
		}

		if (allInactive == true) {
			//
			float angle = ofDegToRad(ofRandom(0, 360));
			ofVec2f attractP((wP / 2) + (cos(angle) * (hP / 2)), (hP / 2) + (sin(angle) * (hP / 2)));
			b[i].currentLevel = 0;
			b[i].maxBranches = ofRandom(40, 55);
			b[i].directionPt.set(attractP);
			//for (int j = 0;j < b[i].p.size();j++) {
			b[i].p.clear();
			particleFlower tP;
			tP.setup(ofVec2f(wP / 2, hP / 2), b[i].directionPt, b[i].currentLevel);
			///cout << "b[i].p[j].pos" << b[i].p[j].pos << endl;
			b[i].p.push_back(tP);

			//b[i].p[0].setup(ofVec2f(wP / 2, hP / 2), b[i].directionPt, 0);

			cout << "setup branch" << i << endl;
		}
	}
		
}

//------------------------------------------------------------------
void branches::draw( ofColor colStart, ofColor colEnd){
	//float scaleN = ofMap(scale, 0.0f, 1.0f, 1.0f, 0.1f,true);
	ofPushMatrix();

	ofTranslate(position);
	ofScale(width/ hP, width / hP);
	ofTranslate(-(wP/2), -(hP/2));	

	for (int i = 0;i < b.size();i++) {
		for (int j = 0;j < b[i].p.size();j++) {
			b[i].p[j].draw(colStart, colEnd);
		}
	}
	ofPopMatrix();

}

