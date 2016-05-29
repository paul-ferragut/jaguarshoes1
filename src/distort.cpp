#include "distort.h"

//--------------------------------------------------------------
void distort::setup(ofRectangle distortRect1, ofRectangle distortRect2) {
//

	gui.setup("distort");//, "distortSettings.xml"
	gui.add(saveFrame.setup("save", false));
	for (int i = 0;i < 3;i++) {
		gui.add(val[i].setup("val" + ofToString(i), 0.0, 0.0, 1.0));
	}
	//gui.loadFromFile("distortSettings.xml");
	
	//gui.add(fpsSave.setup("fpsSave", 5, 1, 120));

	//image.load("title.png");

	shaderPost.load("shaders/POST");
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	//fbo.allocate()
	ofEnableAntiAliasing();
	ofEnableSmoothing();


	int num = arrayLength;
	p.assign(num, particleShader());
	//currentMode = PARTICLE_MODE_ATTRACT;

	//currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";


	for (unsigned int i = 0; i < p.size(); i++) {
		//	p[i].setMode(currentMode);
		//	p[i].setAttractPoints(&attractPointsWithMovement);;
		p[i].reset(distortRect1,distortRect2);
	}
}


//--------------------------------------------------------------
void distort::begin() {

	//ofClear(255);
	//ofBackground(0, 0, 0);
	ofEnableAlphaBlending();
	fbo.begin();
	ofClear(255);
	//ofPushMatrix();
	//ofScale(1, -1, 1);

}

//--------------------------------------------------------------
void distort::end() {
	//ofPopMatrix();
	fbo.end();
}

//--------------------------------------------------------------
void distort::update(float intensity,float radiusEffect) {
	for (unsigned int i = 0; i < p.size(); i++) {
		//p[i].setMode(currentMode);
		p[i].update();
		arrayParticleSize[i] = ofMap(p[i].radius, 0, (ofGetWidth()+ofGetHeight())/2, 0, 1.0);
	}	
	

	val[0] = intensity;
	val[1] = 0.9;
	val[2] = radiusEffect;

	/*
	ofFile fragFile("POST.frag"), vertFile("POST.vert");
	std::filesystem::last_write_time(fragFile);
	Poco::Timestamp fragTimestamp = std::filesystem::last_write_time(fragFile); //fragFile.getPocoFile().getLastModified();
	Poco::Timestamp vertTimestamp = std::filesystem::last_write_time(vertFile);//vertFile.getPocoFile().getLastModified();
	if (fragTimestamp != lastFragTimestampPost || vertTimestamp != lastVertTimestampPost) {
		bool validShader = shaderPost.load("POST");
		if (validShader == false) { cout << "invalid shader: " << "POST" << endl; }
		//setb("validShader", validShader);
	}
	lastFragTimestampPost = fragTimestamp;
	lastVertTimestampPost = vertTimestamp;
	*/
	shaderPost.begin();
	//ofSetColor(255, 255, 255);

	shaderPost.setUniform1f("elapsedTime", ofGetElapsedTimeMillis()*0.00003);
	shaderPost.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	shaderPost.setUniform1f("alpha", 1.0);
	//shaderPost.setUniform2f("pos", mouseX, ofMap(mouseY, 0, ofGetHeight(), ofGetHeight(), 0));

	for (int i = 0;i < p.size();i++) {
		arrayUniform[i].x = p[i].pos.x;
		arrayUniform[i].y = ofMap(p[i].pos.y, 0, ofGetHeight(), ofGetHeight(), 0);//p[i].pos.y;//
	}
	shaderPost.setUniform2fv("posP", &arrayUniform[0].x, arrayLength);
	shaderPost.setUniform1fv("radiusP", &arrayParticleSize[0], arrayLength);
	for (int i = 0;i < 3;i++) {
		shaderPost.setUniform1f("val" + ofToString(i + 1), (float)val[i]);
	}
	shaderPost.setUniformTexture("fboTexture", fbo.getTextureReference(0), 0);

	shaderPost.end();
}

//--------------------------------------------------------------
void distort::draw() {





	//ofSetColor(255, 255, 255, 255);
	//image.draw(0, 0, ofGetWidth(), ofGetWidth());

	


	shaderPost.begin();
	ofSetColor(255, 255, 255);
	ofDisableAlphaBlending();
	fbo.draw(0, 0);
	shaderPost.end();

	//ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	




	/*
	if (saveFrame) {
		if (ofGetFrameNum() % fpsSave == 0) {

			ofSaveScreen(ofGetTimestampString() + ".png");

		}
		//saveFrame = false;
	}

	//	ofBackgroundGradient(ofColor(60, 60, 60), ofColor(10, 10, 10));
*/

}

void distort::drawDebug() {
	for (unsigned int i = 0; i < p.size(); i++) {
		p[i].draw();
	ofDrawBitmapStringHighlight(ofToString(i), p[i].pos);
	}
	
	gui.draw();
}

