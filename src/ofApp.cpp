#include "ofApp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Timespan.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetWindowTitle("Jaguarshoes");
	ofSetFrameRate(30);
	ofSetSmoothLighting(true);

	int london = 26346526;
		weather.setup(london, 'c');
		weather.refresh();





		/*
		Poco::LocalDateTime now;

		date_str = Poco::DateTimeFormatter::format(now, "%Y-%m-%d %H:%M:%S");
		ofLogNotice("NOW") << date_str;
		//ofLogNotice("local tzd") << now.tzd();

		lat = 51.547492; // Note southern degrees need to be - (not like those from google maps)
		lon = -0.0107809; //


		latlon_str = "lat:" + ofToString(lat) + ", lon:" + ofToString(lon);

		ofLogNotice("today") << sun_calc.dateToString(now);

		SunCalcPosition sunpos = sun_calc.getSunPosition(now, lat, lon);

		pos_str = "altitude=" + ofToString(sunpos.altitude) + ", azimuth=" + ofToString(sunpos.azimuth * RAD_TO_DEG);

		ofLogNotice("sunpos") << pos_str;

		todayInfo = sun_calc.getDayInfo(now, lat, lon, true);

		min_info_str = sun_calc.infoToString(todayInfo, true);
		max_info_str = sun_calc.infoToString(todayInfo, false);
		ofLogNotice() << min_info_str << endl << endl << max_info_str;

		//small_font.loadFont(OF_TTF_MONO, 8, false);

		Poco::LocalDateTime sixMonthsAgo = now - Poco::Timespan(30 * 6, 0, 0, 0, 0);
		Poco::LocalDateTime threeMonthsAgo = now - Poco::Timespan(30 * 3, 0, 0, 0, 0);
		Poco::LocalDateTime threeMonthsInFuture = now + Poco::Timespan(30 * 3, 0, 0, 0, 0);

		labels.push_back("6 months ago\n" + ofxSunCalc::dateToDateString(sixMonthsAgo));
		labels.push_back("3 months ago\n" + ofxSunCalc::dateToDateString(threeMonthsAgo));
		labels.push_back("Today\n" + ofxSunCalc::dateToDateString(now));
		labels.push_back("3 months time\n" + ofxSunCalc::dateToDateString(threeMonthsInFuture));


		vector<SunCalcDayInfo> sun_infos;

		sun_infos.push_back(sun_calc.getDayInfo(sixMonthsAgo, lat, lon, false));
		sun_infos.push_back(sun_calc.getDayInfo(threeMonthsAgo, lat, lon, false));
		sun_infos.push_back(todayInfo);
		sun_infos.push_back(sun_calc.getDayInfo(threeMonthsInFuture, lat, lon, false));

		// create/draw a timeline for each date
		for (int i = 0; i<4; i++) {
			timelines.push_back(ofFbo());
			timelines[i].allocate(ofGetWidth() - 20 - 110, 32);
			ofxSunCalc::drawSimpleDayInfoTimeline(timelines[i], sun_infos[i]);
		}
		*/

		cout << "img load" << endl;

		roomImage.load("map-transparent.png");
		roomImageDebug.load("debug.png");
		//roomImageDebug.load("map-transparent.png");


		/*
		ofFbo::Settings fboSettings;
		fboSettings.useDepth = true;
		fboSettings.useStencil = true;
		fboSettings.depthStencilAsTexture = true;
		
		fboSettings.internalformat = GL_RGBA;
		fboSettings.width = ofGetWidth();
		fboSettings.height = ofGetHeight();
		fbo.allocate(fboSettings);
		shaderPost.load("shaders/POST");
		shaderTexture.load("shaders/noise");
		shaderGodrays.load("shaders/godray");
		resolutionWidthTexture = 800;
		resolutionHeightTexture = 800;
		*/
		// 0 output channels, 
		// 2 input channels
		// 44100 samples per second
		// 256 samples per buffer
		// 4 num buffers (latency)

		soundStream.printDeviceList();

		//if you want to set a different device id 
		//soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.

		int bufferSize = 256;


		left.assign(bufferSize, 0.0);
		right.assign(bufferSize, 0.0);
		volHistory.assign(400, 0.0);

		bufferCounter = 0;
		drawCounter = 0;
		smoothedVol = 0.0;
		scaledVol = 0.0;
		soundStream.setDeviceID(5);
		soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
		

		ofBackground(0, 0, 0);



		// ----
		_mapping = new ofxMtlMapping2D();
		_mapping->init(ofGetWidth(), ofGetHeight(), "mapping/xml/shapes.xml", "mapping/controls/mapping.xml");

		setGUI();
		cout << "after gui" << endl;
		color.setup();
		voro.setup();

