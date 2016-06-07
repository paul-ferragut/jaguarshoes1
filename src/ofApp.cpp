#include "ofApp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Timespan.h"

//--------------------------------------------------------------
void ofApp::setup(){

		ofSetWindowTitle("Jaguarshoes");
		ofSetFrameRate(40);
		ofSetSmoothLighting(true);
		//ofEnableSmoothing();

		roomImage.load("map-transparent.png");
		roomImageDebug.load("debug.png");
		cout << "image loaded" << endl;


		soundStream.printDeviceList();

		int bufferSize = 256;

		left.assign(bufferSize, 0.0);
		right.assign(bufferSize, 0.0);
		volHistory.assign(400, 0.0);

		bufferCounter = 0;
		drawCounter = 0;
		smoothedVol = 0.0;
		scaledVol = 0.0;

		// 0 output channels, 
		// 2 input channels
		// 44100 samples per second
		// 256 samples per buffer
		// 4 num buffers (latency)
#ifdef USE_SAVESCREEN
		saveFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 8);
		player.load("mp3/interference.mp3"); 
		player.setLoop(true);
		//player.play();

		fftSmoothed = new float[8192];
		for (int i = 0; i < 8192; i++) {
			fftSmoothed[i] = 0;
		}

		nBandsToGet = 128;
		//int sampleRate = 44100;//player.getSampleRate();
		//ofSoundStreamSetup(1, 0, this, sampleRate, bufferSize, 4);
#else  		
		soundStream.setDeviceID(6);		
		soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
#endif  

		cout << "sound set" << endl;

		ofBackground(0, 0, 0);
		// ----
		_mapping = new ofxMtlMapping2D();
		_mapping->init(ofGetWidth(), ofGetHeight(), "mapping/xml/shapes.xml", "mapping/controls/mapping.xml");
		cout << "mapping set" << endl;

		setGUI();
		cout << "gui set" << endl;
		color.setup();
		voro.setup();

#ifdef  USE_STRUCT
		pSystemLeft[0].init("3d/newcGardenDoor.obj", "settingsUI/particles1.xml", "3d/gardenDoor0", "3d/gardenDoor1", 150000);
		cout << "loading 3d." << endl;
		pSystemLeft[1].init("3d/newcStructure.obj", "settingsUI/particles4.xml", "3d/structure0", "3d/structure1", 150000);
		cout << "loading 3d.." << endl;
		pSystemLeft[2].init("3d/newcGardenWall.obj", "settingsUI/particles3.xml", "3d/gardenWall0", "3d/gardenWall0", 150000);
		cout << "loading 3d..." << endl;
		pSystemRight[0].init("3d/newcCorridor.obj", "settingsUI/particles2.xml", "3d/corridor0", "3d/corridor1", 150000);
		cout << "loading 3d...." << endl;
		pSystemRight[1].init("3d/newcCrane.obj", "settingsUI/particles5.xml", "3d/crane0", "3d/crane1", 250000);
		cout << "loading 3d....." << endl;
		currentPSystemLeft = floor(ofRandom(0,3));
		currentPSystemRight = floor(ofRandom(0, 2));
