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

#endif /* defined(__introspection_illusion__BoidMachine__) */

class BoidMachine {
    
public:
    
    BoidMachine();
    
    void update();
    void draw();
    void setForceField(int index, int xPos, int yPos);
    
    int numBoids;
    
    vector<Boid> boids;

    map<int, ForceField> forceFields;
};