//
//  Boid.cpp
//  introspection-illusion
//
//  Created by Bernhard Eiling on 10.04.14.
//
//

#include "Boid.h"

Boid::Boid() {
    speed = 1.0;
    neighbours = NULL;
    forceFields = NULL;
    neighbourRadius = 3500; // radius 50
    seperationRadius = 250; // radius 10
    gravitationRadius = 12000;
    maxSpeed = speed;
    maxForce = 0.5;
    //sepWeight = 2.0;
    sepWeight = ofRandom(1.0, 10.0);
    //aliWeight = 0.3;
    aliWeight = ofRandom(0.1, 4.0);
    //cohWeight = 0.1;
    cohWeight = ofRandom(0.05, 1.0);
    graviWeight = -0.05;
    scale = 1.5;//ofRandom(0.5, 1.0);
    
    pos.x = (float)(std::rand() % 1024);//500;//ofRandomWidth();
	pos.y = (float)(std::rand() % 768);//ofRandomHeight();
     
    /*
    pos.x = ofGetWidth() / 2;
	pos.y = ofGetHeight() / 2;
    */
    
    vel.x = ofRandom(-speed, speed);
	vel.y = ofRandom(-speed, speed);
    rotate = ofRandom(-100, 100);
}

void Boid::update() {
    ofVec2f gravity = ofVec2f(0.0, 0.0);
    if (forceFields != NULL) {
        gravity = gravitate();
    }
    ofVec2f acc = flock();
    vel = (vel + acc).limit(maxSpeed);
    vel = (vel + gravity).limit(maxSpeed);
    pos += vel;
    // SCALE UP BOID BY VELOCITY
    //scale = vel.length();
    
    stayOnScreen();
    //bounceScreen();
    rotate += 2.0;
}

void Boid::draw() {
    /*
    ofSetColor(255, 255, 255);
    ofCircle(pos.x, pos.y, scale * 1.0);
    */
    // MODEL
    ofPushMatrix();
    ofTranslate(pos.x, pos.y, 0);
    ofRotate(rotate, 0, 1, 1);
    model->drawFaces();
    ofPopMatrix();

    // PRINT NEIGHBOUR RADIUS
    /*
    if( index == 1) {
    ofNoFill();
    ofSetColor(255, 255, 255);
    ofCircle(pos.x, pos.y, 50);
    }
    */
}

ofVec2f Boid::gravitate() {
    ofVec2f grav = ofVec2f(0.0, 0.0);
    
    for(int i = 0; i < forceFields->size(); i++){
        if(forceFields->find(i) != forceFields->end()) {
            ofVec2f deltaVec = forceFields->at(i).torsoPos - pos;
            float deltaSqrt = deltaVec.lengthSquared();
            if (deltaSqrt < forceFields->at(i).radius) {
                grav += deltaVec;
        
            }
            grav *= forceFields->at(i).fieldStrenght;
        }
    }
    //grav *= graviWeight;
    
    
    return grav;
}

ofVec2f Boid::flock() {
    ofVec2f sep;
    ofVec2f ali;
    ofVec2f coh;
    
    int sepCount = 0;
    int aliCount = 0;
    int cohCount = 0;

    for(int i = 0; i < neighbours->size(); i++){
        
        // skip if currentBoid == this
        if (index != i) {
            Boid currentBoid = neighbours->at(i);
            ofVec2f delta = pos - currentBoid.pos;
            float dist = getLenght(delta);
        
            if (dist < seperationRadius) {
                sep += delta.normalize() / sqrt(dist);
                sepCount++;
            }
            if (dist < neighbourRadius) {
                ali += currentBoid.vel;
                aliCount++;
                coh += currentBoid.pos;
                cohCount++;
            }
        }
    }
    
    if (sepCount > 0) sep = sep / sepCount;
    if (aliCount > 0) ali = ali / aliCount;
    if (cohCount > 0) {
        // mean of all pos of neighbours
        coh = coh / cohCount;
    } else {
        // no neighbours -> nowhere to go
        coh = pos;
    }

    coh = steerTo(coh);
    ali = ali.limit(maxForce);
    
    sep *= sepWeight;
    ali *= aliWeight;
    coh *= cohWeight;

    return sep + ali + coh;
}

ofVec2f Boid::steerTo(ofVec2f target) {
    ofVec2f dist = target - pos; // vector from position to target
    dist = dist.normalize(); // Richtungsvektor to target
    dist *= maxSpeed;
    dist -= vel;
    dist.limit(maxForce);
    return dist;
}

void Boid::stayOnScreen() {
    if (pos.x > ofGetWidth()){
		pos.x = 0.0;
	} else if (pos.x < 0) {
		pos.x = ofGetWidth();
	}
	if (pos.y > ofGetHeight()) {
		pos.y = 0.0;
	} else if (pos.y < 0) {
		pos.y = ofGetHeight();
	}
}

void Boid::bounceScreen() {
    if (pos.x > ofGetWidth()){
		vel.x = -vel.x;
	} else if (pos.x < 0) {
		vel.x = -vel.x;
	}
	if (pos.y > ofGetHeight()) {
        vel.y = -vel.y;
	} else if (pos.y < 0) {
        vel.y = -vel.y;
	}
}

float Boid::getLenght(ofVec2f vec) {
    float lenght = vec.x * vec.x + vec.y * vec.y;
    return lenght;
}

void Boid::setNeighbours(vector<Boid> *boids) {
    neighbours = boids;
}

void Boid::setForceFields(map<int, ForceField> *fields) {
    forceFields = fields;
}

void Boid::setModel(ofxAssimpModelLoader *m) {
    model = m;
    //model->setScale(0.05, 0.05, 0.05);
    float scale = ofRandom(0.02, 0.07);
    model->setScale(scale, scale, scale);
}

void Boid::setWeights(float sp, float s, float a, float c) {
    speed = sp;
    sepWeight = s;
    aliWeight = a;
    cohWeight = c;
    
    maxSpeed = speed;
    vel.x = ofRandom(-speed, speed);
	vel.y = ofRandom(-speed, speed);
}

void Boid::setIndex(int i) {
    index = i;
}