#endif //  USE_STRUCT

		svgDrawing.setup("illu2.svg");
		cout << "svg shapes loaded" << endl;

		shadowBack = new ofxSVG;
		shadowBack->load("shadowBack.svg");
		cout << "svg shadow back loaded" << endl;

		shadowFront = new ofxSVG;
		shadowFront->load("shadowFront.svg");
		cout << "svg front shadow loaded" << endl;



		for (int i = 0;i < LIGHT_NUM;i++) {
			light[i].setup(ofRectangle(-ofGetWidth() / 2, -ofGetHeight() / 2, ofGetWidth() * 2, ofGetHeight() * 2), ofRectangle(-ofGetWidth()/2, -ofGetHeight() / 2, ofGetWidth()*2, ofGetHeight()*2), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofVec2f(ofRandom(0.008, 0.012), ofRandom(0.008, 0.012)),ofRandom(150,250));
			light[i].lightShadow.setup(shadowFront, shadowBack);
			light[i].useShadow = true;//useShadowB;
			light[i].useShadowTop = true;//useTopShadowB;
			lightIndex1[i] = ofRandom(0, 4);
			lightIndex2[i] = ofRandom(0, 4);
			cout << "TEST" << ofRandom(0, 4) << endl;
		}

		



		//light[1].setup(ofRectangle(-400, -400, ofGetWidth() + 400, ofGetHeight() + 400), ofRectangle(-ofGetWidth() / 2, -ofGetHeight() / 2, ofGetWidth() * 2, ofGetHeight() * 2), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofRandom(150, 250),true);
		//light[1].lightShadow.setup(shadowFront, shadowBack);
		//light[1].useShadow=true;
		//light[1].useShadowTop = true;
		//light[2].setup(ofRectangle(-400, -400, ofGetWidth() + 400, ofGetHeight() + 400), ofRectangle(-ofGetWidth() / 2, -ofGetHeight() / 2, ofGetWidth() * 2, ofGetHeight() * 2), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofRandom(150, 250), true);
		//light[3].setup(ofRectangle(-400, -400, ofGetWidth() + 400, ofGetHeight() + 400), ofRectangle(-ofGetWidth() / 2, -ofGetHeight() / 2, ofGetWidth() * 2, ofGetHeight() * 2), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofRandom(150, 250));
		//light[4].setup(ofRectangle(-400, -400, ofGetWidth() + 400, ofGetHeight() + 400), ofRectangle(-ofGetWidth() / 2, -ofGetHeight() / 2, ofGetWidth() * 2, ofGetHeight() * 2), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofVec2f(ofRandom(0.008, 0.022), ofRandom(0.008, 0.022)), ofRandom(150, 250), true);

		cout << "light" << endl;
		distortI.setup(ofRectangle(ofGetWidth() / 2, 0, ofGetWidth() / 2, ofGetHeight()), ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
		
		cam.disableMouseInput();

		cout << "distort set" << endl;
		
		glowingShapes.setup("glowShapes.svg");
		cout << "glow set" << endl;
		
		svgShapeSimple.setup("flatShapes.svg");
		cout << "flat set" << endl;

		branchesFlower[0].setup(ofVec2f(437,328),100*1.75);
		branchesFlower[1].setup(ofVec2f(740, 155), 70 * 1.75);
		branchesFlower[2].setup(ofVec2f(4, 455), 110 * 1.75);
		branchesFlower[3].setup(ofVec2f(22, 562), 70 * 1.75);
		cout << "flowers set" << endl;
}

