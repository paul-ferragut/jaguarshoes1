#include "superLight.h"

superLight::superLight() {}
superLight::~superLight() {}

void superLight::setup(ofRectangle posBnds, ofRectangle lookAtBnds, ofVec2f speedPos, ofVec2f speedLookat, float zLight) {
	//speed between 0.0 1.0
	positionBounds= posBnds;
	lookAtBounds= lookAtBnds;

	speedPosition= speedPos;
	speedLookAt= speedLookat;


	//frequencyPosition=freqPos;
	//frequencyLookAt=freqLookAt;
	
	zLightPos = zLight;
	uniqueStartPos.set(ofRandom(0.0f, 1000.0f), ofRandom(0.0f, 1000.0f));
	uniqueStartLookAt.set(ofRandom(0.0f, 1000.0f), ofRandom(0.0f, 1000.0f));


	light.setDiffuseColor(ofColor(0.0f, 0.0f, 255.f));

	// specular color, the highlight/shininess color //
	light.setSpecularColor(ofColor(255.f, 255.f, 255.f));
	light.setPointLight();

	material.setShininess(120);
	// the light highlight of the material //
	material.setSpecularColor(ofColor(255, 255, 255, 255));


}

		void superLight::update() {
			updatePos();
			updateLookAt();
		}
		void superLight::updatePos() {
			
			float xFactor = positionBounds.width;
			float yFactor = positionBounds.height;
//*frequencyPosition.x*frequencyPosition.y
			position.z = zLightPos;

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

		}
		void superLight::setLookAt(ofPoint pos) {

		}

		void superLight::begin(bool useMaterial) {
			useMaterialB = useMaterial;

			if (useMaterialB) {
				material.begin();
			}
			
			light.enable();
			
		}
		void superLight::end() {
			light.disable();

			if (useMaterialB) {
				material.end();
			}
		}

		void superLight::drawDebug() {

			ofEnableDepthTest();
			ofSetColor(255, 255, 255);
			light.draw();
			ofSetColor(255, 0, 0);
			ofSphere(position, 10);
			ofSetColor(255, 255, 0);
			ofSphere(lookAt, 5);

			ofLine(position, lookAt);
			ofSetColor(255, 255, 255);

			ofSphere(getShadowOffset(), 6);
			ofSetColor(255, 0, 255);
			ofSphere(getShadowOffsetMapped(0.5), 7);

			ofDisableDepthTest();

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