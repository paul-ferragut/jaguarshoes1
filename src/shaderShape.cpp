#include "shaderShape.h"

shaderShape::shaderShape()
{

}

//--------------------------------------------------------------
shaderShape::~shaderShape()
{

}

//--------------------------------------------------------------
void shaderShape::setup(string shaderString,string settingsVars, vector<ofPath>p) {
		//, vector<float>v
	saveSettingsString = settingsVars;
	gui.setup("settings shaders", saveSettingsString);

	for (int i = 0;i < SHADER_PARAM_NUM;i++) {
		gui.add(vars[i].set("vars" + ofToString(i),0.5, 0.0, 1.0));
	}
	gui.add(saveGuiB.set("saveGuiB", false));

	gui.loadFromFile(saveSettingsString);
	shader.load(shaderString);
	paths = p;

	for (int i = 0;i < paths.size();i++) {
		paths[i].setColor(ofColor(255, 255, 255));
		uniqueTime.push_back(ofRandom(0, 5000));
	}
	//fColors = floatColors;
	uniqueTimeGroup = ofRandom(0, 5000);
}



//--------------------------------------------------------------
void shaderShape::draw(bool differentTime, vector<ofFloatColor>floatColors){
	
	/*
	ofFile fragFile("shaders/noise.frag"), vertFile("shaders/noise.vert");
	std::filesystem::last_write_time(fragFile);
	Poco::Timestamp fragTimestamp = std::filesystem::last_write_time(fragFile); //fragFile.getPocoFile().getLastModified();
	Poco::Timestamp vertTimestamp = std::filesystem::last_write_time(vertFile);//vertFile.getPocoFile().getLastModified();
	if (fragTimestamp != lastFragTimestampPost || vertTimestamp != lastVertTimestampPost) {
		bool validShader = shader.load("shaders/noise");
		if (validShader == false) { cout << "invalid shader: " << "POST" << endl; }
		//setb("validShader", validShader);
	}
	lastFragTimestampPost = fragTimestamp;
	lastVertTimestampPost = vertTimestamp;
	*/

	if (saveGuiB) {
		saveGui();
		saveGuiB = false;
	}

	//fColors = floatColors;

	//vector<float>redF;
	//vector<float>greenF;
	//vector<float>blueF;

	//floatColors.push_back(ofFloatColor())
	for (int i = 0;i < floatColors.size();i++) {
		redF[i]=floatColors[i].r;
		greenF[i]=floatColors[i].g;
		blueF[i]=floatColors[i].b;

		//cout << "color" << i << " " << floatColors[i] << endl;
	}
	//cout <<  endl;
	//cout << endl;

	redF[5] = redF[0];
	greenF[5] = greenF[0];
	blueF[5] = blueF[0];


	/*
	shaderTexture.begin();
	shaderTexture.setUniform1f("fluidity1", ofMap(fluidity[0], 0, 1, 0.001f, 5));
	shaderTexture.setUniform1f("fluidity2", ofMap(fluidity[1], 0, 1, 0.001f, 1));
	shaderTexture.setUniform1i("fluidity3", (int)ofMap(fluidity[2], 0, 1, 1, 12));
	shaderTexture.setUniform1f("scaleWidth", ofMap(scaleTexture, 0, 1, 1, 800));//(float)resolutionWidthTexture
	shaderTexture.setUniform1f("scaleHeight", ofMap(scaleTexture, 0, 1, 1, 800));//(float)resolutionHeightTexture
	shaderTexture.setUniform1f("time", ofGetElapsedTimeMillis() *timeMotion *0.0001);
	*/

	//cout << (float)1.0 / currentColorNum << "current color num" << endl;
	//TO DO INPUT THE COLOR IN THE SHADER
	//shaderTexture.setUniform1fv("red", &colorP.rFloat[2], shaderTextureColNumUsed);//ofMap(,0,255,0.0,1.0)
	//shaderTexture.setUniform1fv("green", &colorP.gFloat[2], shaderTextureColNumUsed);//ofMap(, 0, 255, 0.0, 1.0)
	//shaderTexture.setUniform1fv("blue", &colorP.bFloat[2], shaderTextureColNumUsed); //start at 

	int shaderTextureColNumUsed = floatColors.size();
	//cout << "col Num" << shaderTextureColNumUsed << endl;
			shader.begin();
			shader.setUniform1f("fluidity1", ofMap(vars[0], 0, 1, 0.001f, 5));
			shader.setUniform1f("fluidity2", ofMap(vars[1], 0, 1, 0.001f, 1));
			shader.setUniform1i("fluidity3", (int)ofMap(vars[2], 0, 1, 1, 12));
			shader.setUniform1f("scaleWidth", ofMap(vars[3], 0, 1, 1, 800));//(float)resolutionWidthTexture
			shader.setUniform1f("scaleHeight", ofMap(vars[3], 0, 1, 1, 800));//(float)resolutionHeightTexture
			shader.setUniform1f("time", (ofGetElapsedTimeMillis() *vars[4] *0.0001) + uniqueTimeGroup);//+ uniqueTime[i]
			shader.setUniform1f("colorNumber", shaderTextureColNumUsed);
			shader.setUniform1fv("colR", &redF[0],6);//ofMap(,0,255,0.0,1.0)
			shader.setUniform1fv("colG", &greenF[0],6);//ofMap(, 0, 255, 0.0, 1.0)
			shader.setUniform1fv("colB", &blueF[0],6);
	if (differentTime) {
		for (int i = 0;i < paths.size();i++) {
			shader.setUniform1f("time", (ofGetElapsedTimeMillis() *vars[4] * 0.0001) + uniqueTime[i]);
			paths[i].draw();
		}

	}
	else {
		for (int i = 0;i < paths.size();i++) {
			paths[i].draw();
		}
	}
	shader.end();
	//gui.draw();
}

void shaderShape::saveGui(){

	gui.saveToFile(saveSettingsString);

}
