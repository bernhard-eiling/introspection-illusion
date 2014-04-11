//
//  BoidMachine.cpp
//  introspection-illusion
//
//  Created by Bernhard Eiling on 11.04.14.
//
//

#include "BoidMachine.h"

BoidMachine::BoidMachine() {
    numBoids = 800;
    
    for (int i = 0; i < numBoids; i++) {
        boids.push_back(Boid());
    }
    for (int i = 0; i < boids.size(); i++) {
        boids.at(i).setIndex(i);
        boids.at(i).setNeighbours(&boids);
    }
}

void BoidMachine::update() {
    for(int i = 0; i < boids.size(); i++) {
		boids[i].update();
	}
}

void BoidMachine::draw() {
    for(int i = 0; i < boids.size(); i++) {
		boids[i].draw();
	}
    forceFields[1].draw();
}

void BoidMachine::setForceField(int index, int xPos, int yPos) {
    forceFields[index] = ForceField(xPos, yPos);
}