#ifdef  USE_STRUCT
		pSystemLeft[0].init("3d/newcGardenDoor.obj", "settingsUI/particles1.xml", "3d/gardenDoor0", "3d/gardenDoor1", 150000);
		cout << "loading." << endl;
		pSystemLeft[1].init("3d/newcStructure.obj", "settingsUI/particles4.xml", "3d/structure0", "3d/structure1", 150000);
		cout << "loading.." << endl;
		pSystemLeft[2].init("3d/newcGardenWall.obj", "settingsUI/particles3.xml", "3d/gardenWall0", "3d/gardenWall0", 150000);
		cout << "loading..." << endl;
		pSystemRight[0].init("3d/newcCorridor.obj", "settingsUI/particles2.xml", "3d/corridor0", "3d/corridor1", 150000);
		cout << "loading...." << endl;
		pSystemRight[1].init("3d/newcCrane.obj", "settingsUI/particles5.xml", "3d/crane0", "3d/crane1", 250000);
		cout << "loading....." << endl;
		currentPSystemLeft = floor(ofRandom(0,3));
		currentPSystemRight = floor(ofRandom(0, 2));
#endif //  USE_STRUCT


		//pSystem1[0].visibleGuiB = true;
		svgDrawing.setup("illu2.svg");


		shadowBack = new ofxSVG;
		shadowBack->load("shadowBack.svg");
		//shadowBack2.load("illu2.svg");
		cout << "svg1" << endl;
		shadowFront = new ofxSVG;
		shadowFront->load("shadowFront.svg");
		cout << "svg2" << endl;

		//*/
		
		//shadowBack.load("illuShadowBack.svg");
		
		//shadowFront.load("illuShadowTop.svg");
		//cout << "svg3" << endl;
		for (int i = 0;i < LIGHT_NUM;i++) {
			light[i].setup(ofRectangle(-ofGetWidth() / 2, -ofGetHeight() / 2, ofGetWidth() * 2, ofGetHeight() * 2), ofRectangle(-ofGetWidth()/2, -ofGetHeight() / 2, ofGetWidth()*2, ofGetHeight()*2), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofVec2f(ofRandom(0.008, 0.012), ofRandom(0.008, 0.012)),ofRandom(150,250));
		
		}

		light[0].lightShadow.setup(shadowFront, shadowBack);
		light[0].useShadow = true;
		light[0].useShadowTop = true;
		//light[1].setup(ofRectangle(-400, -400, ofGetWidth() + 400, ofGetHeight() + 400), ofRectangle(-ofGetWidth() / 2, -ofGetHeight() / 2, ofGetWidth() * 2, ofGetHeight() * 2), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofRandom(150, 250),true);
		light[1].lightShadow.setup(shadowFront, shadowBack);
		light[1].useShadow=true;
		light[1].useShadowTop = true;
		//light[2].setup(ofRectangle(-400, -400, ofGetWidth() + 400, ofGetHeight() + 400), ofRectangle(-ofGetWidth() / 2, -ofGetHeight() / 2, ofGetWidth() * 2, ofGetHeight() * 2), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofRandom(150, 250), true);
		//light[3].setup(ofRectangle(-400, -400, ofGetWidth() + 400, ofGetHeight() + 400), ofRectangle(-ofGetWidth() / 2, -ofGetHeight() / 2, ofGetWidth() * 2, ofGetHeight() * 2), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofRandom(150, 250));
		//light[4].setup(ofRectangle(-400, -400, ofGetWidth() + 400, ofGetHeight() + 400), ofRectangle(-ofGetWidth() / 2, -ofGetHeight() / 2, ofGetWidth() * 2, ofGetHeight() * 2), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofRandom(150, 250), true);

		cout << "light" << endl;
		distortI.setup(ofRectangle(ofGetWidth() / 2, 0, ofGetWidth() / 2, ofGetHeight()), ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));

		cout << "distort" << endl;
		cam.disableMouseInput();

		cout << "glow" << endl;
		glowingShapes.setup("glowShapes.svg");
		
		cout << "flat" << endl;
		svgShapeSimple.setup("flatShapes.svg");


		branchesFlower[0].setup(ofVec2f(437,328),100*1.75);
		branchesFlower[1].setup(ofVec2f(740, 155), 70 * 1.75);
		branchesFlower[2].setup(ofVec2f(4, 455), 110 * 1.75);
		branchesFlower[3].setup(ofVec2f(22, 562), 70 * 1.75);

}

