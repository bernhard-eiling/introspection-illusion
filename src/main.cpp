#include "ofMain.h"
#include "ofApp.h"
#include "Boid.h"

//========================================================================
int main( ){

    int width = 800;
    int height = 800;
    //ofSetupOpenGL(width, height, OF_FULLSCREEN);
    ofSetupOpenGL(width, height, OF_WINDOW);
    ofRunApp( new ofApp());
}
