//
//  ParticleSystemGPU.cpp
//  ParticlesGPU
//
//  Created by Andreas Müller on 11/01/2015.
//
//

#include "ParticleSystemSpawnTexture.h"


ParticleSystemSpawnTexture::ParticleSystemSpawnTexture() {

}

ParticleSystemSpawnTexture::~ParticleSystemSpawnTexture() {
	mesh.clear();
}


//-----------------------------------------------------------------------------------------
//ofEasyCam* _camera,
void ParticleSystemSpawnTexture::init(string meshLocation, string settingsFile, string cam1, string cam2, float  cameraSpeed, int _texSize) //ofVboMesh vboMesh  int instanceNum
{

	settingsString = settingsFile;//meshLocation;
	//string xmlSettingsPath = settingsString;//"Settings/Particles.xml";
	gui.setup( "Particles", settingsString);//


	gui.add(saveSettingsB.set("saveSettingsB", false));
	
	gui.add(saveCameraB.set("saveCameraB", false));
	gui.add(enableMouseControlB.set("enableMouseControlB", false));
	gui.add(visibleGuiB.set("visibleGuiB", false));

	gui.add( particleMaxAge.set("Particle Max Age", 10.0f, 0.0f, 20.0f) );

	gui.add( meshScale.set("Mesh Scale", 1.0f, 0.001f, 10.0f) );
	
	gui.add( noiseMagnitude.set("Noise Magnitude", 0.075, 0.01f, 1.0f) );
	gui.add( noisePositionScale.set("Noise Position Scale", 1.5f, 0.01f, 5.0f) );
	gui.add( noiseTimeScale.set("Noise Time Scale", 1.0 / 4000.0, 0.001f, 1.0f) );
	gui.add( noisePersistence.set("Noise Persistence", 0.2, 0.001f, 1.0f) );
	gui.add( baseSpeed.set("Wind", ofVec3f(0.5,0,0), ofVec3f(-2,-2,-2), ofVec3f(2,2,2)) );
	gui.add( startColor.set("Start Color", ofColor::white, ofColor(0,0,0,0), ofColor(255,255,255,255)) );
	gui.add( endColor.set("End Color", ofColor(0,0,0,0), ofColor(0,0,0,0), ofColor(255,255,255,255)) );
	gui.add( twistNoiseTimeScale.set("Twist Noise Time Scale", 0.01, 0.0f, 0.5f) );
	gui.add( twistNoisePosScale.set("Twist Noise Pos Scale", 0.25, 0.0f, 2.0f) );
	gui.add( twistMinAng.set("Twist Min Ang", -1, -5, 5) );
	gui.add( twistMaxAng.set("Twist Max Ang", 2.5, -5, 5) );

	gui.add(spawnStartPercentage.set("percentage", 50, 0, 100));

	gui.add(startDestruct.set("startDestruct", false));
	gui.add(startConstruct.set("startConstruct", false));
	gui.add(increaseSpawn.set("increaseSpawn", 60, 1, 100));
//	gui.add(cameraRotation.set("cameraRotation", ofVec3f(0, 0, 0), ofVec3f(-180.0, -180.0, -180.0), ofVec3f(180.0, 180.0, 180.0)));
//	gui.add(cameraPosition.set("cameraPos", ofVec3f(0, 0, 0), ofVec3f(-1.0, -1.0, -1.0), ofVec3f(1.0, 1.0, 1.0)));
//	gui.add(cameraRotInvert.set("cameraRotInvert", ofVec3f(0, 0, 0), ofVec3f(-1.0, -1.0, -1.0), ofVec3f(1.0, 1.0, 1.0)));


	//gui.setup( "Light And Material", xmlSettingsPathLight );
	gui.add( globalAmbient.set("Global Ambient", ofFloatColor(0.2,0.2,0.2), ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)) );
	gui.add( light1Diffuse.set("Light 1 Diffuse",   ofColor(50,50,50), ofColor(0,0,0,0), ofColor(255,255,255,255)) );
	gui.add( light1Ambient.set("Light 1 Ambient",   ofColor(50,50,50), ofColor(0,0,0,0), ofColor(255,255,255,255)) );
	gui.add( light1Specular.set("Light 1 Specular", ofColor(255,255,255), ofColor(0,0,0,0), ofColor(255,255,255,255)) );
	gui.add( materialShininess.set("Material Shininess",  20,  0, 127) );
	gui.add( materialAmbient.set("Material Ambient",   	 ofColor(50,50,50), 	ofColor(0,0,0,0), ofColor(255,255,255,255)) );
	gui.add( materialSpecular.set("Material Specular",   ofColor(255,255,255),  ofColor(0,0,0,0), ofColor(255,255,255,255)) );
	gui.add( materialEmissive.set("Material Emmissive",  ofColor(255,255,255),  ofColor(0,0,0,0), ofColor(255,255,255,255)) );
	gui.setPosition( ofVec2f(3800, 0) );

	gui.add(translateXB.set("translateXB", false));
	gui.add(translateX.set("translateX", 0, -1.0,1.0));
	//ofParameter<float> translateX;
	//ofParameter<bool>  translateXB;
	

	loadSettings(settingsString);
	
	loadCamera(cam1,cam2,cameraSpeed);
	camera.setNearClip(0.1);
	camera.disableMouseInput();
	//camera.setLensOffset(ofVec2f(translateX, 0.0));
	// UI for the light and material
	//string xmlSettingsPathLight = "Settings/LightAndMaterial.xml";

	//	guiLightAndMaterial.loadFromFile( xmlSettingsPathLight );
	// Load shaders
	particleUpdate.load("shaders/instancedSpawnTexture/GL2/update");
	particleDraw.load("shaders/instancedSpawnTexture/GL2/drawInstancedGeometry");

	// Set how many particles we are going to have, this is based on data texture size
	textureSize = _texSize;
	numParticles = textureSize * textureSize;

	// Allocate buffers
	ofFbo::Settings fboSettings;
	fboSettings.width  = textureSize;
	fboSettings.height = textureSize;
	
	// We can create several color buffers for one FBO if we want to store velocity for instance,
	// then draw to them simultaneously from a shader using gl_FragData[0], gl_FragData[1], etc.
	fboSettings.numColorbuffers = 2;
	
	fboSettings.useDepth = false;
	fboSettings.internalformat = GL_RGBA32F;	// Gotta store the data as floats, they won't be clamped to 0..1
	fboSettings.textureTarget = GL_TEXTURE_2D;
	fboSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
	fboSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
	fboSettings.minFilter = GL_NEAREST; // No interpolation, that would mess up data reads later!
	fboSettings.maxFilter = GL_NEAREST;
	
	ofDisableTextureEdgeHack();
	
		particleDataFbo.allocate( fboSettings );
	//particleDataFbo.getTextureReference().getTextureData().bFlipTexture = true;

		reset();
	
		spawnPosTexture.allocate( spawnPosBuffer, false );
		spawnPosTexture.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		spawnPosTexture.setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
		spawnPosTexture.loadData( spawnPosBuffer );

		spawnStartTexture.allocate(spawnStartBuffer, false);
		spawnStartTexture.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		spawnStartTexture.setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		spawnStartTexture.loadData(spawnStartBuffer);
	
	ofEnableTextureEdgeHack();
	
	
	ofPrimitiveMode primitiveMode = OF_PRIMITIVE_TRIANGLES; // as we'll be drawing ths mesh instanced many times, we need to have many triangles as opposed to one long triangle strip
	ofMesh tmpMesh;
	
	ofConePrimitive cone( 0.1, 0.1,  5, 2, primitiveMode );
	//tmpMesh = cone.getMesh();

	ofBoxPrimitive box( 0.0015, 0.0015,  0.005 ); // we gotta face in the -Z direction
	tmpMesh = box.getMesh();
	
	singleParticleMesh.append( tmpMesh );
	singleParticleMesh.setMode( primitiveMode );
	
	light[0].setGlobalPosition( ofVec3f( -0.2, 0.35, 0.0 ) );