//--------------------------------------------------------------
void ofApp::update(){

	color.update(simulatedTime,useSimulatedTimeColorB);

	if (usePostShaderB){
		//cout << "update1" << endl;
		if (useSoundB) { 
			postIntensity = scaledVol;
			postRadius = (scaledVol*0.2);
			 }
distortI.update(postIntensity, postRadius);
	//cout << "update2" << endl;
	}

	if (drawStructureB) {
#ifdef USE_STRUCT
		bool nextRight=pSystemRight[currentPSystemRight].update();
		if (nextRight == false) {
			currentPSystemRight++;
			if (currentPSystemRight == 2) {
				currentPSystemRight = 0;
			}
		}
		bool nextLeft = pSystemLeft[currentPSystemLeft].update();
		if (nextLeft == false) {
			currentPSystemLeft++;
			if (currentPSystemLeft == 3) {
				currentPSystemLeft = 0;
			}
		}
#endif //USE_STRUCT
	}

	if(useTimeColorB){
		for (int i = 0;i < COLOR_IN_PALETTE;i++) {
			colorP[i] = color.getColor(i);
			rC[i] = colorP[i].r;
			gC[i] = colorP[i].g;
			bC[i] = colorP[i].b;
		}
	}
	else {
		for (int i = 0;i < COLOR_IN_PALETTE;i++) {
			colorP[i].set(rC[i], gC[i], bC[i]);
			color.setColor(colorP[i], i);
		}
	}

	if (drawVoroB) {
		if (useSoundB) { voro.update(peak); }
		else {
			voro.update(false);
		}
		//voro.update(peak);
	}
	
	//
	//cout << "PRINT COLORS" << endl << endl;
	//for (int i = 0;i < NUM_COLORS_IN_PALETTE;i++) {
		
		//cout << color.getColor(i) << endl;

	//}
	
	//lets scale the vol up to a 0-1 range 
	//scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
	//cout << "scaled vol" << scaledVol<<endl;
	//camTest.setForceAspectRatio(true);
	//camTest.setLensOffset(ofVec2f(0, 0));
	//camTest.setupOffAxisViewPortal(ofVec3f(0, 0, 0), ofVec3f(0, ofGetHeight(), 0), ofVec3f(ofGetWidth(), ofGetHeight(), 0));

	for (int i = 0;i < LIGHT_NUM;i++) {
		if (useLightB[i]) {
		
			light[i].useShadowTop = useTopShadowB[i];
			light[i].update();
			
		}
	}
	
	/**/
	//if (structureToDustB) {
	//	vboParticles->update();
	//}

	_mapping->update();


	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);

	//lets record the volume into an array
	volHistory.push_back(scaledVol);

	//if we are bigger the the size we want to record - lets drop the oldest value
	if (volHistory.size() >= 400) {
		volHistory.erase(volHistory.begin(), volHistory.begin() + 1);
	}


	soundAverage = 0;
	if(volHistory.size()>averageDuration){
		int average = averageDuration;
	for (int i = volHistory.size()- (average);i < volHistory.size();i++) {
		soundAverage += volHistory[i];
	}
	soundAverage = soundAverage / average;
		cout << "soundAverage" << soundAverage << " current" << scaledVol << endl;
	}

	if (scaledVol - soundAverage > 0.075) {
		peak = true;
	}
	else {
		peak = false;
	}
	//bool ;


	for (int i = 0;i < 4;i++) {
		branchesFlower[i].update();
	}

	glowingShapes.update(light[0].getLookAt());

	ofSetWindowTitle("fps:"+ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
	//ofBackground(0);

	if (useMappingB) {
	_mapping->bind();
	}

	ofClear(255);

	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255);

	//ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	if (usePostShaderB) {
		distortI.begin();
	}
	else {
		//distortI.fbo.getTextureReference().getTextureData().bFlipTexture = false;
	}
	ofSetGlobalAmbientColor(colorP[2]);

	if (drawBackgroundB) {
		drawBackground();
	}

		if (drawStructureB) {
#ifdef USE_STRUCT
		pSystemRight[currentPSystemRight].draw(colorP[4], colorP[2]);
		pSystemLeft[currentPSystemLeft].draw(colorP[4], colorP[2]);
#endif //USE_STRUCT
	}

	if (use2DCamB) {
		cam.begin();

	}
	else {
		easyCam.begin();
	}
	//ofScale(1, -1, 1);





	
	if (drawVoroB) {
		ofSetColor(colorP[4]);
		voro.draw();
	}	

	svgShapeSimple.draw(colorP[0], colorP[1]);

	ofEnableDepthTest();





	for (int i = 0;i < LIGHT_NUM;i++) {
		if (useLightB[i] && light[i].useShadow) {
			light[i].drawShadowBack(shadowSpread[i]);
		}
	}

	//if (useLightB) {
	//	light.begin(true);
	//}
	//ofDisableBlendMode();
	
	ofSetColor(255, 255, 255, 255);

	bool lightIsUsed = false;
	for (int i = 0;i < LIGHT_NUM;i++) {
		if (useLightB[i]) {
			lightIsUsed = true;
		}
	}
	if (lightIsUsed) {
		
		ofEnableLighting();
	//cout << "enable light" << endl;
	}
	if (useMaterialB) {
		material.begin();
		material.setShininess(shininess);
		material.setSpecularColor(colorP[2]);
	}
	
	light[0].setColor(colorP[2], colorP[3]);
	light[1].setColor(colorP[2], colorP[3]);
	light[2].setColor(colorP[3], colorP[4]);
	light[3].setColor(colorP[2], colorP[0]);
	light[4].setColor(colorP[4], colorP[1]);
	for (int i = 0;i < LIGHT_NUM;i++) {
		if (useLightB[i]) {
			light[i].setLightType((int)lightType[i]);
			light[i].setZ(lightZ[i]);
			
			light[i].begin();
		}
	}


	//ofScale(1, 1, 1);
	if (drawIllustrationB) {
	//	vector<ofFloatColor>colPass=color.getFloatColors();
		//cout << "color1" << colPass[0] << endl;
		//cout << "color2" << colPass[1] << endl;

		svgDrawing.draw(color.getFloatColors(),zExtrusionShapes);
	//	ofDisableDepthTest();
	}




	for (int i = 0;i < LIGHT_NUM;i++) {
		if (useLightB[i]) {
			
			light[i].end();
		}
	}




	if (useMaterialB) {
		material.end();
	}

	for (int i = 0;i < 4;i++) {
		branchesFlower[i].draw(colorP[2], colorP[0]);
	}

	if (lightIsUsed) { ofDisableLighting(); }



	ofDisableDepthTest();

	//shadowBack->draw();
	//shadowBack2.draw();
	//light[0].lightShadow.svgShadowBack->draw();
	//light[0].lightShadow.svgShadowTop->draw();
	
	//ofDisableDepthTest();



	if (use2DCamB) {
		cam.end();
	}
	else {
		easyCam.end();
	}

	//ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	for (int i = 0;i < LIGHT_NUM;i++) {
		if (useLightB[i] && light[i].useShadowTop) {
			//cout << "shadow Top" << endl;
			light[i].drawShadowTop(shadowSpread[i],topShadowResolution[i]);
		}
	}
	//ofDisableBlendMode();

	glowingShapes.draw(peak, colorP[2]);



	if (usePostShaderB) {
		distortI.end();

		if (flipYB) {
		ofPushMatrix();
		ofScale(1,-1,1);
		ofTranslate(0, -ofGetHeight());
		}

		distortI.draw();
		if (flipYB) {
			ofPopMatrix();
		}
		
	}



	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, 255);
	if (drawRoomB) {
		roomImage.draw(0, 0);
	}

	if (drawRoomDebugB) {
		roomImageDebug.draw(0, 0);
	}

	if (useMappingB) {
	
		if (drawDebugGridB) {
			_mapping->grid();
		}
	//_mapping->blend(_mapping->getMaskShapes());
		_mapping->unbind();

	
	//-------- mapping of the towers/shapes
		ofSetColor(255, 255, 255, 255);
		_mapping->draw();
	}

	if (drawWeatherDebugB) {
		drawWeatherDebug();
	}

	if (gui1->isVisible()) {
		for (int i = 0;i < LIGHT_NUM;i++) {
			if (useLightB[i]) {
				light[i].drawDebug();
			}
		}
		ofFill();
		for (int i = 0;i < 5;i++) {
			ofSetColor(colorP[i]);
			ofDrawRectangle(colDebugRect.x, colDebugRect.y + (i*colDebugRect.height), colDebugRect.width, colDebugRect.height);
		}
		vector<ofFloatColor>fct=color.getFloatColors();
		for (int i = 0;i < 5;i++) {
			ofSetColor(fct[i]);
			ofDrawRectangle(colDebugRect.x-40, colDebugRect.y + (i*colDebugRect.height), colDebugRect.width, colDebugRect.height);
		}

		distortI.drawDebug();
		
	}

