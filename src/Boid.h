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
#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"

#endif /* defined(__introspection_illusion__Boid__) */

class Boid {
    
public:
    
    Boid();

    void update();
    void draw();
    
    void setForceFields(map<int, ForceField> *fields);
    void setIndex(int i);
    void setNeighbours(vector<Boid> *boids);
    void setModel(ofxAssimpModelLoader *model);
    void setWeights(float sp, float sep, float ali, float coh);
    float getLenght(ofVec2f vec);
    
    ofVec2f pos;
    ofVec2f vel;
    float rotate;
    vector<Boid> *neighbours;
    map<int, ForceField> *forceFields;
    ofxAssimpModelLoader *model;
    
    float sepWeight;
    float aliWeight;
    float cohWeight;
    float graviWeight;
    
private:
    ofVec2f gravitate();
    ofVec2f flock();
    ofVec2f steerTo(ofVec2f target);
    void stayOnScreen();
    void bounceScreen();
    
    float speed;
    float neighbourRadius;
    float seperationRadius;
    float gravitationRadius;
    float maxSpeed;
    float maxForce;
    float scale;
    

    
    int index;
};