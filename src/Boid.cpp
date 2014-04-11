//
//  Boid.cpp
//  introspection-illusion
//
//  Created by Bernhard Eiling on 10.04.14.
//
//

#include "Boid.h"

Boid::Boid() {
    neighbours = NULL;
    neighbourRadius = 300;
    seperationRadius = 150;
    maxSpeed = 3;
    maxForce = 0.5;
    scale = ofRandom(0.5, 1.0);
    
    pos.x = ofRandomWidth();
	pos.y = ofRandomHeight();
    
    vel.x = ofRandom(0.0, 2.0);
	vel.y = ofRandom(0.0, 2.0);
}

void Boid::update() {
    ofVec2f acc = flock();
    vel += acc;
    pos += vel;
}

void Boid::draw() {
    ofSetColor(255, 255, 255);
    ofCircle(pos.x, pos.y, scale * 1.5);
}

ofVec2f Boid::flock() {
    ofVec2f sep = separation();
    ofVec2f ali = alignment();
    ofVec2f coh = cohesion();
    
    return sep + ali + coh;
}

ofVec2f Boid::separation() {
    ofVec2f mean;
    int count = 0;
    
    for(unsigned int i = 0; i < neighbours->size(); i++){
        
        Boid currentBoid = neighbours->at(i);
        ofVec2f delta = currentBoid.pos - pos;
        float lenght = getLenght(delta);
        if (lenght > 0 && lenght < neighbourRadius) {
            ofVec2f sub = currentBoid.pos - pos;
            mean += sub.normalize() / lenght;
            count++;
        }
    }
    if (count > 0) {
        return mean / count;
    } else {
        return mean;
    }
}

ofVec2f Boid::alignment() {
    ofVec2f mean;
    int count = 0;
    
    for(unsigned int i = 0; i < neighbours->size(); i++){
        
        Boid currentBoid = neighbours->at(i);
        ofVec2f delta = currentBoid.pos - pos;
        float lenght = getLenght(delta);
        if (lenght > 0 && lenght < neighbourRadius) {
            mean += currentBoid.pos;
            count++;
        }
    }
    if (count > 0) {
        return mean / count;
    } else {
        return mean.limit(maxForce);
    }
}

ofVec2f Boid::cohesion() {
    ofVec2f sum;
    int count = 0;
    
    for(unsigned int i = 0; i < neighbours->size(); i++){
        
        Boid currentBoid = neighbours->at(i);
        ofVec2f delta = currentBoid.pos - pos;
        float lenght = getLenght(delta);
        if (lenght > 0 && lenght < neighbourRadius) {
            sum += currentBoid.pos;
            count++;
        }
    }
    if (count > 1) {
        return steerTo(sum / count);
    } else {
        return sum;
    }
}

ofVec2f Boid::steerTo(ofVec2f target) {
    ofVec2f toTarget = target - pos;
    toTarget = toTarget.normalize();
    toTarget *= maxForce;
    toTarget -= vel;
    toTarget.limit(maxForce);
    return toTarget;
}

float Boid::getLenght(ofVec2f vec) {
    float lenght = vec.x * vec.x + vec.y * vec.y;
    return lenght;
}

void Boid::setNeighbours(vector<Boid> *boids) {
    neighbours = boids;
}