#ifdef USE_STRUCT
	if (showGuiPSystemRightB) {
	pSystemRight[currentPSystemRight].drawGui();
	}

	if (showGuiPSystemLeftB) {
		pSystemLeft[currentPSystemLeft].drawGui();
	}
#endif //USE_STRUCT
}

//--------------------------------------------------------------
/*
void ofApp::drawOverlayImage() {


}
void ofApp::drawUnderneathShadows() {

}
//--------------------------------------------------------------
void ofApp::drawUnderneath() {

}


//--------------------------------------------------------------
void ofApp::drawBackgroundStructure() {

}
*/
//--------------------------------------------------------------
void ofApp::drawBackground() {

	ofBackgroundGradient(colorP[0], colorP[1], OF_GRADIENT_LINEAR);

}
/*

//--------------------------------------------------------------
void ofApp::drawPostFlatBegin() {
	
}

//--------------------------------------------------------------
void ofApp::drawPostFlatEnd() {
}

//--------------------------------------------------------------
void ofApp::drawLightBegin() {

}

//--------------------------------------------------------------

void ofApp::drawLightEnd() {

}
*/
//--------------------------------------------------------------
void ofApp::drawWeatherDebug() {

	Poco::LocalDateTime now; //(2015,5,29,17,38);

	if (ofGetKeyPressed(OF_KEY_ALT)) {
		// auto step the time of day to proof changes
		int total_min = fabs(sin(ofGetElapsedTimef()*.05)) * 1440; // 1440 == mins per day  60 * 24
		int hr = floor(total_min / 60.0);
		int mn = total_min - (hr * 60); //now.minute();
		now.assign(now.year(), now.month(), now.day(), hr, mn);
	}

	float sun_brightness = ofxSunCalc::getSunBrightness(todayInfo, now);

	if (ofGetKeyPressed(OF_KEY_COMMAND)) {
		sun_brightness = fabs(sin(ofGetElapsedTimef()*.1));
	}

	// draw background gradient based on sun_brightness

	ofColor nightBG(ofColor::black);
	ofColor nightFG(64);

	ofColor dayBG(ofColor::skyBlue);
	ofColor dayFG(ofColor::paleGoldenRod);

	ofColor background = nightBG.lerp(dayBG, sun_brightness);
	ofColor foreground = nightFG.lerp(dayFG, sun_brightness);

	//	ofBackgroundGradient(foreground, background);
	if (drawWeatherDebugB) {
		ofDrawBitmapStringHighlight(date_str, 15, 20, ofColor::paleGoldenRod, ofColor::black);

		ofDrawBitmapStringHighlight(min_info_str, 15, 45, ofColor::salmon, ofColor::white);

		ofDrawBitmapStringHighlight(max_info_str, 15, 125, ofColor::darkorange, ofColor::white);

		ofDrawBitmapStringHighlight(latlon_str, 180, 20, ofColor::gold, ofColor::black);

		ofDrawBitmapStringHighlight(pos_str, 180, 45, ofColor::cornsilk, ofColor::black);

		ofDrawBitmapStringHighlight("Current Brightness " + ofToString(sun_brightness, 3), 180, 70, ofColor::goldenRod, ofColor::white);

		float tx = 10 + 110;
		float ty = 320;
		for (int i = 0; i<timelines.size(); i++) {

			ofSetColor(255);
			timelines[i].draw(tx, ty);

			ofDrawBitmapStringHighlight(labels[i], 10, ty + 13);

			if (i == 2) { // today
				ofNoFill();
				ofSetLineWidth(1.0);
				ofSetColor(255);
				ofRect(tx, ty, timelines[i].getWidth(), timelines[i].getHeight());

				// Draw a current time mark
				float pixels_per_min = (timelines[i].getWidth() / 24) / 60.0;
				float nx = tx + pixels_per_min * (now.hour() * 60 + now.minute());
				ofSetColor(255, 0, 0);
				ofSetLineWidth(2.0);
				ofLine(nx, ty, nx, ty + timelines[i].getHeight());
			}

			ty += timelines[i].getHeight() + 25;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == '1') {
	
		weather.resetWOEID(26346526);
		//ofLogNotice("WOEID = london");
		cout << weather.getTemperature() << endl;
		cout << weather.getHumidity() << endl;
	}


	if (key == ' ') {

		if (bUseEasyCam) {
			easyCam.disableMouseInput();
			cam.enableMouseInput();
		}
		else {
			easyCam.enableMouseInput();
			cam.disableMouseInput();
		}

		bUseEasyCam ^= true;
	}
	if (key == 'a') {
		cam.setLookAt(ofx2DCam::OFX2DCAM_LEFT);
	}
	else if (key == 's') {
		cam.setLookAt(ofx2DCam::OFX2DCAM_FRONT);
	}
	else if (key == 'd') {
		cam.setLookAt(ofx2DCam::OFX2DCAM_RIGHT);
	}
	else if (key == 'w') {
		cam.setLookAt(ofx2DCam::OFX2DCAM_TOP);
	}
	else if (key == 'q') {
		cam.setLookAt(ofx2DCam::OFX2DCAM_TOP_INVERT);
	}
	else if (key == 'x') {
		cam.setLookAt(ofx2DCam::OFX2DCAM_BOTTOM);
	}
	else if (key == 'z') {
		cam.setLookAt(ofx2DCam::OFX2DCAM_BACK);
	}


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	//unsigned idx = key - '0';
//	if (idx < postProcessing.size()) postProcessing[idx]->setEnabled(!postProcessing[idx]->getEnabled());

	tLensoffset = !tLensoffset;


	if (key == 'h'){
		gui1->toggleVisible();
		gui2->toggleVisible();
		gui3->toggleVisible();
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	gui1->saveSettings("settings1.xml");
	gui2->saveSettings("settings2.xml");
	gui3->saveSettings("settings3.xml");
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e) {

}


//--------------------------------------------------------------
void ofApp::setGUI() {
	gui1 = new ofxUISuperCanvas("MAIN");
	gui1->addSpacer();
	gui1->addFPS();
	gui1->addLabel("'h' to Hide GUI", OFX_UI_FONT_SMALL);
	gui1->addLabel("'m' to show mapping", OFX_UI_FONT_SMALL);
	/*
	gui1->addSlider("positionCamera x",  -1000, 1000, &positionCamera.x);
	gui1->addSlider("positionCamera y",  -1000, 1000, &positionCamera.y);
	gui1->addSlider("positionCamera z",  -5000, 5000,&positionCamera.z);
	gui1->addSlider("rotation x", -180, 180, &rotationCamera.x);
	gui1->addSlider("rotation y", -180, 180, &rotationCamera.y);
	gui1->addSlider("rotation z", -180, 180, &rotationCamera.z);
	gui1->addSlider("fov", 0, 100, &cameraFov);
	gui1->addSlider("scale", 0.01, 10, &modelFoliageScale);
	*/
	//opacityShader = 1.0;
	//gui1->addSlider("drawingScale", 0.1, 3.0, &drawingScale);
	gui1->addSlider("smoothedVol", 0.0, 0.2, &smoothedVol);
	gui1->addSlider("scaledVol", 0.0, 1.0, &scaledVol);

	gui1->addSlider("averageDuration", 0.0, 300.0, &averageDuration);
	gui1->addToggle("peak", &peak);


	gui1->addToggle("useMappingB", &useMappingB);
	gui1->addToggle("use2DCamB", &use2DCamB);
	gui1->addToggle("flipYB", &flipYB);

	//gui1->addToggle("useLightB", &useLightB);
	gui1->addToggle("drawDebugGridB", &drawDebugGridB);
	gui1->addToggle("drawRoomB", &drawRoomB);
	gui1->addToggle("drawRoomDebugB", &drawRoomDebugB);
	gui1->addToggle("drawBackgroundB", &drawBackgroundB);
	gui1->addToggle("drawStructureB", &drawStructureB);
	gui1->addToggle("showGuiPSystemLeftB", &showGuiPSystemLeftB);
	gui1->addToggle("showGuiPSystemRightB", &showGuiPSystemRightB);
	//gui1->addToggle("drawStructureB", &drawStructureB);
//	gui1->addSlider("structure X", -3840, 3840, &pSystem1X);
	gui1->addToggle("drawVoroB", &drawVoroB);
	gui1->addToggle("useSoundB", &useSoundB);
	gui1->addToggle("drawIllustrationB", &drawIllustrationB);
	gui1->addToggle("drawWeatherDebugB", &drawWeatherDebugB);
	gui1->addToggle("usePostShaderB", &usePostShaderB);
	gui1->addSlider("postRadius", 0.0, 1.0, &postRadius);
	gui1->addSlider("postIntensity", 0.0, 1.0, &postIntensity);

	//gui1->addSlider("x", -1, 1, &x);
	//gui1->addSlider("y", -1, 1, &y);
	//gui1->addSlider("z", -1, 1, &z);

	
	//float postRadius;
	//float postIntensity;


	gui1->addSpacer();
	//gui1->addToggle("drawOverlayImageB", &drawOverlayImageB);
	//gui1->addSpacer();
	//gui1->addToggle("drawUnderneathB", &drawUnderneathB);
	//gui1->addSpacer();
	//gui1->addLabel("Texture", OFX_UI_FONT_SMALL);
	//gui1->addToggle("useTextureB", &useTextureB);
	//gui1->addSlider("fluidity 1", 0, 1.0, &fluidity[0]);
	//gui1->addSlider("fluidity 2", 0, 1.0, &fluidity[1]);
	//gui1->addSlider("fluidity 3", 0, 1.0, &fluidity[2]);
	//gui1->addSlider("time", 0, 1.0, &timeMotion);
	//gui1->addSlider("scale texture", 0, 1.0, &scaleTexture);
	//gui1->addSpacer();
	//gui1->addLabel("Shadow", OFX_UI_FONT_SMALL);
	//gui1->addToggle("useDirectShadowB", &drawUnderneathShadowsB);
	//gui1->addSlider("shadow offset x", -1000, 1000, &shadowOffset.x);
	//gui1->addSlider("shadow offset y", -1000, 1000, &shadowOffset.y);
	//gui1->addSlider("shadow offset z", -1000, 1000, &shadowOffset.z);





	//gui1->addSpacer();
	//gui1->addLabel("Distortions", OFX_UI_FONT_SMALL);
	
	//gui1->addSlider("alpha shader", 0.0, 1.0, &opacityShader);

	//for (int i = 0;i < VAR_SHADER;i++) {
	//	gui1->addSlider("val" + ofToString(i + 1), 0.0, 1.0, &uniformFloatShader[i]);
	//}
	//gui1->addToggle("usePostWithSoundB", &usePostWithSoundB);

	//gui1->addToggle("usePost3dShaderB", &usePost3dShaderB);
	
	//gui1->addSpacer();
	//gui1->addLabel("Mapping", OFX_UI_FONT_SMALL);

	gui1->autoSizeToFitWidgets();
	ofAddListener(gui1->newGUIEvent, this, &ofApp::guiEvent);

	gui1->loadSettings("settings1.xml");

	gui1->setPosition(1920- gui1->getGlobalCanvasWidth(), 0);


	gui2 = new ofxUISuperCanvas("COLORS");
	gui2->addSpacer();
	gui2->addToggle("useTimeColorB", &useTimeColorB);
	gui2->addToggle("useSimulatedTimeColorB", &useSimulatedTimeColorB);
	gui2->addSlider("simulatedTime " , 0, 100000, &simulatedTime);
	for (int i = 0;i < COLOR_IN_PALETTE;i++) {
		gui2->addLabel("Color " + ofToString(i), OFX_UI_FONT_SMALL);
		gui2->addSlider("red " + ofToString(i), 0, 255, &rC[i]);
		gui2->addSlider("green " + ofToString(i), 0, 255, &gC[i]);
		gui2->addSlider("blue " + ofToString(i), 0, 255, &bC[i]);
		gui2->addSpacer();
	}

	gui2->autoSizeToFitWidgets();
	ofAddListener(gui2->newGUIEvent, this, &ofApp::guiEvent);

	gui2->loadSettings("settings2.xml");

	gui2->setPosition(1920 - (gui1->getGlobalCanvasWidth()*2), 0);

	int widthColRect = 40;
	colDebugRect.set(gui2->getRect()->x - widthColRect, 0, widthColRect, 100);
	//
	gui3 = new ofxUISuperCanvas("LIGHT");
	gui3->addSpacer();
	gui3->addSlider("zExtrusionShapes" , 0.01, 2.0, &zExtrusionShapes);
	gui3->addSlider("shininess", -125, 125, &shininess);
	gui3->addToggle("useMaterial" , &useMaterialB);

	gui3->addSpacer();
	//gui2->addLabel("'h' to Hide GUI", OFX_UI_FONT_SMALL);
	for (int i = 0;i <LIGHT_NUM;i++) {
		
		gui3->addToggle("use light" + ofToString(i), &useLightB[i]);
		gui3->addToggle("useTopShadowB" + ofToString(i), &useTopShadowB[i]);
		//gui3->addToggle("use light" + ofToString(i), &useLightB[i]);
		gui3->addSlider("topShadowResolution" + ofToString(i), 0, 10, &topShadowResolution[i]);
		gui3->addSlider("light type " + ofToString(i), 0, 3.0, &lightType[i]);
		gui3->addSlider("shadow spread" + ofToString(i), 5.0, 200.0, &shadowSpread[i]);
		gui3->addSlider("lightZ" + ofToString(i), -200.0, 200.0, &lightZ[i]);


		bool useTopShadowB[LIGHT_NUM];
		int topShadowResolution[LIGHT_NUM];

		gui3->addSpacer();

	}


	//gui3->addToggle("drawLightDebugB", &drawLightDebugB);
	gui3->autoSizeToFitWidgets();
	ofAddListener(gui3->newGUIEvent, this, &ofApp::guiEvent);

	gui3->loadSettings("settings3.xml");

	gui3->setPosition(1920 - ((gui1->getGlobalCanvasWidth() * 3)+ widthColRect), 0);

	
}


//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels) {

	float curVol = 0.0;

	// samples are "interleaved"
	int numCounted = 0;

	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (int i = 0; i < bufferSize; i++) {
		left[i] = input[i * 2] * 0.5;
		right[i] = input[i * 2 + 1] * 0.5;

		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted += 2;
	}

	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;

	// this is how we get the root of rms :) 
	curVol = sqrt(curVol);

	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;

	bufferCounter++;

}