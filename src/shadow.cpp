#include "shadow.h"

//--------------------------------------------------------------
void shadow::setup(ofxSVG *svgShadowTopIn, ofxSVG *svgShadowBackIn){
	//svg.load("illu2.svg");
 
	svgShadowBack = svgShadowBackIn;
	svgShadowTop = svgShadowTopIn; //NOT WORKING

//	svgShadowBack.load("illu2.svg");
	//shader.load("shaders/blur");
	//cam.disableMouseInput();


	blurHShader.load("shaders/basic.vert", "shaders/gaussblur_h5.frag");
	blurVShader.load("shaders/basic.vert", "shaders/gaussblur_v5.frag");

	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	fbo2.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);//32F_ARB

	fbo2.begin();
	ofClear(255, 255, 255, 0);
	fbo2.end();

	vector<ofColor>groupByCol;
	groupByCol.clear();

	vector<vector<ofPath>>pathsShader;


	for (int i = 0; i < svgShadowTop->getNumPath(); i++) {
		ofPath p = svgShadowTop->getPathAt(i);
		p.setPolyWindingMode(OF_POLY_WINDING_ODD);
		ofColor colToDepth = p.getFillColor();

		if (isGray(colToDepth)==false) {	
		}
		else {
			bool colExist = false;

			for (int j = 0;j < groupByCol.size();j++) {
				if (colToDepth == groupByCol[j]) {
					colExist = true;
				}
			}
			if (colExist == false) {
				
				float zPos = ofMap(colToDepth.getBrightness(), 0, 255, 200, 0.0);
				zPosition.push_back(zPos);
			//cout << "zPos Top" << zPos << endl;
				groupByCol.push_back(colToDepth);

				//cout << "color" << colToDepth << endl;
				//vector<ofPath>tP;
				//ofPath pp
				//tP.push_back(pp);
				//pathsShader.push_back(tP);
				maskLayersI.push_back(0);
			}
		}
	}



	ofSetWindowShape(ofGetWidth(), ofGetHeight());
	masker.setup(ofGetWidth(), ofGetHeight());
	maskLayer = masker.newLayer();


	for (int i = 0; i < zPosition.size(); i++) {

	//	maskLayersI[i] = masker.newLayer();
	}


	//cout << "first loop" << endl;
	vector<ofPath>tempP;
	tempP.clear();
	//tempP.assign(1,tp);
	pathsShader.assign(groupByCol.size(), tempP);
	//cout << groupByCol.size() << "group" << endl;

	for (int i = 0; i < svgShadowTop->getNumPath(); i++) {

		ofPath p = svgShadowTop->getPathAt(i);
		p.setPolyWindingMode(OF_POLY_WINDING_ODD);
		ofColor colToDepth = p.getFillColor();

		if (isGray(colToDepth)) {

			for (int j = 0;j < groupByCol.size();j++) {
				if (groupByCol[j] == colToDepth) {
					pathsShader[j].push_back(p);
					//return;
				}
			}
		}
		else {
			///bool colExist = false;

		}

	}

	pathsShaderTopS = pathsShader;
	for (int i = 0; i < pathsShaderTopS.size();i++) {
		for (int j = 0; j < pathsShaderTopS[i].size();j++) {
			//ofPushMatrix();
			//ofTranslate(0, 0, zPosition[1]);//
			pathsShaderTopS[i][j].simplify(3.0);
			//pathsShaderS[i][j].draw();
			//ofPopMatrix();
		}
	}


	//Sorting the order brighter to darker
	struct less_than_key
	{
		inline bool operator() (const vector<ofPath>& struct1, const vector<ofPath>& struct2)
		{
			return (struct1[0].getFillColor().r < struct2[0].getFillColor().r);
		}
	};
	//sort(pathsShader.begin(), pathsShader.end(), less_than_key());
	sort(pathsShaderTopS.begin(), pathsShaderTopS.end(), less_than_key());
	sort(zPosition.begin(), zPosition.end()); 
	//Sorting the order brighter to darker




	for (int i = 0; i < svgShadowBack->getNumPath(); i++) {
		ofPath p = svgShadowBack->getPathAt(i);
		//cout << " back" << p.getFillColor() << endl;
		p.setPolyWindingMode(OF_POLY_WINDING_ODD);
		p.simplify(1.0);
		p.setColor(ofColor(0, 0, 0));
		pathsShaderBackS.push_back(p);
	}


	gui.setup("saved", "settingsShadow.xml");
	//gui.add(posLight.setup("posLight", ofVec3f(0, 0, 0), ofVec3f(0, 0, -1000), ofVec3f(1920 * 4, 1080, 1000.0)));
	//gui.add(lookAtLight.setup("lookAtLight", ofVec3f(0, 0, 0), ofVec3f(0, 0, -1000), ofVec3f(1920 * 4, 1080, 1000.0)));
	//gui.add(rotLight.setup("rotLight", ofVec3f(0, 0, 0), ofVec3f(-180, -180, -180), ofVec3f(180, 180, 180)));
	//gui.add(zWall.setup("zWall", 0, -500, 500));
	//gui.add(scale.setup("scale", 0.4, 0.01, 2.0));

	
	gui.add(blurFactor.setup("m_blurFactor", 4.0f, 0.0, 8.0));
	gui.add(texelSize.setup("m_texelSize", 1.0, 0.0, 10.0));
	gui.loadFromFile("settingsShadow.xml");
	

}

