#pragma once

#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>
#include <XnPropNames.h>

#include "ofMain.h"
#include "BoidMachine.h"

#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"
 

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void resetParticles();
        void setupKinect();
        void updateKinect();

		string currentModeStr;

		vector<ofPoint> attractPoints;
		vector<ofPoint> attractPointsWithMovement;
    
        /////////////////
        // MESH
        ofxAssimpModelLoader model1;
        ofxAssimpModelLoader model2;
        ofxAssimpModelLoader model3;
        ofxAssimpModelLoader model4;
        ofxAssimpModelLoader model5;
        ofxAssimpModelLoader model6;
        ofxAssimpModelLoader bigMama;


        ofLight	light;
        vector<ofxAssimpModelLoader> modelArray;
    
        bool bAnimate;
        bool bAnimateMouse;
        float animationPosition;
    
        /////////////////
        // KINECT
    /*
    xn::Context g_Context;
    xn::ScriptNode g_scriptNode;
    xn::UserGenerator g_UserGenerator;

    
    ~ofApp() {
        g_scriptNode.Release();
        g_UserGenerator.Release();
        g_Context.Release();
    }
    */
};