//	light[0].enable();

	//translateX = 0;

	//reset();
	ofxAssimpModelLoader loader;
	//loader[0].loadModel("newcCorridor.obj");
	//cout << "1" << endl;
	//loader[1].loadModel("newcCrane.obj");
	//cout << "2" << endl;
	//"newcGardenDoor.obj"
	loader.loadModel(meshLocation);
	mesh = loader.getMesh(0);
	for (int i = 0;i < mesh.getNumVertices();i++) {
		bool t = false;
		usedVertices.push_back(t);
	}


	//instanceNumber = instanceNum;


	particlePhase = PARTICLEPHASE_INVISIBLE;


	inactiveCounter=0;
	inactiveCounterMax=100;
	invisibleCounter=0;
	invisibleCounterMax=100;
	constructActiveParticle = 0;


	stepsGrayConstruct = 100;




}


void initNew(string meshLocation, string settingsFile, string cam1, string cam2, float  cameraSpeed = 6000, int _texSize = 128) {

}
//-----------------------------------------------------------------------------------------
//
void ParticleSystemSpawnTexture::reset() {

	ofFbo::Settings fboSettings;
	fboSettings.width = textureSize;
	fboSettings.height = textureSize;

	ofEnableTextureEdgeHack();

	// We are going to encode our data into the FBOs like this
	//
	//	Buffer 1: XYZ pos, W age
	//	Buffer 2: XYZ vel, W not used
	//
	for (int i = 0;i < usedVertices.size();i++) {
		usedVertices[i] = false;
	}
	

	// Initialise the starting and static data
	ofVec4f* startPositionsAndAge = new ofVec4f[numParticles];

	spawnPosBuffer.allocate(fboSettings.width, fboSettings.height, 3);

	spawnStartBuffer.allocate(fboSettings.width, fboSettings.height, 3);

	int tmpIndex = 0;
	for (int y = 0; y < textureSize; y++)
	{
		for (int x = 0; x < textureSize; x++)
		{
			ofVec3f pos(0, 0, 0);
			//ofVec3f pos (MathUtils::randomPointOnSphere() * 0.1);
			//pos.set( ofRandom(-1,1), ofRandom(0,2), ofRandom(-1,1) );
			float startAge = particleMaxAge;// 0;//ofRandom(particleMaxAge); // position is not very important, but age is, by setting the lifetime randomly somewhere in the middle we can get a steady stream emitting

			startPositionsAndAge[tmpIndex] = ofVec4f(pos.x, pos.y, pos.z, startAge);

			// startPositionsAndAge[tmpIndex].w=
			//ofVec3f spawnPos( ofRandom( 1.0f ), ofRandom( 1.0f ), ofRandom( 1.0f ) );
			ofVec3f spawnPos = MathUtils::randomPointOnSphere() * 0.02;

			spawnPosBuffer.getPixels()[(tmpIndex * 3) + 0] = spawnPos.x;
			spawnPosBuffer.getPixels()[(tmpIndex * 3) + 1] = spawnPos.y;
			spawnPosBuffer.getPixels()[(tmpIndex * 3) + 2] = spawnPos.z;

			//if (ofMap(tmpIndex, 0, spawnStartBuffer.size(), 0, 100)>spawnStartPercentage) {
			//spawnStartBuffer.getPixels()[(tmpIndex * 3) + 0] = 1.0;
			//}
			//else {
			spawnStartBuffer.getPixels()[(tmpIndex * 3) + 0] = 0.0;
			//}


			tmpIndex++;
		}
	}

	// Upload it to the source texture
	particleDataFbo.source()->getTextureReference(0).loadData((float*)&startPositionsAndAge[0].x, textureSize, textureSize, GL_RGBA);

	ofDisableTextureEdgeHack();

	timeStep = 1.0f / 60.0f;
	time = 0.0f;

	activeParticles = 0;
	activeParticlesB = false;

	reachedFirstTimerParticleEventB = false;
	reachedFirstTimerParticleB = false;
	//reachedMaxResolution = false;

	//reachedFirstTimerParticleEventB = true;
	//reachedFirstTimerParticleB = true;
	//aboveTimerParticlesMinIndex = 0;

	aboveTimerParticlesMaxIndex = 0;
	aboveTimerParticlesMinIndex = 0;
	activeParticlesNoLimit = 0;
	timeTimer = 0;

	reachedMaxParticleOnce = false;

	reachedMaxParticlesHowMany = 0;

}
//-----------------------------------------------------------------------------------------
bool ParticleSystemSpawnTexture::update() {

	if (saveSettingsB) {
		gui.saveToFile(settingsString);
		saveSettingsB = false;
	}
	if (saveCameraB) {
		saveCamera(ofGetTimestampString());
		saveCameraB = false;
	}

	if (enableMouseControlB) {
		camera.enableMouseInput();
	}
	else {
		camera.disableMouseInput();
	cameraSaveLoad.transitionCameraUpdate(camera, true);

	}




	timeStep = ofLerp(timeStep, 1.0f / 120.0f, 0.1f);
	time += timeStep;

	

	if (startDestruct) {

		startDestruct = false;
		timeTimer = time;
		activeParticlesB = true;

		particlePhase = PARTICLEPHASE_DESTRUCT;
	}

	if (startConstruct) {

		startConstruct = false;
		particlePhase = PARTICLEPHASE_CONSTRUCT;
		constructActiveParticle = 0;
	}


	if (particlePhase == PARTICLEPHASE_DESTRUCT) {
		//REACHING THE TIMER
		if (activeParticlesB == true && (time - timeTimer) > particleMaxAge) {


			if (reachedFirstTimerParticleEventB == false) {
				reachedFirstTimerParticleEventB = true;
				reachedFirstTimerParticleB = true;
				aboveTimerParticlesMinIndex = 0;
				aboveTimerParticlesMaxIndex = activeParticles;
			}

		}

		if (activeParticlesB) {

			activeParticles += increaseSpawn;
			if (activeParticles > numParticles) {
				activeParticles = numParticles;// REACHED THE MAX OF PARTICLES TO BE USED
														 //cout << "REACHED MAX" << endl;
														 //reachedFirstTimerParticleB = true;
				activeParticlesNoLimit += increaseSpawn;
				reachedMaxParticleOnce = true;

				float division = (float)activeParticlesNoLimit / (float)numParticles;
				reachedMaxParticlesHowMany = floor(division);

			}
			else {
				activeParticlesNoLimit = activeParticles;//?
			}
		}

		if (activeParticlesB && reachedFirstTimerParticleB) {


			//if (activeParticlesNoLimit != mesh.getNumVertices()) {
			aboveTimerParticlesMinIndex += increaseSpawn;
			aboveTimerParticlesMaxIndex += increaseSpawn;
			//}
			//aboveTimerParticlesMaxIndex = 0;//reset
			//aboveTimerParticlesMaxIndex = aboveTimerParticlesMinIndex + activeParticles;
			if (aboveTimerParticlesMinIndex > mesh.getNumVertices()) {
				//cout << "END" << endl;
				particlePhase = PARTICLEPHASE_INVISIBLE;
				reset();

			}
		}


		int tmpIndex = 0;
		//int tmpMeshIndex = activeParticles;
		int limit;
		if (reachedMaxParticleOnce) {
			limit = activeParticlesNoLimit;
		}
		else {
			limit = activeParticles;
		}

		tmpIndex = 0;
		bool endSequence = false;
		//cout << "reach particke" << reachedMaxParticlesHowMany << endl;
		for (int y = 0; y < textureSize; y++)
		{
			for (int x = 0; x < textureSize; x++)
			{
				bool skip = false;
				int meshIndex = tmpIndex;
				int meshIndexBefore = meshIndex;
				ofVec3f spawnPos = mesh.getVertex(meshIndex);
				if (reachedMaxParticleOnce) {
					meshIndex = tmpIndex + (numParticles*(reachedMaxParticlesHowMany));//- 1
					meshIndexBefore = tmpIndex + (numParticles*(reachedMaxParticlesHowMany - 1));
			
					if (meshIndex<mesh.getNumVertices()) {
						spawnPos = mesh.getVertex(meshIndex);//NEEDS TO BE CHANGED WITH NUM OF ITERATION
					}
					else if(meshIndexBefore>aboveTimerParticlesMinIndex
						&& meshIndexBefore < limit){
						//spawnStartBuffer.getPixels()[(tmpIndex * 3) + 0] = 0.0;//DON'T DRAW
						spawnPos.set(0, 0, 0); //DIRTY TRICK

					}
					else {
						skip = true;
					}
				}


				//+maxParticlesReached
				spawnPosBuffer.getPixels()[(tmpIndex * 3) + 0] = spawnPos.x;
				spawnPosBuffer.getPixels()[(tmpIndex * 3) + 1] = spawnPos.y;
				spawnPosBuffer.getPixels()[(tmpIndex * 3) + 2] = spawnPos.z;

				if (activeParticlesB == true) {

					if (tmpIndex + (numParticles*reachedMaxParticlesHowMany)>aboveTimerParticlesMinIndex 
						&& tmpIndex + (numParticles*reachedMaxParticlesHowMany) < limit) {
						spawnStartBuffer.getPixels()[(tmpIndex * 3) + 0] = 1.0;// DRAW
					}
					else if (reachedMaxParticleOnce == true &&
						tmpIndex + (numParticles*(reachedMaxParticlesHowMany - 1)) < limit
						&&
						tmpIndex + (numParticles*(reachedMaxParticlesHowMany - 1)) > aboveTimerParticlesMinIndex
						//&&
						) {
						spawnStartBuffer.getPixels()[(tmpIndex * 3) + 0] = 1.0;
					}
					else {
						spawnStartBuffer.getPixels()[(tmpIndex * 3) + 0] = 0.0;//DON'T DRAW

					}

					if (skip) {
						spawnStartBuffer.getPixels()[(tmpIndex * 3) + 0] = 0.0;//DON'T DRAW
					}


				}
				tmpIndex++;
			}
		}



		tmpIndex = 0;
		for (int y = 0; y < textureSize; y++)
		{
			for (int x = 0; x < textureSize; x++)
			{

				int meshIndex = tmpIndex;
				ofVec3f spawnPos = mesh.getVertex(meshIndex);
				if (reachedMaxParticleOnce) {
					meshIndex = tmpIndex + (numParticles*(reachedMaxParticlesHowMany));//- 1
					if (meshIndex < mesh.getNumVertices()) {
						spawnPos = mesh.getVertex(meshIndex);//NEEDS TO BE CHANGED WITH NUM OF ITERATION
					}
					else {
						//spawnStartBuffer.getPixels()[(tmpIndex * 3) + 0] = 0.0;//DON'T DRAW

					}
				}
				//usedVertices[meshIndex] = true;

				tmpIndex++;
			}
		}


		if(reachedFirstTimerParticleB){
		//cout << aboveTimerParticlesMinIndex << " aboveTimerParticlesMinIndex" << endl;
		for (int i = 0;i <  mesh.getNumVertices();i++) {

			usedVertices[i] = true;
		}

		for (int i = aboveTimerParticlesMinIndex-numParticles; i < mesh.getNumVertices(); i++) {


			usedVertices[i] = false;
		}
	  }


		spawnPosTexture.loadData(spawnPosBuffer);
		spawnStartTexture.loadData(spawnStartBuffer);

		updateTime(time, timeStep);

	}
	else if (particlePhase == PARTICLEPHASE_CONSTRUCT) {
		constructActiveParticle += (increaseSpawn*10);//- (increaseSpawn*stepsGrayConstruct)
		if (constructActiveParticle  > mesh.getNumVertices()) {
			particlePhase = PARTICLEPHASE_INACTIVE;
			constructActiveParticle = 0;
		}
	}
	else if (particlePhase == PARTICLEPHASE_INVISIBLE) {
		invisibleCounter++;


		if (invisibleCounter>invisibleCounterMax) {
			//cout << "MAX REACHED" << endl;
			particlePhase = PARTICLEPHASE_CONSTRUCT;
			startConstruct = true;
			invisibleCounter = 0;
			return false;
		}
	
	}
	else if (particlePhase == PARTICLEPHASE_INACTIVE) {
		inactiveCounter ++;
		if (inactiveCounter>inactiveCounterMax) {
			particlePhase = PARTICLEPHASE_DESTRUCT;
			startDestruct = true;
			invisibleCounter = 0;
		}
	}
	return true;
}


