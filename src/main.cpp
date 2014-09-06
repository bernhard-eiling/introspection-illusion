#include "ofMain.h"
#include "ofApp.h"
#include "Boid.h"

//========================================================================
int main( ){

    int width = 1024;
    int height = 768;
    ofSetupOpenGL(width, height, OF_FULLSCREEN);
    //ofSetupOpenGL(768, 768, OF_WINDOW);
    ofRunApp( new ofApp());
}
