//
//  BoidMachine.cpp
//  introspection-illusion
//
//  Created by Bernhard Eiling on 11.04.14.
//
//

#include "BoidMachine.h"
#include "ForceField.h"

BoidMachine::BoidMachine(float sp, float s, float a, float c) {
    
    speed = sp;
    sep = s;
    ali = a;
    coh = c;

    numBoids = 600;
    
    for (int i = 0; i < numBoids; i++) {
        Boid boid; // = new Boid(float sep, float ali, float coh);
        boid.setWeights(speed, sep, ali, coh);
        boids.push_back(boid);
    }
    for (int i = 0; i < boids.size(); i++) {
        boids.at(i).setIndex(i);
        boids.at(i).setNeighbours(&boids);
        boids.at(i).setForceFields(&forceFields);
    }
}

void BoidMachine::update() {
    for(int i = 0; i < boids.size(); i++) {
		boids[i].update();
	}
    for(int i = 0; i < forceFields.size(); i++) {
		forceFields[i].update();
	}
    //printf("\nnumForceFields: %lu", forceFields.size());
}

void BoidMachine::draw() {
    glShadeModel(GL_SMOOTH); //some model / light stuff
    light.enable();
    ofEnableSeparateSpecularLight();

    for(int i = 0; i < boids.size(); i++) {
		boids[i].draw();
        /*
        for(int k = 0; k < forceFields.size(); k++) {
            
            ofLine(boids[i].pos.x, boids[i].pos.y, forceFields[k].torsoPos.x,forceFields[k].torsoPos.y);
        }
         */
        /*
        for(int k = 0; k < boids.size(); k++) {
            ofLine(boids[i].pos.x, boids[i].pos.y, boids[k].pos.x, boids[k].pos.y);
        }
         */
	}
    for(int i = 0; i < forceFields.size(); i++) {
		forceFields[i].draw();
	}
}

void BoidMachine::addForceField(int user) {
   // forceFields.insert(std::make_pair(user, ofVec2f()));

}

void BoidMachine::removeForceField(int user) {
    forceFields.erase(user);
}

void BoidMachine::setPosForceField(int user, int xTorso, int yTorso, int xLeftHand, int yLeftHand, int xRightHand, int yRightHand) {
    forceFields[user].setPos(xTorso, yTorso, xLeftHand, yLeftHand, xRightHand, yRightHand);
}

void BoidMachine::setModel(ofxAssimpModelLoader *m) {
    model = m;
    // MESH
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.
    
    model->playAllAnimations();
    //model->setScale(0.05, 0.05, 0.05);
    
    for (int i = 0; i < boids.size(); i++) {
        boids.at(i).setModel(model);
    }
}
