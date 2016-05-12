#include "particleVoronoi.h"

//------------------------------------------------------------------
particleVoronoi::particleVoronoi(){
	attractPoints = NULL;
}

//------------------------------------------------------------------
particleVoronoi::~particleVoronoi() {
	
}


//------------------------------------------------------------------
void particleVoronoi::setAttractPoints( vector <ofPoint> * attract ){
	attractPoints = attract;
}

//------------------------------------------------------------------
void particleVoronoi::reset(){
	//the unique val allows us to set properties slightly differently for each particle
	uniqueVal = ofRandom(-10000, 10000);
	
	isDouble = false;
	doubleIndex = 0;
	cellSize = 1.0;
	pos.x = ofRandomWidth();
	pos.y = ofRandomHeight();
    
    originPos=pos;
	
	vel.x = ofRandom(-0.9, 0.9);
	vel.y = ofRandom(-0.9, 0.9);
	
	frc   = ofPoint(0,0,0);
	
    bassCounter=0;
    bassTap=false;
	soundPt.set(0, 0);
	scale = ofRandom(0.5, 1.0);
	
	//if( mode == PARTICLE_MODE_NOISE ){
	//	drag  = ofRandom(0.97, 0.99);
	//	vel.y = fabs(vel.y) * 3.0; //make the particles all be going down
	//}else{
		drag  = ofRandom(0.95, 0.998);	
	//}
}

//------------------------------------------------------------------
void particleVoronoi::update(){


    
        ofPoint attractPt=originPos;
        frc = attractPt-pos; // we get the attraction force/vector by looking at the mouse pos relative to our pos
        
        frc.x+=ofSignedNoise(uniqueVal, pos.y * 0.3, ofGetElapsedTimef()*0.2);
        frc.y+=ofSignedNoise(uniqueVal, pos.x * 0.3, ofGetElapsedTimef()*0.2);
        
        frc.normalize(); //by normalizing we disregard how close the particle is to the attraction point
        
        vel *= drag; //apply drag
        vel += frc * 0.02; //apply force
        
      //  frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
      //  frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
        
        
       if(bassTap){
           
           
           bassCounter=1.0;
                      bassTap=false;
        
        }  
        
        if(bassCounter<=0){
            bassCounter=0;
        }else{
            bassCounter-=0.2;
            bassCounter=bassCounter;
        //}
        
            ofPoint repelPt(soundPt);
            frc = repelPt-pos;
            
            //let get the distance and only repel points close to the mouse
            float dist = frc.length();
            frc.normalize();
            
            //vel *= drag;
            if( dist < 750 ){
                vel += (-frc * 0.09) * bassCounter ; //notice the frc is negative
			}
			else {
				//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy.

			   // vel += frc * 0.04;
			}
        }

            

        
    
  
	
	//2 - UPDATE OUR POSITION
	
	pos += vel*cellSize; 
	
	
	//3 - (optional) LIMIT THE PARTICLES TO STAY ON SCREEN 
	//we could also pass in bounds to check - or alternatively do this at the ofApp level
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
		
}

//------------------------------------------------------------------
void particleVoronoi::draw(){


	ofDrawCircle(pos.x, pos.y, scale * 4.0);
}

