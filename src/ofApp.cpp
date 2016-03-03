#include "ofApp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Timespan.h"

//--------------------------------------------------------------
void ofApp::setup(){

	int london = 26346526;
		weather.setup(london, 'c');
		weather.refresh();




		ofSetWindowTitle("ofxSunCalc Example");

		ofSetFrameRate(30);

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

		image.load("2.png");

		modelFoliage1.loadModel("3d/plantpart1b.obj");
		modelFoliage2.loadModel("3d/plantpart2b.obj");
		modelStructure.loadModel("3d/wallStudio.obj");

		fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
		shaderPost.load("shaders/POST");
		shaderTexture.load("shaders/noise");
		resolutionWidthTexture = 800;
		resolutionHeightTexture = 800;



		soundStream.printDeviceList();
		int bufferSize = 256;
		left.assign(bufferSize, 0.0);
		right.assign(bufferSize, 0.0);
		volHistory.assign(400, 0.0);
		bufferCounter = 0;
		drawCounter = 0;
		smoothedVol = 0.0;
		scaledVol = 0.0;
		soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
		ofBackground(0, 0, 0);

		cam.setLookAt(ofx2DCam::OFX2DCAM_TOP_INVERT);
		cam.setTranslation2D(ofVec2f(0.0, image.getHeight()));
		cam.setScale(2.0);
		cam.disableMouseInput();
		/*
		camTest.enableOrtho();
		camTest.setLensOffset(ofVec2f(0, 0));
		camTest.setupOffAxisViewPortal(ofVec3f(0,0,0), ofVec3f(0, ofGetHeight(), 0), ofVec3f(ofGetWidth(), ofGetHeight(), 0));
		camTest.setFarClip (2000);
		camTest.setNearClip(-1000);
		camTest.setScale(2.0, 2.0, -1);
		camTest.setAspectRatio(1.333333);
		camTest.setForceAspectRatio(false);
		camTest.setOrientation(ofVec3f(90, -180, 0));
		camTest.setPosition(0.0, image.getHeight(),0);
		ofMatrix4x4 TM = ofMatrix4x4(1, 0, 0, 0,
			0, 0, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 1);
		camTest.setTransformMatrix(TM);
		*/
	
		

		ofSetCoordHandedness(OF_RIGHT_HANDED);
		// Setup post-processing chain
		postProcessing.init(ofGetWidth(), ofGetHeight(),false);
		//postProcessing.setFlip(false);
		
		postProcessing.createPass<FxaaPass>()->setEnabled(false);
		postProcessing.createPass<BloomPass>()->setEnabled(false);
		postProcessing.createPass<DofPass>()->setEnabled(false);
		postProcessing.createPass<KaleidoscopePass>()->setEnabled(false);
		postProcessing.createPass<NoiseWarpPass>()->setEnabled(false);
		postProcessing.createPass<PixelatePass>()->setEnabled(false);
		postProcessing.createPass<EdgePass>()->setEnabled(false);
		postProcessing.createPass<VerticalTiltShifPass>()->setEnabled(false);
		postProcessing.createPass<GodRaysPass>()->setEnabled(true);
		light.setPosition(1000, 1000, 2000);

		setGUI();
}

//--------------------------------------------------------------
void ofApp::update(){

	//lets scale the vol up to a 0-1 range 
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
	//cout << "scaled vol" << scaledVol<<endl;
	//camTest.setForceAspectRatio(true);
	//camTest.setLensOffset(ofVec2f(0, 0));
	//camTest.setupOffAxisViewPortal(ofVec3f(0, 0, 0), ofVec3f(0, ofGetHeight(), 0), ofVec3f(ofGetWidth(), ofGetHeight(), 0));
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(255);
	
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255);

	if (drawBackgroundB) {
		drawBackground();
	}
	//POST FLAT
	if (usePostFlatShaderB) {
		fbo.begin();
	}//POST FLAT


	//POST 3D
	if (usePost3dShaderB) {
		drawPost3dBegin();
	}//POST 3D




	if (drawBackgroundStructureB) {
		
		drawBackgroundStructure();

	}
	
	if (drawUnderneathB){

		drawUnderneath();
	}



	if (drawOverlayImageB) {
		drawOverlayImage();
	}
	//POST 3D
	if (usePost3dShaderB) {
		drawPost3dEnd();
	}//POST 3D


	//POST FLAT
	if (usePostFlatShaderB) {
		fbo.end();
		drawPostFlatBegin();
		fbo.draw(0,0);
		drawPostFlatEnd();
	}//POST FLAT
	

	if (drawWeatherDebugB) {

		drawWeatherDebug();

	}


}

