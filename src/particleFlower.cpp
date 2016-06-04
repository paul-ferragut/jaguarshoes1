#include "particleFlower.h"

//------------------------------------------------------------------
particleFlower::particleFlower(){

}
//------------------------------------------------------------------
particleFlower::~particleFlower(){
   // attractPoints = NULL;
	trail.clear();
}


//------------------------------------------------------------------

void particleFlower::setup(ofPoint origin, ofVec2f direction, int subDivision) {
	
    
    uniqueVal = ofRandom(-10000, 10000);
    pos = origin;
	state = STATE_UPDATE;
    float normalizedLevel = ofMap(subDivision, 0, 60, 1.0, 0.01, true);
    //TO DO DIRECTION, WEAKER ACCORDING TO SUBDivison
    
    vel.x = ofRandom(-0.002, 0.002)* normalizedLevel;
	vel.y = ofRandom(-0.002, 0.002)* normalizedLevel;
	
	frc   = ofPoint(0,0,0);
	
	dir = direction;
	
	readyDeconstruct = false;

   // attraction=
    
	scale = normalizedLevel;
	
	//if( mode == PARTICLE_MODE_NOISE ){
	//	drag  = ofRandom(0.97, 0.99);
	//	vel.y = fabs(vel.y) * 3.0; //make the particles all be going down
	//}else{
    drag  = 0.95;//ofRandom(0.95, 0.998);
	//}
    
    durationEnd=ofRandom(500,700)* (normalizedLevel);
	breakEnd = durationEnd + ofRandom(1, 900);//*(normalizedLevel);
    duration=0;
	//ended = false;

}

//------------------------------------------------------------------
int particleFlower::update(){

    
    
    
    duration++;
    if(duration<durationEnd){
    
	//1 - APPLY THE FORCES BASED ON WHICH MODE WE ARE IN 
	/*
	if( mode == PARTICLE_MODE_ATTRACT ){
		ofPoint attractPt(ofGetMouseX(), ofGetMouseY());
		frc = attractPt-pos; // we get the attraction force/vector by looking at the mouse pos relative to our pos
		frc.normalize(); //by normalizing we disregard how close the particle is to the attraction point 
		
		vel *= drag; //apply drag
		vel += frc * 0.6; //apply force
	}
	else if( mode == PARTICLE_MODE_REPEL ){*/
		ofPoint attractPt(dir);
		frc = attractPt-pos; 
		
		//let get the distance and only repel points close to the mouse
		float dist = frc.length();
		frc.normalize(); 
		
		vel *= drag; 
		//if( dist < 1500 ){
	
		//}else{
            
            ofVec2f frc2;
			//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy. 			
			frc2.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
			frc2.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
			vel += frc2 * 0.004;
        
        vel += -frc * (0.003*scale); //notice the frc is negative
        
        
		//}
	/*
    }
	else if( mode == PARTICLE_MODE_NOISE ){
		//lets simulate falling snow 
		//the fake wind is meant to add a shift to the particles based on where in x they are
		//we add pos.y as an arg so to prevent obvious vertical banding around x values - try removing the pos.y * 0.006 to see the banding
		float fakeWindX = ofSignedNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6);
		
		frc.x = fakeWindX * 0.25 + ofSignedNoise(uniqueVal, pos.y * 0.04) * 0.6;
		frc.y = ofSignedNoise(uniqueVal, pos.x * 0.006, ofGetElapsedTimef()*0.2) * 0.09 + 0.18;

		vel *= drag; 
		vel += frc * 0.4;
		
		//we do this so as to skip the bounds check for the bottom and make the particles go back to the top of the screen
		if( pos.y + vel.y > ofGetHeight() ){
			pos.y -= ofGetHeight();
		}
	}
     */

	
	
	//2 - UPDATE OUR POSITION
	
	pos += vel;
        if((duration % 201 == 200) || durationEnd==duration+1){
        trail.push_back(pos);
        }
        
        
		state = STATE_UPDATE;
        return STATE_UPDATE;
	}
	else if (duration == durationEnd) {
		state = STATE_ENDED;
		return STATE_ENDED;
	}else if (duration > durationEnd) {


		if (duration > breakEnd && readyDeconstruct==true) {
			if (trail.size() != 0) {
				trail.erase(trail.begin());
				state = STATE_DECONSTRUCT;
				return STATE_DECONSTRUCT;
			}
			else if (trail.size() == 0) {
				state = STATE_READY;
				return STATE_READY;
			}
			
		}
		state = STATE_INACTIVE;
		return STATE_INACTIVE;
    }
        
	
	//3 - (optional) LIMIT THE PARTICLES TO STAY ON SCREEN 
	//we could also pass in bounds to check - or alternatively do this at the ofApp level
    /*
	if( pos.x > ofGetWidth() ){
		pos.x = ofGetWidth();
		vel.x *= -1.0;
	}else if( pos.x < 0 ){
		pos.x = 0;
		vel.x *= -1.0;
	}
	if( pos.y > ofGetHeight() ){
		pos.y = ofGetHeight();
		vel.y *= -1.0;
	}
	else if( pos.y < 0 ){
		pos.y = 0;
		vel.y *= -1.0;
	}	
     */
		
}

//------------------------------------------------------------------
ofVec2f particleFlower::getPos(){
    return pos;
}

//------------------------------------------------------------------
void particleFlower::draw(ofColor colStart, ofColor colEnd){
	//float scaleN = ofMap(scale, 0.0f, 1.0f, 1.0f, 0.1f,true);
    
    ofSetColor(colStart.lerp(colEnd,scale));
    /*
	if( mode == PARTICLE_MODE_ATTRACT ){
		ofSetColor(255, 63, 180);
	}
	else if( mode == PARTICLE_MODE_REPEL ){
		ofSetColor(208, 255, 63);
	}
	else if( mode == PARTICLE_MODE_NOISE ){
		ofSetColor(99, 63, 255);
	}
	else if( mode == PARTICLE_MODE_NEAREST_POINTS ){
		ofSetColor(103, 160, 237);
	}*/
			
    ofSetLineWidth(scale*2.0);
    if(trail.size()>1){
    for(int i=0;i<trail.size()-1;i++){
    
    ofDrawLine(trail[i].x, trail[i].y, trail[i+1].x, trail[i+1].y);
        
    }
        }
	if (trail.size() > 0) {
		ofDrawCircle(pos.x, pos.y, scale * 1.5);
	}
}