//
void ParticleSystemSpawnTexture::updateTime( float _time, float _timeStep )
{

	updateParticles( _time, _timeStep );
}

//-----------------------------------------------------------------------------------------
//
void ParticleSystemSpawnTexture::draw(  )//ofCamera* _camera
{	
	//fbo.begin();

	//ofSetGlobalAmbientColor(globalAmbient);
	light[0].setAmbientColor( light1Ambient.get() ); // If you're having trouble passing an 'ofParameter<Class>' into something that expects a 'Class' use .get()
	light[0].setDiffuseColor( light1Diffuse.get() );
	light[0].setSpecularColor( light1Specular.get() );

	particleMaterial.setAmbientColor( materialAmbient.get() );
	particleMaterial.setSpecularColor( materialSpecular.get() );
	particleMaterial.setEmissiveColor( materialEmissive.get() );
	particleMaterial.setShininess( materialShininess );

	//ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	if (translateXB) {
		camera.setLensOffset(ofVec2f(translateX, 0));
	}
	
		camera.begin();
		ofPushMatrix();
		ofScale(1, -1, 1);
		//ofTranslate(translateX, 0, 0);

		if (particlePhase == PARTICLEPHASE_DESTRUCT) {
			ofEnableLighting();
	light[0].enable();
			drawParticles(&particleDraw);//_camera, &camera
	light[0].disable();	
			ofDisableLighting();
		}

		ofSetColor(light[0].getDiffuseColor());
		ofDrawSphere(light[0].getGlobalPosition(), 0.01);
		ofDrawAxis(2);

		ofSetColor(ofColor::white);
		//particleMaterial.begin();
		drawMesh();
		//particleMaterial.end();
		ofPopMatrix();
		camera.end();
	

		//fbo.end();
		//fbo.draw(translateX, 0);
		

}


