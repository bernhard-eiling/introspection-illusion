//
//  ForceField.cpp
//  introspection-illusion
//
//  Created by Bernhard Eiling on 11.04.14.
//
//

#include "ForceField.h"


ForceField::ForceField(int i) {
    user = i;
    standing = false;
    standingAccuracy = 3.0;
    fieldStrenght = 0.3;
    fieldMulti = 0.00001;
    radius = 0.0;
    radiusSqrt = 0.0;
    freezeCounter = 0;
}

void ForceField::update() {
    if (lastTorsoPos == torsoPos) {
        freezeCounter++; // if ForceField didnt move imcrement freezeCounter
        if (freezeCounter > 60) {
            freeze = true;
        }
    } else {
        freezeCounter = 0;
        freeze = false;
    }
    standing = isStanding();
    lastTorsoPos = torsoPos;
}

void ForceField::draw() {
    ofPushStyle();
    if (standing == 1) {
        ofSetColor(0, 255, 0);
    } else {
        ofSetColor(255, 0, 0);
    }

    ofCircle(torsoPos.x, torsoPos.y, 50);
    /*
    ofCircle(leftHandPos.x, leftHandPos.y, 5);
    ofCircle(rightHandPos.x, rightHandPos.y, 5);
     */
    //ofNoFill();
    //ofCircle(torsoPos.x, torsoPos.y, sqrt(radius));
    ofPopStyle();
}

bool ForceField::isStanding() {
    if (torsoPos.match(lastTorsoPos, standingAccuracy)) {
        return 1;
    } else {
        return 0;
    }
}

void ForceField::setPos(float xTorso, float yTorso, float xLeftHand, float yLeftHand, float xRightHand, float yRightHand) {
    torsoPos = ofVec2f(xTorso, yTorso);
    leftHandPos = ofVec2f(xLeftHand, yLeftHand);
    rightHandPos = ofVec2f(xRightHand, yRightHand);
}

void ForceField::setPos(float xTorso, float yTorso) {
    torsoPos = ofVec2f(xTorso, yTorso);
}

ofVec2f ForceField::getTorsoPos() {
    return torsoPos;
}
ofVec2f ForceField::getLeftHandPos() {
    return leftHandPos;
}
ofVec2f ForceField::getRightHandPos() {
    return rightHandPos;
}