//--------------------------------------------------------------
void ofApp::drawOverlayImage() {

	ofPushStyle();
	ofPushMatrix();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(255, 255, 255, 255);
	image.draw(0, 0);
	ofDisableBlendMode();
	ofPopMatrix();
	ofPopStyle();

}

//--------------------------------------------------------------
void ofApp::drawUnderneath() {




	cam.begin();
	//camTest.begin();

	//ofEnableDepthTest();
	ofSetColor(255, 255, 255, 255);

	//ofDrawGrid(20, 10);
	//ofDisableDepthTest();

	if (useTextureB) {
		shaderTexture.begin();
		shaderTexture.setUniform1f("fluidity1", ofMap(fluidity[0], 0, 1, 0.001f, 5));
		shaderTexture.setUniform1f("fluidity2", ofMap(fluidity[1], 0, 1, 0.001f, 1));
		shaderTexture.setUniform1i("fluidity3", (int)ofMap(fluidity[2],0,1,1,12));
		shaderTexture.setUniform1f("scaleWidth", ofMap(scaleTexture,0, 1, 1, 800));//(float)resolutionWidthTexture
		shaderTexture.setUniform1f("scaleHeight", ofMap(scaleTexture, 0, 1, 1, 800));//(float)resolutionHeightTexture

		shaderTexture.setUniform1f("time", ofGetElapsedTimeMillis() *timeMotion *0.0001);
		int shaderTextureColNumUsed = 2;
		shaderTexture.setUniform1f("colorRange", shaderTextureColNumUsed);//1.0/
																  //cout << (float)1.0 / currentColorNum << "current color num" << endl;

		colorP.convertToFloatCol();
		shaderTexture.setUniform1fv("red", &colorP.rFloat[2], shaderTextureColNumUsed);//ofMap(,0,255,0.0,1.0)
		shaderTexture.setUniform1fv("green", &colorP.gFloat[2], shaderTextureColNumUsed);//ofMap(, 0, 255, 0.0, 1.0)
		shaderTexture.setUniform1fv("blue", &colorP.bFloat[2], shaderTextureColNumUsed); //start at 
	}



	modelFoliage1.drawFaces();
	modelFoliage2.drawFaces();

	if (useTextureB) {
	shaderTexture.end();
	}
	cam.end();
	//camTest.end();

}

//--------------------------------------------------------------
void ofApp::drawBackgroundStructure() {

	easyCam.begin();
	modelStructure.drawVertices();
	easyCam.end();

}

//--------------------------------------------------------------
void ofApp::drawBackground() {

	ofBackgroundGradient(colorP.getCol(0), colorP.getCol(1), OF_GRADIENT_LINEAR);

}

//--------------------------------------------------------------
void ofApp::drawPost3dBegin() {


	glPushAttrib(GL_ENABLE_BIT);

	// setup gl state
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
	light.enable();
	// begin scene to post process easyCam
	postProcessing.begin();

	


}

//--------------------------------------------------------------
void ofApp::drawPost3dEnd() {

	//shaderPost.end();
	// set gl state back to original
	postProcessing.end();
	glPopAttrib();

	// draw help
	ofSetColor(0, 255, 255);
	ofDrawBitmapString("Number keys toggle effects, mouse rotates scene", 10, 20);
	for (unsigned i = 0; i < postProcessing.size(); ++i)
	{
		if (postProcessing[i]->getEnabled()) ofSetColor(0, 255, 255);
		else ofSetColor(255, 0, 0);
		ostringstream oss;
		oss << i << ": " << postProcessing[i]->getName() << (postProcessing[i]->getEnabled() ? " (on)" : " (off)");
		ofDrawBitmapString(oss.str(), 10, 20 * (i + 2));
	}
}

