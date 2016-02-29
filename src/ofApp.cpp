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
		shader.load("shaders/POST");



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

		setGUI();
}

//--------------------------------------------------------------
void ofApp::update(){
	//lets scale the vol up to a 0-1 range 
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
	//cout << "scaled vol" << scaledVol<<endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(255);
	
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
	ofEnableAlphaBlending();
	//

	ofSetColor(255, 255, 255);

//	fbo.begin();

	ofBackground(0, 0, 0);

//	camEasy.begin();
//	modelStructure.drawVertices();
//	camEasy.end();

	cam.setTranslation2D(ofVec2f(positionCamera.x, positionCamera.y));
//	cam.setOrientation(rotationCamera);

	if (bUseEasyCam) {
		easyCam.begin();
	}
	else {
		cam.begin();
	}
ofEnableDepthTest();
	//camStatic.enableOrtho();
	//camStatic.setPosition(positionCamera);
	//camStatic.setOrientation(rotationCamera);
	
	//camStatic.setFov(cameraFov);
	
	//camStatic.begin();
	//ofPushMatrix();	
	//ofTranslate(positionCamera);
	//ofScale(modelFoliageScale, modelFoliageScale, modelFoliageScale);

		//ofTranslate(-modelFoliage1.getSceneCenter().x / 2, -modelFoliage1.getSceneCenter().y / 2);//* 0.3528 * 0.3528

	//ofRotateX(rotationCamera.x);
	//ofRotateY(rotationCamera.y);
	//ofRotateZ(rotationCamera.z);

	//	ofTranslate(modelFoliage1.getSceneCenter().x / 2 , modelFoliage1.getSceneCenter().y / 2 );//* 0.3528* 0.3528	
	modelFoliage1.drawFaces();
	modelFoliage2.drawFaces();
	//ofPopMatrix();
	//camStatic.end();
	//ofDisableDepthTest();


	ofDrawGrid(20, 10);
	ofDisableDepthTest();
	if (bUseEasyCam) {
		easyCam.end();
	}
	else {
		cam.end();
	}

	ofPushMatrix();
	//ofScale(modelFoliageScale,modelFoliageScale,modelFoliageScale);
	//ofEnableBlendMode(OF_BLENDMODE_ADD);
	image.draw(0,0);
	//ofDisableBlendMode();
	ofPopMatrix();
	

//	fbo.end();

	shader.begin();

	ofSetColor(255, 255, 255);
	//validShaderPost = reloadShader(&shaderPost, &lastVertPostTimestamp, &lastFragPostTimestamp, "shaders/post/", &shaderPostStyle, shaderSelectionPostString);

	shader.begin();
	shader.setUniform1f("elapsedTime", ofGetElapsedTimef());
	shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	shader.setUniform1f("alpha", opacityShader);
	
	uniformFloatShader[2] = scaledVol;
	uniformFloatShader[3] = scaledVol;

	for (int i = 0;i < VAR_SHADER;i++) {
		shader.setUniform1f("val" + ofToString(i + 1), uniformFloatShader[i]);
	}




	shader.setUniformTexture("fboTexture", fbo.getTextureReference(0), 0);
	ofSetColor(255, 255, 255, 255);
	//fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
//	
	shader.end();


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

}

//--------------------------------------------------------------
void ofApp::exit() {
	gui->saveSettings("settings.xml");
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
	gui = new ofxUISuperCanvas("JAGUARSHOES Test 3");
	gui->addSpacer();
	gui->addLabel("'h' to Hide GUI", OFX_UI_FONT_SMALL);

	gui->addSlider("positionCamera x",  -1000, 1000, &positionCamera.x);
	gui->addSlider("positionCamera y",  -1000, 1000, &positionCamera.y);
	gui->addSlider("positionCamera z",  -5000, 5000,&positionCamera.z);

	gui->addSlider("rotation x", -180, 180, &rotationCamera.x);
	gui->addSlider("rotation y", -180, 180, &rotationCamera.y);
	gui->addSlider("rotation z", -180, 180, &rotationCamera.z);

	gui->addSlider("fov", 0, 100, &cameraFov);

	gui->addSlider("scale", 0.01, 10, &modelFoliageScale);

	opacityShader = 1.0;
	gui->addSlider("alpha shader",0.0, 1.0, &opacityShader);

	for (int i = 0;i < VAR_SHADER;i++) {
		gui->addSlider("val" + ofToString(i + 1), 0.0, 1.0, &uniformFloatShader[i]);
	}


	gui->addToggle("show weather", &showWeather);
	/*
	gui->addSpacer();
	gui1->addSlider("RED", 0.0, 255.0, &red)->setTriggerType(OFX_UI_TRIGGER_ALL);
	gui1->addSlider("GREEN", 0.0, 255.0, &green)->setTriggerType(OFX_UI_TRIGGER_BEGIN | OFX_UI_TRIGGER_CHANGE | OFX_UI_TRIGGER_END);
	gui1->addSlider("BLUE", 0.0, 255.0, &blue)->setTriggerType(OFX_UI_TRIGGER_BEGIN | OFX_UI_TRIGGER_CHANGE);

	gui1->addSpacer();
	gui1->addLabel("V SLIDERS");
	gui1->addSlider("0", 0.0, 255.0, 150, 17, 160);
	gui1->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	gui1->addSlider("1", 0.0, 255.0, 150, 17, 160);
	gui1->addSlider("2", 0.0, 255.0, 150, 17, 160);
	gui1->addSlider("3", 0.0, 255.0, 150, 17, 160);
	gui1->addSlider("4", 0.0, 255.0, 150, 17, 160);
	gui1->addSlider("5", 0.0, 255.0, 150, 17, 160);
	gui1->addSlider("6", 0.0, 255.0, 150, 17, 160);
	gui1->addSlider("7", 0.0, 255.0, 150, 17, 160);
	gui1->addSlider("8", 0.0, 255.0, 150, 17, 160);
	gui1->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

	gui1->addSpacer();
	gui1->addRadio("RADIO HORIZONTAL", names, OFX_UI_ORIENTATION_HORIZONTAL);
	gui1->addRadio("RADIO VERTICAL", names, OFX_UI_ORIENTATION_VERTICAL);

	gui1->addSpacer();
	gui1->setWidgetFontSize(OFX_UI_FONT_SMALL);
	gui1->addButton("BUTTON", false);
	gui1->addToggle("TOGGLE", false);

	gui1->addSpacer();
	gui1->addLabel("RANGE SLIDER");
	gui1->addRangeSlider("RSLIDER", 0.0, 255.0, 50.0, 100.0);

	string textString = "This widget is a text area widget. Use this when you need to display a paragraph of text. It takes care of formatting the text to fit the block.";
	gui1->addSpacer();

	gui1->addTextArea("textarea", textString, OFX_UI_FONT_SMALL);
	*/
	gui->autoSizeToFitWidgets();
	ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);

	gui->loadSettings("settings.xml");
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