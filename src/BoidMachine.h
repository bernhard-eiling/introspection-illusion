//
//  BoidMachine.h
//  introspection-illusion
//
//  Created by Bernhard Eiling on 11.04.14.
//
//

#ifndef __introspection_illusion__BoidMachine__
#define __introspection_illusion__BoidMachine__

#pragma once

#include <iostream>
#include "ofMain.h"
#include "Boid.h"
#include "ForceField.h"
#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"
 
#endif /* defined(__introspection_illusion__BoidMachine__) */

class BoidMachine {
    
public:
    
    BoidMachine(float speed, float sep, float ali, float coh);
    
    void update();
    void draw();
    void varySep();
    void addForceField(int index);
    void removeForceField(int user);
    void setPosForceField(int user, int xTorso, int yTorso, int xLeftHand, int yLeftHand, int xRightHand, int yRightHand);
    void setModel(ofxAssimpModelLoader *m);
    
    int numBoids;
    float speed;
    float sep;
    float maxSep; // max seperation
    float minSep; // min seperation
    float sepTolerance; // margin of varying seperation force
    float sepChange; // amount of change of varying seperation force
    float ali;
    float coh;
    float randomizer;
    bool explode = false;
    
    vector<Boid> boids;
    map<int, ForceField> forceFields;
    ofxAssimpModelLoader *model;
    
    ofLight	light;
};