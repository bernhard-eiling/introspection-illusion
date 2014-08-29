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
    
    sepTolerance = .0;
    speed = sp;
    sep = s;
    ali = a;
    coh = c;
    maxSep = 100.0;
    minSep = s;
    sepChange = 0.01;

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
    varySep();
    
    for(int i = 0; i < boids.size(); i++) {
		boids[i].update();
	}
    /*
    for(int i = 0; i < forceFields.size(); i++) {
		forceFields[i].update();
	}
    
    for (auto &field : forceFields) {
        field.second.update();
    }
     */
}

void BoidMachine::draw() {
    glShadeModel(GL_SMOOTH);
    light.enable();
    light.setPosition(-300, -300, -30000);
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
  
    for(int i = 1; i < forceFields.size(); i++) {
		forceFields[i].draw();
	}

    /*
    for (auto &field : forceFields) {
        field.second.draw();
    }
     */
}

void BoidMachine::varySep() {

    randomizer = ofRandom(0.0, 1.0);
    if (randomizer > 0.99) {
        if (explode) {
            /*
            for(int i = 0; i < boids.size(); i++) {
                boids[i].sepWeight = 100.0;
                boids[i].cohWeight = coh;
            }
             */
            explode = false;
            //printf("explode: %d\n", explode);
        } else {
            /*
            for(int i = 0; i < boids.size(); i++) {
                boids[i].sepWeight = sep;
                boids[i].cohWeight = 4.0;
            }
             */
            explode = true;
            //printf("explode: %d\n", explode);
        }
    }
    if (explode) {
        if (sep <= maxSep) {
            for(int i = 0; i < boids.size(); i++) {
                sep += sepChange;
                boids[i].sepWeight = sep;
            }
        }
    } else {
        if (sep >= minSep) {
            for(int i = 0; i < boids.size(); i++) {
                sep -= sepChange;
                boids[i].sepWeight = sep;
            }

        }
    }
}

void BoidMachine::addForceField(int user) {
    forceFields.insert(make_pair(user, ForceField()) );
}

void BoidMachine::removeForceField(int user) {
    //printf("eraseForceField\n");
    //printf("# before: %lu\n", forceFields.size());
    forceFields.erase(user);
    //printf("# after: %lu\n", forceFields.size());
}

void BoidMachine::setPosForceField(int user, int xTorso, int yTorso, int xLeftHand, int yLeftHand, int xRightHand, int yRightHand) {
    forceFields.at(user).setPos(xTorso, yTorso, xLeftHand, yLeftHand, xRightHand, yRightHand);
}

void BoidMachine::setPosForceField(int user, int xTorso, int yTorso) {
    forceFields.at(user).setPos(xTorso, yTorso);
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