void ParticleSystemSpawnTexture::drawMesh()
{
	//camera.begin();
	ofEnableLighting();
	ofEnableAlphaBlending();
	if (particlePhase == PARTICLEPHASE_DESTRUCT) {	
		ofSetColor(ofColor::white);

		ofVboMesh vboMesh;
		
	

		int limit;
		if (reachedMaxParticleOnce) {
			limit = activeParticlesNoLimit;
		}
		else {
			limit = activeParticles;
		}
		for (int i = limit; i < mesh.getNumVertices(); i++) {
			vboMesh.addVertex(mesh.getVertex(i));
		}

		vboMesh.drawVertices();
		//ofDisableLighting();

	}
	else if (particlePhase == PARTICLEPHASE_CONSTRUCT) {

		//ofEnableLighting();

		for (int i = 0; i <stepsGrayConstruct; i++) {
				ofVboMesh vboMesh;
			for (int j = constructActiveParticle - (increaseSpawn*(i+1)); j < constructActiveParticle - (increaseSpawn*i); j++) {
				if (j >= 0 && j < mesh.getNumVertices()) {
				
					vboMesh.addVertex(mesh.getVertex(j));
					//indexCurrent++;
				}	
				ofSetColor(255, 255, 255, ofMap(i, 0, stepsGrayConstruct, 0, 255));
				vboMesh.drawVertices();
			}
		}

		ofVboMesh vboMeshA;
		ofSetColor(255, 255, 255, 255); //- (increaseSpawn*stepsGrayConstruct)
		for (int i = 0; i < constructActiveParticle; i++) {
			vboMeshA.addVertex(mesh.getVertex(i));
		}
		vboMeshA.drawVertices();

		//ofDisableLighting();

	}
	else if (particlePhase == PARTICLEPHASE_INVISIBLE) {

		ofSetColor(ofColor::white);
		//ofEnableLighting();
		//ofDisableLighting();
	}
	else if (particlePhase == PARTICLEPHASE_INACTIVE) {
		//constructActiveParticle		
		ofSetColor(ofColor::white);
	
		ofVboMesh vboMesh;
		for (int i = 0; i < mesh.getNumVertices(); i++) {
			vboMesh.addVertex(mesh.getVertex(i));
		}

		vboMesh.drawVertices();
		
	}
	ofDisableLighting();
}

