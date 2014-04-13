//
//  ForceField.cpp
//  introspection-illusion
//
//  Created by Bernhard Eiling on 11.04.14.
//
//

#include "ForceField.h"


ForceField::ForceField() {
    standing = 0;
    standingAccuracy = 10.0;
    fieldStrenght = 0.01;
    fieldMulti = 0.00001;
    radius = 0.0;
    radiusSqrt = 0.0;
}

void ForceField::update() {
    standing = isStanding();
    
    if (standing) {
        //fieldStrenght += fieldMulti;
        /*
        radiusSqrt += 100;
        radius = sqrt(radiusSqrt);
         */
        radius += 100;
    } else {
        //fieldStrenght = 0;
        radius = 0.0;
        radiusSqrt = 0.0;
    }
    
    lastTorsoPos = torsoPos;
    
    //printf("Standin: %d\n", standing);
}

void ForceField::draw() {
    
    if (standing == 1) {
        ofSetColor(0, 255, 0);
    } else {
        ofSetColor(255, 0, 0);
    }

    //ofCircle(torsoPos.x, torsoPos.y, 10);
    /*
    ofCircle(leftHandPos.x, leftHandPos.y, 5);
    ofCircle(rightHandPos.x, rightHandPos.y, 5);
     */
    ofNoFill();
    ofCircle(torsoPos.x, torsoPos.y, sqrt(radius));
}

bool ForceField::isStanding() {
    if (torsoPos.match(lastTorsoPos, standingAccuracy)) {
        return 1;
    } else {
        return 0;
    }
}

void ForceField::setPos(int xTorso, int yTorso, int xLeftHand, int yLeftHand, int xRightHand, int yRightHand) {
    torsoPos = ofVec2f(xTorso, yTorso);
    leftHandPos = ofVec2f(xLeftHand, yLeftHand);
    rightHandPos = ofVec2f(xRightHand, yRightHand);
}