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
    
    ForceField(int user);
    
    void update();
    void draw();
    void setPos(float xTorso, float yTorso, float xLeftHand, float yLeftHand, float xRightHand, float yRightHand);
    void setPos(float xTorso, float yTorso);
    ofVec2f getTorsoPos();
    ofVec2f getLeftHandPos();
    ofVec2f getRightHandPos();
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
    
    int user;
    
    bool freeze;
    int freezeCounter;
};