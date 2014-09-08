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
    boidEmitThreshold = 0.2; // smaller -> less Boids
    explodeCounter = 0;

    numBoids = 700;
    
    for (int i = 0; i < numBoids; i++) {
        Boid boid;// = new Boid();
        boid.setWeights(speed, sep, ali, coh);
        boids.push_back(boid);
    }

    for (int i = 0; i < boids.size(); i++) {
        boids.at(i).setIndex(i);
        boids.at(i).setNeighbours(boids);
        boids.at(i).setForceFields(forceFields);
    }
}

void BoidMachine::update() {
    
    varySep();
    
    for (auto &boid : boids) {
        boid.update();
    }
    for (auto &field : forceFields) {
        field.update();
    }
    
    ///////////////////////////
    // swarm interaction
    float randBoid = ofRandom(0.0, 1.0);
    if (boidEmitThreshold > randBoid) {
        for (auto &field : forceFields) {
            if (field.standing) {
                int randomIndex1 = ofRandom(0.0, boids.size());
                int randomIndex2 = ofRandom(0.0, boids.size());

                //boids.at(randomIndex).setPos(field.getPos());
                boids.at(randomIndex1).setPos(field.getLeftHandPos());
                boids.at(randomIndex2).setPos(field.getRightHandPos());
            }
        }
    }
    deleteLostForceFields();
}

void BoidMachine::draw() {
    glShadeModel(GL_SMOOTH);
    light.enable();
    light.setPosition(-300, -300, -30000);
    ofEnableSeparateSpecularLight();

    for (auto &boid : boids) {
        boid.draw();
    }
        /*
        for(int k = 0; k < forceFields.size(); k++) {
            
            ofLine(boids[i].pos.x, boids[i].pos.y, forceFields[k].torsoPos.x,forceFields[k].torsoPos.y);
        }

        for(int k = 0; k < boids.size(); k++) {
            ofLine(boids[i].pos.x, boids[i].pos.y, boids[k].pos.x, boids[k].pos.y);
        }
         */
    for (auto &field : forceFields) {
        field.draw();
    }
}

void BoidMachine::varySep() {
    
    randomizer = ofRandom(0.0, 1.0);
    if (randomizer > 0.996 && explode == false) {
        //printf("start explosion\n");
        explode = true;
    }
    
    if (explode) {
        explodeCounter++;
        if (sep <= maxSep) {
            for(int i = 0; i < boids.
                size(); i++) {
                sep += sepChange;
                boids.at(i).sepWeight = sep;
            }
        }
    } else {
        if (sep >= minSep) {
            for(int i = 0; i < boids.size(); i++) {
                sep -= sepChange;
                boids.at(i).sepWeight = sep;
            }

        }
    }
    // after 40 frames end explode
    if (explodeCounter > 30) {
        //printf("end explosion\n");
        explodeCounter = 0;
        explode = false;
    }
}

void BoidMachine::addForceField(int user) {
    ForceField field(user);
    forceFields.push_back(field);
}

void BoidMachine::removeForceField(int user) {
    for (int i = 0; i < forceFields.size(); i++) {
        if (forceFields.at(i).user == user) {
            forceFields.erase(forceFields.begin() + user);
        }
    }
}

void BoidMachine::setPosForceField(int user, float xTorso, float yTorso, float xLeftHand, float yLeftHand, float xRightHand, float yRightHand) {
    for (auto &field : forceFields) {
        if (field.user == user) {
            field.setPos(xTorso, yTorso, xLeftHand, yLeftHand, xRightHand, xRightHand);
        }
    }}

void BoidMachine::setPosForceField(int user, int xTorso, int yTorso) {
    for (auto &field : forceFields) {
        if (field.user == user) {
            field.setPos(xTorso, yTorso);
        }
    }
}

void BoidMachine::setModelArray(vector<ofxAssimpModelLoader> &m) {
    // MESH
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.
    //model->playAllAnimations();
    //model->setScale(0.05, 0.05, 0.05);
    
    for (int i = 0; i < boids.size(); i++) {
        int modelIndex = ofRandom(0, m.size());
        boids.at(i).setModel(m.at(modelIndex));
    }
}

void BoidMachine::setModel(ofxAssimpModelLoader &m) {
    // MESH
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.
    //model->playAllAnimations();
    //model->setScale(0.05, 0.05, 0.05);
    
    for (int i = 0; i < boids.size(); i++) {
        boids.at(i).setModel(m);
    }
}

void BoidMachine::deleteLostForceFields() {
    for (int i = 0; i < forceFields.size(); i++) {
        if (forceFields.at(i).freeze) {
            forceFields.at(i).setPos(-9999.0, -9999.0);
        }
    }
}