//--------------------------------------------------------------
void ofApp::update(){

#ifdef USE_SAVESCREEN

#else
		scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
#endif
		//lets record the volume into an array
		volHistory.push_back(scaledVol);

		//if we are bigger the the size we want to record - lets drop the oldest value
		if (volHistory.size() >= 400) {
			volHistory.erase(volHistory.begin(), volHistory.begin() + 1);
		}

	if (useSoundB) {

#ifdef USE_SAVESCREEN

		ofSoundUpdate();
		float * val = ofSoundGetSpectrum(nBandsToGet);		// request 128 values for fft
		for (int i = 0;i < nBandsToGet; i++) {

			// let the smoothed calue sink to zero:
			fftSmoothed[i] *= 0.96f;

			// take the max, either the smoothed or the incoming:
			if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];

		}
		scaledVol = 0;
		for (int i = 0;i < nBandsToGet; i++) {
			// (we use negative height here, because we want to flip them
			// because the top corner is 0,0)
			scaledVol +=(fftSmoothed[i] * 200);
		}
		scaledVol = scaledVol / nBandsToGet;
		scaledVol = ofMap(scaledVol, 0, 50, 0.0, 1.0, true);
		//cout << "scaled vol" << scaledVol<< endl;
		if (scaledVol - soundAverage > 0.25) {
			peak = true;
		}
		else {
			peak = false;
		}

#else
		soundAverage = 0;
		if(volHistory.size()>averageDuration){
			int average = averageDuration;
		for (int i = volHistory.size()- (average);i < volHistory.size();i++) {
			soundAverage += volHistory[i];
		}
			soundAverage = soundAverage / average;
			//cout << "soundAverage" << soundAverage << " current" << scaledVol << endl;
		}

		if (scaledVol - soundAverage > 0.075) {
			peak = true;
		}
		else {
			peak = false;
		}
#endif
	}
	
	if (peak == false) {
		peakCounter = 0;
	}
		//peakCounter -=1;
		//if (peakCounter < 0) { 
		//	peakCounter = 0; 
		//}
	//}

	if (peak==true) {
		peakCounter++;
	}
		

	if (peakCounter > 3) {
		peak = false;
	}
	

	color.update(simulatedTime,useSimulatedTimeColorB);

	if (usePostShaderB){
		if (useSoundB) { 
			postIntensity = scaledVol;
			postRadius = (scaledVol*0.2);
		}
	distortI.update(postIntensity, postRadius);
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
		if (useSoundB) { 
			voro.update(peak); 
		}
		else {
			voro.update(false);
		}
	}
	
	for (int i = 0;i < LIGHT_NUM;i++) {
		if (useLightB[i]) {
			light[i].useShadowTop = useTopShadowB[i];
			light[i].useShadow = useShadowB;
			light[i].update();	
		}
	}

	_mapping->update();



	if (drawGenerativeFlowersB) {
		for (int i = 0;i < 4;i++) {
			branchesFlower[i].update();
		}
	}

	if (drawPulsingShapesB) {
		glowingShapes.update(light[0].getLookAt());
	}



}

//--------------------------------------------------------------
void ofApp::draw(){
	//ofBackground(0);

	if (useMappingB) {
	_mapping->bind();
	}

	ofClear(255);

#ifdef USE_SAVESCREEN
	if (saveScreenB) {
		saveFbo.begin();
	} //|| saveScreenVideoB
	if (saveScreenVideoB) {
		if (player.isPlaying() == false) {
			player.play();
		}
		saveScreenVideoB = false;
	}

#endif


	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255);

	if (usePostShaderB) {
		distortI.begin();
	}

	ofSetGlobalAmbientColor(colorP[2]);

	if (drawBackgroundB) {
		drawBackground();
	}

		if (drawStructureB) {
#ifdef USE_STRUCT
		pSystemRight[currentPSystemRight].draw(colorP[4], colorP[3]);
		pSystemLeft[currentPSystemLeft].draw(colorP[4], colorP[3]);
#endif //USE_STRUCT
	}

	if (use2DCamB) {
		cam.begin();
	}
	else {
		easyCam.begin();
	}

	
	if (drawVoroB) {
		ofSetColor(colorP[4]);
		voro.draw();
	}	

	if (drawFlatShapesB) {
		if (useSoundB) {
			svgShapeSimple.draw(colorP[0], colorP[1],scaledVol);
		}
		else {
			svgShapeSimple.draw(colorP[0], colorP[1]);
		}
		
	}

	ofEnableDepthTest();

	for (int i = 0;i < LIGHT_NUM;i++) {
		if (useLightB[i] && light[i].useShadow) {
			light[i].drawShadowBack(shadowSpread[i]);
		}
	}
	
	ofSetColor(255, 255, 255, 255);

	bool lightIsUsed = false;
	for (int i = 0;i < LIGHT_NUM;i++) {
		if (useLightB[i]) {
			lightIsUsed = true;
		}
	}

	if (lightIsUsed) {
		ofEnableLighting();
	}

	if (useMaterialB) {
		material.begin();
		material.setShininess(shininess);
		material.setSpecularColor(colorP[2]);
	}
	
	for (int i = 0;i < LIGHT_NUM;i++) {
		if (useLightB[i]) {
			light[i].setColor(colorP[lightIndex1[i]], colorP[lightIndex2[i]], concentration[i], cutoff[i]);
			light[i].setZ(lightZ[i]);
			light[i].begin();
		}
	}

	if (drawIllustrationB) {
		svgDrawing.draw(color.getFloatColors(),zExtrusionShapes);
	}

	for (int i = 0;i < LIGHT_NUM;i++) {
		if (useLightB[i]) {	
			light[i].end();
		}
	}

	if (useMaterialB) {
		material.end();
	}

	if (drawGenerativeFlowersB) {
		for (int i = 0;i < 4;i++) {
			branchesFlower[i].draw(colorP[2], colorP[0]);
		}
	}

	if (lightIsUsed) { ofDisableLighting(); }

	ofDisableDepthTest();

	if (drawPulsingShapesB) {
		glowingShapes.draw(peak, colorP[2]);
	}

	if (use2DCamB) {
		cam.end();
	}
	else {
		easyCam.end();
	}

	for (int i = 0;i < LIGHT_NUM;i++) {
		if (useLightB[i] && light[i].useShadowTop) {
			light[i].drawShadowTop(shadowSpread[i]);
		}
	}

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
		_mapping->unbind();
		ofSetColor(255, 255, 255, 255);
		_mapping->draw();
	}

	#ifdef USE_WEATHER
		if (drawWeatherDebugB) {
			drawWeatherDebug();
		}
	#endif

	if (gui2->isVisible()) {
		ofFill(); //&& saveScreenVideoB==false&& saveScreenB==false
		for (int i = 0;i < 5;i++) {
			ofSetColor(colorP[i]);
			ofDrawRectangle(colDebugRect.x, colDebugRect.y + (i*colDebugRect.height), colDebugRect.width, colDebugRect.height);
		}
		vector<ofFloatColor>fct = color.getFloatColors();
		for (int i = 0;i < 5;i++) {
			ofSetColor(fct[i]);
			ofDrawRectangle(colDebugRect.x - 40, colDebugRect.y + (i*colDebugRect.height), colDebugRect.width, colDebugRect.height);
		}
	}	

	if (drawDebugB) {
		distortI.drawDebug();
		for (int i = 0;i < LIGHT_NUM;i++) {
			if (useLightB[i]) {
				light[i].drawDebug();
			}
		}
		

	}

