#include "ofMain.h"
#include "ofApp.h"
#include "Boid.h"

//========================================================================
int main( ){

	//ofSetupOpenGL(1024,768, OF_WINDOW);
    int width = 1024;
    int height = 768;
    ofSetupOpenGL(width, height, OF_FULLSCREEN);
    ofRunApp( new ofApp());
}
