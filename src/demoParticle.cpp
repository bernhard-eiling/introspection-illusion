#include "demoParticle.h"
#include <XnCppWrapper.h>

//------------------------------------------------------------------
demoParticle::demoParticle(){
	attractPoints = NULL;
    forceFieldsPoint = NULL;
}

//------------------------------------------------------------------
void demoParticle::setAttractPoints( vector <ofPoint> * attract ){
	attractPoints = attract;
}


//------------------------------------------------------------------
void demoParticle::setForceFieldsPoint(vector<ofPoint> *fieldPoint) {
    forceFieldsPoint = fieldPoint;
}

//------------------------------------------------------------------
void demoParticle::reset(){
	//the unique val allows us to set properties slightly differently for each particle
	uniqueVal = ofRandom(-10000, 10000);
	
	pos.x = ofRandomWidth();
	pos.y = ofRandomHeight();
	
	vel.x = ofRandom(-1.0, 1.0);
	vel.y = ofRandom(-1.0, 1.0);
	
	frc   = ofPoint(0,0,0);
	
	scale = ofRandom(0.5, 1.0);
	
	if( mode == PARTICLE_MODE_NOISE ){
		drag  = ofRandom(0.97, 0.99);
		vel.y = fabs(vel.y) * 3.0; //make the particles all be going down
	}else{
		drag  = ofRandom(0.95, 0.998);	
	}
}

//------------------------------------------------------------------
void demoParticle::update(){

        if( forceFieldsPoint ){
            ofPoint sumFrc = ofPoint(0,0);
            for(unsigned int i = 0; i < forceFieldsPoint->size(); i++){
                ofPoint currentAttr = forceFieldsPoint->at(i) - pos;
                float dist = currentAttr.lengthSquared();
                if (dist < 12000) {
                    
                    float dist1 = currentAttr.lengthSquared();
                    sumFrc += currentAttr;
                }
            }
            frc = sumFrc;
            vel += frc * 0.0050;
            float len = vel.length();
            
            if (vel.length() > 1.5) {
                vel.scale(1.4);
            }
/*
            ofPoint currentAttr = forceFieldsPoint->at(1);
            printf("\nUser: %i | x: %f | y: %f", 1, currentAttr.x, currentAttr.y);
 */
        
        }
	
	
	//2 - UPDATE OUR POSITION
	
	pos += vel; 
	
	
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
void demoParticle::draw(){

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
	}
    else if( mode == PARTICLE_MODE_KINECT ){
		ofSetColor(255, 255, 255);
	}
			
	ofCircle(pos.x, pos.y, scale * 1.5);
}