//--------------------------------------------------------------
/*
void shadow::update(){
	
	ofFile fragFile("shaders/basic.vert"), vertFile("shaders/gaussblur_h5.frag");
	std::filesystem::last_write_time(fragFile);
	Poco::Timestamp fragTimestamp = std::filesystem::last_write_time(fragFile); //fragFile.getPocoFile().getLastModified();
	Poco::Timestamp vertTimestamp = std::filesystem::last_write_time(vertFile);//vertFile.getPocoFile().getLastModified();
	if (fragTimestamp != lastFragTimestampPost || vertTimestamp != lastVertTimestampPost) {
		bool validShader = blurHShader.load("shaders/basic.vert", "shaders/gaussblur_h5.frag");
		if (validShader == false) { cout << "invalid shader: " << "POST" << endl; }
		//setb("validShader", validShader);
	}
	lastFragTimestampPost = fragTimestamp;
	lastVertTimestampPost = vertTimestamp;
	
	ofSetWindowTitle(ofToString(ofGetFrameRate()));

}
*/
//--------------------------------------------------------------
void shadow::drawBackShadow(ofVec2f posLight, float shadowSpread){

	pos.set(posLight);

	//ofClear(255);
	//ofBackgroundGradient(ofColor(255, 0, 0), ofColor(0, 0, 255), OF_GRADIENT_LINEAR);
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255);







	
	fbo.begin();
	ofClear(255);
	//ofClear(255);
	ofBackground(255, 255, 255);
	//cam2.begin();
	
	

	for (int i = 0; i < pathsShaderBackS.size();i++) {
	//	for (int j = 0; j < pathsShaderS[i].size();j++) {[j]
			vector<ofPolyline>poly= pathsShaderBackS[i].getOutline();
			ofVec2f pathCenter(0,0);
			for (int k = 0;k < poly.size();k++) {
				pathCenter += poly[k].getCentroid2D();
			}
			pathCenter.x = pathCenter.x / poly.size();
			pathCenter.y = pathCenter.y / poly.size();
			

			float distanceToLight = ofDist(pos.x, pos.y, pathCenter.x, pathCenter.y);
			ofVec2f pushFromOrigin = pos - pathCenter ;
			pushFromOrigin.normalize();
			pushFromOrigin.scale(-1);
			pushFromOrigin.scale(ofMap(distanceToLight, 0, 1400,0 , shadowSpread, true));
			//pushFromOrigin.x = ofMap(distanceToLight, -1.0, 1.0, 100, 0, true);
			//pushFromOrigin.y = ofMap(distanceToLight, -1.0, 1.0, 100, 0, true);

			//ofColor tCol=pathsShader[i][j].getFillColor();[j][j]

			//pathsShaderBackS[i].setColor(ofColor(0, 0, 0));

			ofPushMatrix();
			ofTranslate(pushFromOrigin.x, pushFromOrigin.y, 0);//zPosition[i]
			pathsShaderBackS[i].draw();
			ofPopMatrix();

		//	pathsShaderS[i][j].setColor(tCol);


			
		//}
		
	}
	
	//shader.end();


	//cam2.end();
	fbo.end();

	ofSetColor(255, 255, 255);

	//fbo.begin();
	

	//ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	//fbo.draw(0, 0);

	//fbo.end();

	blurHShader.begin();
	blurHShader.setUniformTexture("blurSampler", fbo.getTextureReference(0),0);
	blurHShader.setUniform1f("sigma",blurFactor);
	blurHShader.setUniform1f("blurSize", texelSize);
	//blurHShader.setUniform2f("posLight", pos.x, ofMap(pos.y, 0, ofGetHeight(), ofGetHeight(), 0));
	blurHShader.setUniform2f("posLight", pos.x, pos.y);//ofMap(pos.y, 0, ofGetHeight(), ofGetHeight(), 0));//;
	blurHShader.setUniform2f("resolution", ofGetWidth(),ofGetHeight());
	blurVShader.begin();
	blurVShader.setUniformTexture("blurSampler", fbo.getTextureReference(0), 0);
	blurVShader.setUniform1f("sigma", blurFactor);
	blurVShader.setUniform1f("blurSize", texelSize);
	//blurVShader.setUniform2f("posLight", pos.x, ofMap(pos.y, 0, ofGetHeight(), ofGetHeight(), 0));//;pos.y
	blurHShader.setUniform2f("posLight", pos.x, pos.y);
	blurVShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	fbo.draw(0, 0);
	blurVShader.end();
	blurHShader.end();







	/*
	cam2.begin();
	for (int i = 0; i < pathsShader.size();i++) {
		ofPushMatrix();
		ofTranslate(0, 0, zPosition[i]);//
		for (int j = 0; j < pathsShader[i].size();j++) {
			pathsShader[i][j].draw();
		}
		ofPopMatrix();
	}
	cam2.end();
	*/