#ifdef USE_STRUCT
	if (showGuiPSystemRightB) {
		pSystemRight[currentPSystemRight].drawGui();
	}

	if (showGuiPSystemLeftB) {
		pSystemLeft[currentPSystemLeft].drawGui();
	}
#endif //USE_STRUCT

#ifdef USE_SAVESCREEN
	if (saveScreenB) {
		saveFbo.end();//||saveScreenVideoB
	}
	if (saveScreenB) {
		ofPixels pixels;
		saveFbo.readToPixels(pixels);
		ofImage img;
		img.allocate(saveFbo.getWidth(), saveFbo.getHeight(), OF_IMAGE_COLOR);
		img.setFromPixels(pixels);
		img.draw(0, 0);
		img.saveImage("screencapture/" + ofGetTimestampString() + ".png");
		saveScreenB = false;
	}

	if (saveScreenVideoB) {
		/*
		ofPixels pixels;
		saveFbo.readToPixels(pixels);
		ofImage img;
		img.allocate(saveFbo.getWidth(), saveFbo.getHeight(), OF_IMAGE_COLOR);
		img.setFromPixels(pixels);
		img.draw(0, 0);
		img.saveImage("screencapture/video/" + ofToString(ofGetFrameNum()) + ".png");
		*/
	}
#endif

}

