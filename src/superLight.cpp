#include "superLight.h"

superLight::superLight() {}
superLight::~superLight() {}

void superLight::setup(ofRectangle posBnds, ofRectangle lookAtBnds, ofVec2f speedPos, ofVec2f speedLookat, float zLight) {
	//speed between 0.0 1.0
	positionBounds= posBnds;
	lookAtBounds= lookAtBnds;

	speedPosition= speedPos;
	speedLookAt= speedLookat;

	//bool shadowIsInitiated;
	 useShadow=false;
	 useShadowTop=false;

	//frequencyPosition=freqPos;
	//frequencyLookAt=freqLookAt;
	
	zLightPos = zLight;
	uniqueStartPos.set(ofRandom(0.0f, 1000.0f), ofRandom(0.0f, 1000.0f));
	uniqueStartLookAt.set(ofRandom(0.0f, 1000.0f), ofRandom(0.0f, 1000.0f));


	light.setDiffuseColor(ofColor(0.0f, 0.0f, 255.f));

	// specular color, the highlight/shininess color //
	light.setSpecularColor(ofColor(255.f, 255.f, 255.f));
	light.setPointLight();

	//material.setShininess(120);
	// the light highlight of the material //
	//material.setSpecularColor(ofColor(255, 255, 255, 255));


}

		void superLight::update() {
			updatePos();
			updateLookAt();
		}
		void superLight::updatePos() {
			
			float xFactor = positionBounds.width;
			float yFactor = positionBounds.height;
//*frequencyPosition.x*frequencyPosition.y
		//	position.z = zLightPos;

		   position.x= positionBounds.x+ ofNoise(((ofGetElapsedTimef()*speedPosition.x)+uniqueStartPos.x))*xFactor;
		   position.y= positionBounds.y+ ofNoise(((ofGetElapsedTimef()*speedPosition.y)+uniqueStartPos.y))*yFactor;
		   light.setPosition(position);

		}
		void superLight::updateLookAt() {

			float xFactor = lookAtBounds.width;
			float yFactor = lookAtBounds.height;
//*frequencyLookAt.x*frequencyLookAt.y
			lookAt.x = lookAtBounds.x + ofNoise(((ofGetElapsedTimef()*speedLookAt.x) + uniqueStartLookAt.x))*xFactor;
			lookAt.y = lookAtBounds.y + ofNoise(((ofGetElapsedTimef()*speedLookAt.y) + uniqueStartLookAt.y))*yFactor;
			

			light.lookAt(lookAt);
		}

		void superLight::setPosition(ofPoint pos) {
			position = pos;
		}
		void superLight::setLookAt(ofPoint pos) {

		}


		void superLight::setZ(float posZ) {
			position.z = posZ;
		}
		/*
		void superLight::setMaterial(int shine) {
			useMaterialB = true;
			material.setShininess(shine);
		}
		*/
		void superLight::begin() {
			

			//if (useMaterialB) {
			//	material.begin();
			//}
			
			light.enable();
		
			
		}
		void superLight::end() {
			light.disable();

			/*
			if (useMaterialB) {
				material.end();
			}
			useMaterialB = false;*/
		}

		void superLight::drawDebug() {

		//	ofEnableDepthTest();
			ofSetColor(255, 255, 255);
			light.draw();
			ofSetColor(255, 0, 0);
			ofSphere(position, 10);
			ofSetColor(255, 255, 0);
			ofSphere(lookAt, 5);

			ofLine(position, lookAt);
			ofSetColor(255, 255, 255);

		//	ofSphere(getShadowOffset(), 6);
		//	ofSetColor(255, 0, 255);
		//	ofSphere(getShadowOffsetMapped(0.5), 7);

		//	ofDisableDepthTest();

		}


		
		ofVec2f superLight::getShadowOffset() {

			return lookAt - position;
		}

		ofVec2f superLight::getShadowOffsetMapped(float lengthRatio) {
		
			ofVec2f n = getShadowOffset();
			//n.x = fabs(n.x);
			//n.y = fabs(n.y);

			float dist=ofDist(0,0, n.x, n.y);

			float scaleDist = ofMap(dist, 0, (positionBounds.width + positionBounds.height) / 2, 0, ((positionBounds.width + positionBounds.height) / 2.0)*lengthRatio);

			
			float scaleRatio =   scaleDist/dist; 

			n *= scaleRatio;
			return n;
		}

		void superLight::setLightType(int lightType) {
		
			switch(lightType){

			case LIGHT_TYPE_POINT:

				// Point lights emit light in all directions //
				light.setPointLight();
			break;

			case LIGHT_TYPE_SPOT:
				// turn the light into spotLight, emit a cone of light //
				light.setSpotlight();

				// size of the cone of emitted light, angle between light axis and side of cone //
				// angle range between 0 - 90 in degrees //
				light.setSpotlightCutOff(50);

				// rate of falloff, illumitation decreases as the angle from the cone axis increases //
				// range 0 - 128, zero is even illumination, 128 is max falloff //
				light.setSpotConcentration(45);
			break;

			case LIGHT_TYPE_DIRECTIONAL:
				// Directional Lights emit light based on their orientation, regardless of their position //
				light.setDirectional();
			break;
			
			}
			
		
		}


		void superLight::setColor(ofColor specular, ofColor diffuse) {

			// set the diffuse color, color reflected from the light source // ofColor materialCol,
			light.setDiffuseColor(diffuse);

			// specular color, the highlight/shininess color //
			light.setSpecularColor(specular);


			// shininess is a value between 0 - 128, 128 being the most shiny //
		//	material.setShininess(shininess);
			// the light highlight of the material //
			//material.setSpecularColor(materialCol);, int shininess

		}


		void superLight::drawShadowBack(float spread) {
			//cout << position << endl;
			lightShadow.drawBackShadow(ofVec2f(position.x, position.y), spread);
		}
		void superLight::drawShadowFront(float spread) {
		
			lightShadow.drawShadowFront(ofVec2f(position.x, position.y), spread);
		}

		