//	

	//


	gui.draw();
}

void shadow::drawShadowTop(ofVec2f posLight, float shadowSpread,float levelsShadow)
{	

	ofEnableAlphaBlending();
	

	ofSetColor(255, 255, 255, 255);
	fbo2.begin();
		//ofClear(255, 255,255, 0);
	ofFill();	
		ofSetColor(255, 255, 255, levelsShadow);
	ofDrawRectangle(0, 0, ofGetWidth() , ofGetHeight()); // 2/ 2
	//ofBackground(255, 255, 255);
	//cam2.begin();
	
	drawShadowShape(levelsShadow, shadowSpread*0.08);
	/*
	ofSetColor(0, 0, 0, 255);
	ofDrawCircle(ofPoint((cos(ofGetElapsedTimeMillis()*0.001) * 400) + 2400, (sin(ofGetElapsedTimeMillis()*0.001) * 400) + 400), 150);
	*/
	//cam2.end();
	//cout << "pt" << ofPoint((cos(ofGetElapsedTimeMillis()*0.001) * 400) + 400, (sin(ofGetElapsedTimeMillis()) * 400) + 400) << endl;
	
	
	fbo2.end();
	ofSetColor(255, 255, 255, 255);

	//fbo2.draw(0, 0);
	//
	/*
	fbo2.begin();

	//!!!!
	//ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	fbo2.draw(0, 0);

	fbo2.end();*/
	//fbo2.draw(0, 0);
			blurHShader.begin();
	blurHShader.setUniformTexture("blurSampler", fbo2.getTextureReference(0), 0);
	blurHShader.setUniform1f("sigma", blurFactor*0.5);
	blurHShader.setUniform1f("blurSize", texelSize*0.5);
	//blurHShader.setUniform2f("posLight", pos.x,ofMap(pos.y, 0, ofGetHeight(), ofGetHeight(), 0));//;
	blurHShader.setUniform2f("posLight", pos.x, pos.y);//ofMap(pos.y, 0, ofGetHeight(), ofGetHeight(), 0));//;
	blurHShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	
	blurVShader.begin();
	blurVShader.setUniformTexture("blurSampler", fbo2.getTextureReference(0), 0);
	blurVShader.setUniform1f("sigma", blurFactor*0.5);
	blurVShader.setUniform1f("blurSize", texelSize*0.5);
	//blurVShader.setUniform2f("posLight", pos.x, ofMap(pos.y, 0, ofGetHeight(), ofGetHeight(), 0));//;pos.y
	blurVShader.setUniform2f("posLight", pos.x, pos.y);
	blurVShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	//!!!!
	//ofSetColor(255, 255, 255, levelsShadow);
	//ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	fbo2.draw(0, 0);

	blurVShader.end();
	blurHShader.end();


	/*
	for (int i = 0; i < pathsShaderTopS.size();i++) {
		for (int j = 0; j < pathsShaderTopS[i].size();j++) {
			pathsShaderTopS[i][j].draw();
		}

	}
	*/
}

