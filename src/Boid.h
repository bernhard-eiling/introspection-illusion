//
//  Boid.h
//  introspection-illusion
//
//  Created by Bernhard Eiling on 10.04.14.
//
//

#ifndef __introspection_illusion__Boid__
#define __introspection_illusion__Boid__

#pragma once

#include <iostream>
#include "ofMain.h"

#endif /* defined(__introspection_illusion__Boid__) */

class Boid {
    
public:
    
    Boid();
    
    void reset();
    void update();
    void draw();
    
    void setNeighbours(vector<Boid> *boids);
    float getLenght(ofVec2f vec);
    
    ofVec2f pos;
    ofVec2f vel;
    vector<Boid> *neighbours;
    
private:
    ofVec2f flock();
    ofVec2f separation();
    ofVec2f alignment();
    ofVec2f cohesion();
    ofVec2f steerTo(ofVec2f target);
    
    int neighbourRadius;
    int seperationRadius;
    int maxSpeed;
    float maxForce;
    float scale;
};