//-----------------------------------------------------------------------------------------
//
void ParticleSystemSpawnTexture::drawGui()
{
	
	//if (visibleGuiB) {
	
	gui.draw();
//	guiLightAndMaterial.draw();
	
	spawnPosTexture.draw( gui.getPosition() + ofVec2f(0,gui.getHeight() + 10 ), 128, 128);
	spawnStartTexture.draw(gui.getPosition() + ofVec2f(138, gui.getHeight() + 10), 128, 128);
//}
	
}

//-----------------------------------------------------------------------------------------
//
void ParticleSystemSpawnTexture::updateParticles( float _time, float _timeStep )
{
	ofEnableBlendMode( OF_BLENDMODE_DISABLED ); // Important! We just want to write the data as is to the target fbo
	//ofClear(255);try 27 didnt worked
	particleDataFbo.dest()->begin();
	
		particleDataFbo.dest()->activateAllDrawBuffers(); // if we have multiple color buffers in our FBO we need this to activate all of them
		
		particleUpdate.begin();
	
			particleUpdate.setUniformTexture( "u_particlePosAndAgeTexture",	particleDataFbo.source()->getTextureReference(0), 0 );
			particleUpdate.setUniformTexture( "u_particleVelTexture",		particleDataFbo.source()->getTextureReference(1), 1 );
			particleUpdate.setUniformTexture( "u_spawnPosTexture",			spawnPosTexture,								  2 );


			particleUpdate.setUniformTexture("u_particleSpawnStart", spawnStartTexture,3);
	

			particleUpdate.setUniform1f("u_time", _time );
			particleUpdate.setUniform1f("u_timeStep", _timeStep );
			
			particleUpdate.setUniform1f("u_particleMaxAge", particleMaxAge );
			
			particleUpdate.setUniform1f("u_noisePositionScale", noisePositionScale );
			particleUpdate.setUniform1f("u_noiseTimeScale", noiseTimeScale );
			particleUpdate.setUniform1f("u_noisePersistence", noisePersistence );
			particleUpdate.setUniform1f("u_noiseMagnitude", noiseMagnitude );
			particleUpdate.setUniform3f("u_baseSpeed", baseSpeed.get().x, baseSpeed.get().y, baseSpeed.get().z );
			
			particleDataFbo.source()->draw(0,0);
		
		particleUpdate.end();
		
	particleDataFbo.dest()->end();
	
	particleDataFbo.swap();
}

