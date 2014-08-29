//
//  ForceField.h
//  introspection-illusion
//
//  Created by Bernhard Eiling on 11.04.14.
//
//
#pragma once

#ifndef __introspection_illusion__ForceField__
#define __introspection_illusion__ForceField__

#include <iostream>
#include "ofMain.h"

#endif /* defined(__introspection_illusion__ForceField__) */

class ForceField {
    
public:
    
    ForceField();
    
    void update();
    void draw();
    void setPos(int xTorso, int yTorso, int xLeftHand, int yLeftHand, int xRightHand, int yRightHand);
    void setPos(int xTorso, int yTorso);
    bool isStanding();
    
    ofVec2f torsoPos;
    ofVec2f leftHandPos;
    ofVec2f rightHandPos;
    ofVec2f lastTorsoPos;
    
    bool standing;
    float standingAccuracy;
    float fieldStrenght;
    float fieldMulti;
    float radius;
    float radiusSqrt;
    
};