//--------------------------------------------------------------
void ofApp::drawBackground() {

	ofBackgroundGradient(colorP[0], colorP[1], OF_GRADIENT_LINEAR);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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

	//GUI 1
	gui1 = new ofxUISuperCanvas("MAIN");
	gui1->addSpacer();
	gui1->addFPS();
	gui1->addLabel("'h' to Hide GUI", OFX_UI_FONT_SMALL);
	gui1->addLabel("'m' to show mapping", OFX_UI_FONT_SMALL);
	gui1->addSpacer();

	gui1->addToggle("useMappingB", &useMappingB);	
	gui1->addLabel("Camera", OFX_UI_FONT_SMALL);
	gui1->addToggle("use2DCamB", &use2DCamB);
	gui1->addToggle("flipYB", &flipYB);
	gui1->addSpacer();

	gui1->addLabel("Visual components", OFX_UI_FONT_SMALL);
	gui1->addToggle("drawBackgroundB", &drawBackgroundB);
	gui1->addToggle("drawStructureB", &drawStructureB);
	gui1->addToggle("drawVoroB", &drawVoroB);
	gui1->addToggle("drawFlatShapesB", &drawFlatShapesB);
	gui1->addToggle("drawIllustrationB", &drawIllustrationB);
	gui1->addToggle("drawPulsingShapesB", &drawPulsingShapesB);
	gui1->addToggle("drawGenerativeFlowersB", &drawGenerativeFlowersB);
	gui1->addToggle("usePostShaderB", &usePostShaderB);
	gui1->addSlider("postRadius", 0.0, 1.0, &postRadius);
	gui1->addSlider("postIntensity", 0.0, 1.0, &postIntensity);
	gui1->addToggle("drawRoomB", &drawRoomB);
	gui1->addSpacer();

	gui1->addLabel("Debug", OFX_UI_FONT_SMALL);
	gui1->addToggle("drawRoomDebugB", &drawRoomDebugB);
	gui1->addToggle("showGuiPSystemLeftB", &showGuiPSystemLeftB);
	gui1->addToggle("showGuiPSystemRightB", &showGuiPSystemRightB);
	#ifdef USE_WEATHER
	gui1->addToggle("drawWeatherDebugB", &drawWeatherDebugB);
	#endif
	gui1->addToggle("drawDebugB", &drawDebugB);
	
#ifdef USE_SAVESCREEN
	gui1->addSpacer();
	gui1->addToggle("saveScreenB", &saveScreenB);
	gui1->addToggle("saveScreenVideoB", &saveScreenVideoB);
#endif

	gui1->addSpacer();

	gui1->addLabel("Sound", OFX_UI_FONT_SMALL);
	gui1->addToggle("useSoundB", &useSoundB);
	gui1->addSlider("smoothedVol", 0.0, 0.2, &smoothedVol);
	gui1->addSlider("scaledVol", 0.0, 1.0, &scaledVol);
	gui1->addSlider("averageDuration", 0.0, 300.0, &averageDuration);
	gui1->addToggle("peak", &peak);
	gui1->addSpacer();

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

	
	//GUI 3
	gui3 = new ofxUISuperCanvas("LIGHT");
	gui3->addSpacer();
	gui3->addSlider("zExtrusionShapes" , 0.01, 2.0, &zExtrusionShapes);
	gui3->addSlider("shininess", -75, 75, &shininess);//-125, 125
	gui3->addToggle("useMaterial" , &useMaterialB);
	gui3->addSpacer();
	for (int i = 0;i <LIGHT_NUM;i++) {
		gui3->addToggle("useLightB" + ofToString(i), &useLightB[i]);
		gui3->addSlider("cutoff" + ofToString(i), 0, 90, &cutoff[i]);
		gui3->addSlider("concentration" + ofToString(i), 0, 128, &concentration[i]);
		gui3->addSlider("lightZ" + ofToString(i), -200.0, 200.0, &lightZ[i]);
		gui3->addToggle("useShadowB" + ofToString(i), &useShadowB[i]);
		gui3->addToggle("useTopShadowB" + ofToString(i), &useTopShadowB[i]);
		gui3->addSlider("shadow spread" + ofToString(i), 5.0, 200.0, &shadowSpread[i]);
		gui3->addSpacer();
	}
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

	//for (int i = 0; i < bufferSize; i++) sampleBuffer[i] = input[i];
	//sampleBuffer[i] = sample.update();
	//player.update();
	

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

//--------------------------------------------------------------
#ifdef USE_WEATHER
void ofApp::setupWeather() {
	int london = 26346526;
	weather.setup(london, 'c');
	weather.refresh();


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

}

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

#endif

//--------------------------------------------------------------