//-----------------------------------------------------------------------------------------
//
void ParticleSystemSpawnTexture::drawParticles( ofShader* _shader )//, ofCamera* _camera
{
	ofFloatColor particleStartCol = startColor.get();
	ofFloatColor particleEndCol = endColor.get();

	ofSetColor( ofColor::white );
	ofEnableBlendMode( OF_BLENDMODE_ALPHA );
	
	_shader->begin();

		_shader->setUniformTexture("u_particlePosAndAgeTexture", particleDataFbo.source()->getTextureReference(0), 1 );
		_shader->setUniformTexture("u_particleVelTexture", particleDataFbo.source()->getTextureReference(1), 2 );
		
		
		_shader->setUniformTexture("u_particleSpawnStart", spawnStartTexture, 3);



		_shader->setUniform1f("u_meshScale", meshScale );
	
		_shader->setUniform2f("u_resolution", particleDataFbo.source()->getWidth(), particleDataFbo.source()->getHeight() );
		_shader->setUniform1f("u_time", ofGetElapsedTimef() );
	
		_shader->setUniformMatrix4f("u_modelViewMatrix",ofGetCurrentMatrix(OF_MATRIX_MODELVIEW) );//_camera->getModelViewMatrix() camera.getModelViewMatrix()
		_shader->setUniformMatrix4f("u_projectionMatrix", ofGetCurrentMatrix(OF_MATRIX_PROJECTION));
		//camera.getProjectionMatrix() 
		//_camera->getProjectionMatrix()camera.getModelViewProjectionMatrix()
		//ofGetCurrentMatrix(OF_MATR)
		_shader->setUniformMatrix4f("u_modelViewProjectionMatrix", ofGetCurrentMatrix(OF_MATRIX_MODELVIEW) *ofGetCurrentMatrix(OF_MATRIX_PROJECTION));// camera.getModelViewProjectionMatrix());//_
//camera.getModelViewProjectionMatrix() 
		//_shader->setUniformMatrix4f("u_modelViewMatrix", ofGetCurrentMatrix(OF_MATRIX_MODELVIEW)); // 

		_shader->setUniform1f("u_particleMaxAge", particleMaxAge );

		_shader->setUniform1i("u_numLights", 1 );
	
		_shader->setUniform4fv("u_particleStartColor", particleStartCol.v );
		_shader->setUniform4fv("u_particleEndColor", particleEndCol.v );

		// Calling begin() on the material sets the OpenGL state that we then read in the shader
		particleMaterial.begin();
	
			singleParticleMesh.drawInstanced( OF_MESH_FILL, numParticles );

		particleMaterial.end();
	
	_shader->end();

	ofEnableBlendMode(OF_BLENDMODE_DISABLED); // Important! We just want to write the data as is to the target fbo

	
}

void ParticleSystemSpawnTexture::saveCamera(string saveString) {

	cameraSaveLoad.saveCamera(camera, saveString);// + ofToString(instanceNumber)
}

void ParticleSystemSpawnTexture::loadCamera(string camera1, string camera2, float durationMotion) {
	//cameraSaveLoad.loadCamera(camera, "camera/camSettings0");// + ofToString(instanceNumber)
	cameraSaveLoad.loadCamera(camera, camera1);
	cameraSaveLoad.transitionCameraSetup(camera1, camera2, durationMotion);
}

void ParticleSystemSpawnTexture::saveSettings(string saveS) {
	gui.saveToFile(saveS);
}
void ParticleSystemSpawnTexture::loadSettings(string loadS) {
	gui.loadFromFile(loadS);
}
