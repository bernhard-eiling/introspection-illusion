#pragma once
#include "ofMain.h"
#include <XnCppWrapper.h>

enum particleMode{
	PARTICLE_MODE_ATTRACT = 0,
	PARTICLE_MODE_REPEL,
	PARTICLE_MODE_NEAREST_POINTS,
	PARTICLE_MODE_NOISE,
    PARTICLE_MODE_KINECT
};

class demoParticle{

	public:
		demoParticle();

		void setAttractPoints(vector<ofPoint> *attract);
        void setForceFieldsPoint(vector<ofPoint> *forceFieldsPoint);
    
		void reset();
		void update();
		void draw();		
		
		ofVec2f pos;
		ofVec2f vel;
		ofVec2f frc;
    
		float drag; 
		float uniqueVal;
		float scale;
		
		particleMode mode;
		
		vector<ofPoint> *attractPoints;
        vector<ofPoint> *forceFieldsPoint;
};