//--------------------------------------------------------------
void ofApp::drawPostFlatBegin() {



	shaderPost.begin();

	ofSetColor(255, 255, 255);
	//validShaderPost = reloadShader(&shaderPost, &lastVertPostTimestamp, &lastFragPostTimestamp, "shaders/post/", &shaderPostStyle, shaderSelectionPostString);
	shaderPost.begin();
	shaderPost.setUniform1f("elapsedTime", ofGetElapsedTimeMillis());
	shaderPost.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	shaderPost.setUniform1f("alpha", opacityShader);

	if (usePostWithSoundB) {

	uniformFloatShader[2] = scaledVol;
	uniformFloatShader[3] = scaledVol;

	}

	for (int i = 0;i < VAR_SHADER;i++) {
	shaderPost.setUniform1f("val" + ofToString(i + 1), uniformFloatShader[i]);
	}
	shaderPost.setUniformTexture("fboTexture", fbo.getTextureReference(0), 0);
	ofSetColor(255, 255, 255);
	ofDisableAlphaBlending();
	
}

//--------------------------------------------------------------
void ofApp::drawPostFlatEnd() {
	shaderPost.end();
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
	if (showWeather) {
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
	unsigned idx = key - '0';
	if (idx < postProcessing.size()) postProcessing[idx]->setEnabled(!postProcessing[idx]->getEnabled());
}

//--------------------------------------------------------------
void ofApp::exit() {
	gui1->saveSettings("settings1.xml");
	gui2->saveSettings("settings2.xml");
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
	gui1 = new ofxUISuperCanvas("JAGUARSHOES");
	gui1->addSpacer();
	gui1->addFPS();
	gui1->addFPSSlider("FPS");
	gui1->addLabel("'h' to Hide GUI", OFX_UI_FONT_SMALL);
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
	gui1->addSlider("alpha shader",0.0, 1.0, &opacityShader);

	for (int i = 0;i < VAR_SHADER;i++) {
		gui1->addSlider("val" + ofToString(i + 1), 0.0, 1.0, &uniformFloatShader[i]);
	}

	//gui->addToggle("show weather", &showWeather);


	gui1->addToggle("drawWeatherDebugB", &drawWeatherDebugB);


	gui1->addToggle("drawOveralyImageB", &drawOverlayImageB);


	gui1->addToggle("drawUnderneathB", &drawUnderneathB);
	gui1->addToggle("useTextureB", &useTextureB);

	gui1->addSlider("fluidity 1", 0, 1.0, &fluidity[0]);
	gui1->addSlider("fluidity 2", 0, 1.0, &fluidity[1]);
	gui1->addSlider("fluidity 3", 0, 1.0, &fluidity[2]);
	gui1->addSlider("time", 0, 1.0, &timeMotion);
	gui1->addSlider("scale texture", 0, 1.0, &scaleTexture);

	gui1->addToggle("useLightsB", &useLightsB);



	gui1->addToggle("drawBackgroundStructureB", &drawBackgroundStructureB);
	gui1->addToggle("structureToDustB", &structureToDustB);


	gui1->addToggle("drawBackgroundB", &drawBackgroundB);






	gui1->addToggle("usePostFlatShaderB", &usePostFlatShaderB);
	gui1->addToggle("usePost3dShaderB", &usePost3dShaderB);
	gui1->addToggle("usePostWithSoundB", &usePostWithSoundB);




	gui1->autoSizeToFitWidgets();
	ofAddListener(gui1->newGUIEvent, this, &ofApp::guiEvent);

	gui1->loadSettings("settings1.xml");

	gui1->setPosition(0, 0);


	gui2 = new ofxUISuperCanvas("COLORS");
	gui2->addSpacer();
	gui2->addLabel("'h' to Hide GUI", OFX_UI_FONT_SMALL);
	for (int i = 0;i < COLOR_IN_PALETTE;i++) {
		gui2->addSlider("red " + ofToString(i), 0, 255, &colorP.r[i]);
		gui2->addSlider("green " + ofToString(i), 0, 255, &colorP.g[i]);
		gui2->addSlider("blue " + ofToString(i), 0, 255, &colorP.b[i]);
		gui2->addSpacer();
	}
	gui2->autoSizeToFitWidgets();
	ofAddListener(gui2->newGUIEvent, this, &ofApp::guiEvent);

	gui2->loadSettings("settings2.xml");

	gui2->setPosition(gui1->getGlobalCanvasWidth(), 0);


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