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
#include "ForceField.h"

#endif /* defined(__introspection_illusion__Boid__) */

class Boid {
    
public:
    
    Boid();

    void update();
    void draw();
    
    void setForceField(ForceField *forceField);
    void setIndex(int i);
    void setNeighbours(vector<Boid> *boids);
    float getLenght(ofVec2f vec);
    
    ofVec2f pos;
    ofVec2f vel;
    vector<Boid> *neighbours;
    
private:
    ofVec2f flock();
    ofVec2f steerTo(ofVec2f target);
    void stayOnScreen();
    void bounceScreen();
    
    float speed;
    float neighbourRadius;
    float seperationRadius;
    float maxSpeed;
    float maxForce;
    float scale;
    
    float sepWeight;
    float aliWeight;
    float cohWeight;
    
    int index;
};