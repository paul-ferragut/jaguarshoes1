//
//  ParticleSystemGPU.h
//  ParticlesGPU
//
//  Created by Andreas Müller on 11/01/2015.
//
//

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
//#include "ofxAutoReloadedShader.h"
#include "ofxCameraSaveLoad.h"
#include "ofxAssimpModelLoader.h"

#include "Math/MathUtils.h"
#include "Utils/FboPingPong.h"


#define PARTICLEPHASE_INACTIVE 0
#define PARTICLEPHASE_INVISIBLE 1
#define PARTICLEPHASE_CONSTRUCT 2
#define PARTICLEPHASE_DESTRUCT 3


class ParticleSystemSpawnTexture
{

	public:
	//ofEasyCam * _camera,
		ParticleSystemSpawnTexture();
		~ParticleSystemSpawnTexture();

		void init(string meshLocation, string settingsFile, string cam1,string cam2, float  cameraSpeed=60000,int _texSize = 128);// ofVboMesh vboMesh, int instanceNum);
		void initNew(string meshLocation, string settingsFile, string cam1, string cam2, float  cameraSpeed = 6000, int _texSize = 128);// ofVboMesh vboMesh, int instanceNum);

		void updateTime( float _time, float _timeStep );
		bool update();

		void drawMesh();
		void draw(ofColor col1,ofColor col2);
		
		void saveCamera(string saveString);
		void loadCamera(string camera1, string camera2, float durationMotion);

		void updateParticles( float _time, float _timeStep );
		void drawParticles(ofShader* _shader);
	
		void drawGui();

		void reset();

		void saveSettings(string saveS);
		void loadSettings(string loadS);
	
		int						numParticles;
		int						textureSize;
	
		FboPingPong				particleDataFbo;
	
		ofFloatPixels			spawnPosBuffer;
		
		ofTexture				spawnPosTexture;

		ofFloatPixels			spawnStartBuffer;
		ofTexture				spawnStartTexture;
	
		ofVboMesh				singleParticleMesh;

		ofVboMesh				mesh;
		vector<bool>usedVertices;
	
		ofLight					light[1];
		ofMaterial				particleMaterial;
	
		ofShader 	particleUpdate;
		ofShader 	particleDraw;
	

		ofxPanel				gui;
		ofParameter<float>		particleMaxAge;
		ofParameter<float>		meshScale;
		ofParameter<float>		noisePositionScale;
		ofParameter<float>		noiseMagnitude;
		ofParameter<float>		noiseTimeScale;
		ofParameter<float>		noisePersistence;
		ofParameter<float>		twistNoiseTimeScale;
		ofParameter<float>		twistNoisePosScale;
		ofParameter<float>		twistMinAng;
		ofParameter<float>		twistMaxAng;
		ofParameter<float>		spawnStartPercentage;
		

		ofParameter<bool>		startConstruct;
		ofParameter<bool>		startDestruct;
		ofParameter<int>			increaseSpawn;


		ofParameter<ofVec3f>	baseSpeed;
		
		ofParameter<ofColor>	startColor;
		ofParameter<ofColor>	endColor;
	

		ofxPanel				guiLightAndMaterial;
	
		ofParameter<ofFloatColor>	globalAmbient;

		ofParameter<ofColor>	light1Ambient;
		ofParameter<ofColor>	light1Diffuse;
		ofParameter<ofColor>	light1Specular;

		//ofParameter<ofColor>	materialDiffuse; // We will provide our own diffuse per particle
		ofParameter<ofColor>	materialAmbient;
		ofParameter<ofColor>	materialSpecular;
		ofParameter<ofColor>	materialEmissive;

	//	ofParameter<ofVec3f>	cameraPosition;
	//	ofParameter<ofVec3f>	cameraRotation;
	//	ofParameter<ofVec3f>	cameraRotInvert;
	
		ofParameter<float>		materialShininess;

		ofEasyCam camera;

		float time;
		float timeStep;

		int activeParticles;
		bool activeParticlesB;

		bool reachedFirstTimerParticleEventB;
		bool reachedFirstTimerParticleB;
		//reachedMaxResolution = false;

		int aboveTimerParticlesMaxIndex;
		int aboveTimerParticlesMinIndex;
		int activeParticlesNoLimit;
		float timeTimer;

		bool reachedMaxParticleOnce;
		int reachedMaxParticlesHowMany;

		//ofxLoadCamera loadCamera;
		//ofxSaveCamera saveCamera;

		//int instanceNumber;

		ofxCameraSaveLoad cameraSaveLoad;
		
		int particlePhase;
		int constructActiveParticle;

		int inactiveCounter;
		int inactiveCounterMax;
		int invisibleCounter;
		int invisibleCounterMax;

		int stepsGrayConstruct;

		ofParameter<float> translateX;
		ofParameter<bool>  translateXB;
	
		string settingsString;
		ofParameter<bool> saveSettingsB;
		
		ofParameter<bool> saveCameraB;
		ofParameter<bool> enableMouseControlB;
		ofParameter<bool>  visibleGuiB;

		//ofFbo fbo;

};