void shadow::exit()
{
	gui.saveToFile("settingsShadow.xml");
}

void shadow::drawShadow(float exentricity,float exentricity2, ofColor color, int index)
{

	//for (int i = 0; i < pathsShader.size();i++) {
		for (int j = 0; j < pathsShaderTopS[index].size();j++) {
			vector<ofPolyline>poly = pathsShaderTopS[index][j].getOutline();
			ofVec2f pathCenter(0, 0);
			for (int k = 0;k < poly.size();k++) {
				pathCenter += poly[k].getCentroid2D();
			}
			pathCenter.x = pathCenter.x / poly.size();
			pathCenter.y = pathCenter.y / poly.size();
			/*
			
			
			
			TO DO CENTROID IN SHADOW;
			
			
			
			*/


			float distanceToLight = ofDist(pos.x, pos.y, pathCenter.x, pathCenter.y);
			ofVec2f pushFromOrigin = pos - pathCenter;
			pushFromOrigin.normalize();
			pushFromOrigin.scale(-1);
			pushFromOrigin.scale(ofMap(distanceToLight, 0, 1400, exentricity2, exentricity, true));
			//pushFromOrigin.x = ofMap(distanceToLight, -1.0, 1.0, 100, 0, true);
			//pushFromOrigin.y = ofMap(distanceToLight, -1.0, 1.0, 100, 0, true);

		//	ofColor tCol = pathsShaderS[index][j].getFillColor();
		//pathsShaderS[index][j].setColor(tCol);
			pathsShaderTopS[index][j].setColor(color);

			ofPushMatrix();
			ofTranslate(pushFromOrigin.x, pushFromOrigin.y, 0);//zPosition[i]
			pathsShaderTopS[index][j].draw();
			ofPopMatrix();

		//	

		}

	//}

}

void shadow::drawShadowShape(int levels,float spread)
{
	//cout << "draw on top" << endl;-1
	fadeIndex++;
	if(fadeIndex== pathsShaderTopS.size()){
		fadeIndex = 0;
	}
	int i = fadeIndex;
	//	
	//for (int i = 0;i <pathsShaderTopS.size() - 1;i++) {
	
		int index = i;

		for (int j = 0;j < pathsShaderTopS.size();j++) {

			//&& j <index + levels
			//&& j <index + (pathsShaderTopS.size()-j)
			if ( j> index ) {
				masker.beginLayer(maskLayer);
				{
					//if (i == 0) 
					//ofClear(255, 255, 255, 0); 
					//ofClear(0, 0, 0, 255);
					ofClear(0, 0, 0, 255);
			
					ofSetColor(255, 255, 255);
					ofBackground(255, 255, 255, 0);

					drawShadow(spread + (j), spread*0.5, ofColor(0, 0, 0, 235), index);//- (5 * i) ))

				}
				masker.endLayer(maskLayer);

				//Draw mask #1

				masker.beginMask(maskLayer);
				{
					ofClear(0, 0, 0, 255);
					//ofClear(255, 255, 255, 0);
					//ofClear(255, 255, 255, 255);
					//ofBackground(255, 255, 255, 1 );
					ofSetColor(255, 255, 255); 
					if (j + 1 < pathsShaderTopS.size()) {
						for (int k = 0; k < pathsShaderTopS[j + 1].size();k++) {
							pathsShaderTopS[j + 1][k].setColor(ofColor(255, 255, 255));
							pathsShaderTopS[j + 1][k].draw();
						}
					}
					
				}masker.endMask(maskLayer);
				masker.draw();
			}
		}
	//}

}


bool shadow::isGray(ofColor cGray) {
	bool gray = true;
	if (cGray.r >= cGray.g - 2 && cGray.r <= cGray.g + 2) {
	}
	else {
		gray = false;
	}

	if (cGray.g >= cGray.b - 2 && cGray.g <= cGray.b + 2) {
	}
	else {
		gray = false;
	}

	return gray;

}