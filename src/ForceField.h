//
//  ForceField.h
//  introspection-illusion
//
//  Created by Bernhard Eiling on 11.04.14.
//
//

#ifndef __introspection_illusion__ForceField__
#define __introspection_illusion__ForceField__

#include <iostream>
#include "ofMain.h"

#endif /* defined(__introspection_illusion__ForceField__) */

class ForceField {
    
public:
    
    ForceField(int xPos, int yPos);
    ForceField();
    
    void update();
    void draw();
    
    ofVec2f pos;
};