//
//  ForceField.cpp
//  introspection-illusion
//
//  Created by Bernhard Eiling on 11.04.14.
//
//

#include "ForceField.h"

ForceField::ForceField(int xPos, int yPos) {
    pos = ofVec2f(xPos, yPos);
}

ForceField::ForceField() {
    
}

void ForceField::update() {
    
}

void ForceField::draw() {
    ofSetColor(255, 0, 0);
    ofCircle(pos.x, pos.y, 10);
}