#include "ofApp.h"
#include <XnCppWrapper.h>

#define SAMPLE_XML_PATH "/users/bernhard/git/SamplesConfig.xml"
#define SAMPLE_XML_PATH_LOCAL "SamplesConfig.xml"
#define MAX_NUM_USERS 15

xn::Context g_Context;
xn::ScriptNode g_scriptNode;
xn::UserGenerator g_UserGenerator;
xn::DepthGenerator g_DepthGenerator;
xn::DepthMetaData g_depthMD;

XnBool g_bNeedPose = FALSE;
XnChar g_strPose[20] = "";

XnUserID aUsers[MAX_NUM_USERS];
XnUInt16 nUsers;
XnSkeletonJointTransformation torsoJoint;
XnSkeletonJointTransformation leftHand;
XnSkeletonJointTransformation rightHand;

vector<ofPoint> forceFieldsPoint;

//BoidMachine boidMachineFast(5.0, 10.0, 1.0, 0.3);
BoidMachine boidMachineFast(3.0, 10.0, 3.0, 1.0);
BoidMachine boidMachineSlow(1.0, 5.0, 0.5, 0.1);
//vector<demoParticle> p;
float xFactor = 1024.0 / 640.0;
float yFactor = 768.0 / 480.0;


//---------------------------------------------------------------------------
// CALLBACKS
//---------------------------------------------------------------------------
XnBool fileExists(const char *fn)
{
	XnBool exists;
	xnOSDoesFileExist(fn, &exists);
	return exists;
}

// Callback: New user was detected
void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d New User %d\n", epochTime, nId);
    // New user found
    if (g_bNeedPose)
    {
        g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
    }
    else
    {
        g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
    }
}
// Callback: An existing user was lost
void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Lost user %d\n", epochTime, nId);

    // REMOVE FORCEFIELD
    //boidMachine.removeForceField(nId - 1);
}
// Callback: Detected a pose
void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& /*capability*/, const XnChar* strPose, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Pose %s detected for user %d\n", epochTime, strPose, nId);
    g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
    g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}
// Callback: Started calibration
void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& /*capability*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Calibration started for user %d\n", epochTime, nId);
}

void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& /*capability*/, XnUserID nId, XnCalibrationStatus eStatus, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    if (eStatus == XN_CALIBRATION_STATUS_OK)
    {
        
        // Calibration succeeded
        printf("%d Calibration complete, start tracking user %d\n", epochTime, nId);
        g_UserGenerator.GetSkeletonCap().StartTracking(nId);
        
        // ADD FORCEFIELD
        //boidMachine.addForceField(nId);
    }
    else
    {
        // Calibration failed
        printf("%d Calibration failed for user %d\n", epochTime, nId);
        if(eStatus==XN_CALIBRATION_STATUS_MANUAL_ABORT)
        {
            printf("Manual abort occured, stop attempting to calibrate!");
            return;
        }
        if (g_bNeedPose)
        {
            g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
        }
        else
        {
            g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
        }
    }
}


#define CHECK_RC(nRetVal, what)					    \
if (nRetVal != XN_STATUS_OK)				    \
{								    \
printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));    \
return nRetVal;						    \
}


//--------------------------------------------------------------
//--------------------------------------------------------------
// openFrameworks
//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	/*
	int num = 40000;
	p.assign(num, demoParticle());
    */
    
	currentMode = PARTICLE_MODE_KINECT;

	currentModeStr = "KINECT ACTION";

	resetParticles();
    
   
    /*
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.
    
    bAnimate = false;
    bAnimateMouse = false;
    animationPosition = 0;
    
    //model.loadModel("particle_11.dae", true);
    model.loadModel("particles.dae", true);
    //model.loadModel("particle_anim.dae", true);
    model.setPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75 , 0);
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    if(!bAnimate) {
        model.setPausedForAllAnimations(true);
    }
    model.setScale(0.1, 0.1, 0.1);
    */
    //model.loadModel("astroBoy.dae", true);
    model.loadModel("particle_11.dae", true);
    //model.loadModel("particles1.dae", true);
    boidMachineFast.setModel(&model);
    boidMachineSlow.setModel(&model);

  //  setupKinect();
}



//--------------------------------------------------------------
void ofApp::resetParticles(){

	//these are the attraction points used qin the forth demo
	attractPoints.clear();
	for(int i = 0; i < 4; i++){
		attractPoints.push_back( ofPoint( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) ) );
	}
    
    forceFieldsPoint.clear();
	for(int i = 0; i <= MAX_NUM_USERS; i++){
		forceFieldsPoint.push_back( ofPoint( -10000.0, -10000.0 ) );
	}
	
	attractPointsWithMovement = attractPoints;
	/*
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setAttractPoints(&attractPointsWithMovement);
        p[i].setForceFieldsPoint(&forceFieldsPoint);
		p[i].reset();
	}	
     */
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //updateKinect();
    boidMachineFast.update();
    boidMachineSlow.update();
    /*
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].update();
	}
     */
    //boidMachine.setPosForceField(0, ofGetMouseX(), ofGetMouseY());
    
    ///////////////
    // MESH
    /*
    model.update();
    
    if(bAnimateMouse) {
        model.setPositionForAllAnimations(animationPosition);
    }
    */
    //mesh = model.getCurrentAnimatedMesh(0);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    //ofBackground(0,0,0, 10);
    ofBackground(0, 0);

	ofSetColor(230);
    string fpsStr = "FPS: " + ofToString(ofGetFrameRate(), 3);
    ofDrawBitmapString(fpsStr, 10,20);

    boidMachineFast.draw();
    boidMachineSlow.draw();
    
    ///////////////
    // KINECT
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
	ofEnableDepthTest();
    
    ///////////////
    // MESH
    /*
    model.drawFaces();
    light.enable();
    */
    /*
    ofSetColor(255);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
	ofEnableDepthTest();
    
    glShadeModel(GL_SMOOTH); //some model / light stuff
    light.enable();
    ofEnableSeparateSpecularLight();
    
    ofPushMatrix();
    ofTranslate(model.getPosition().x+100, model.getPosition().y, 0);
    ofRotate(-mouseX, 0, 1, 0);
    ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
    model.drawFaces();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(model.getPosition().x+300, model.getPosition().y, 0);
    ofRotate(-mouseX, 0, 1, 0);
    ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
    model.drawFaces();
    ofPopMatrix();
    
    if(ofGetGLProgrammableRenderer()){
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    }
    glEnable(GL_NORMALIZE);
    
    ofPushMatrix();
    ofTranslate(model.getPosition().x-300, model.getPosition().y, 0);
    ofRotate(-mouseX, 0, 1, 0);
    ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
    
    ofxAssimpMeshHelper & meshHelper = model.getMeshHelper(0);
    
    ofMultMatrix(model.getModelMatrix());
    ofMultMatrix(meshHelper.matrix);
    
    ofMaterial & material = meshHelper.material;
    if(meshHelper.hasTexture()){
        meshHelper.getTexturePtr()->bind();
    }
    material.begin();
    mesh.drawWireframe();
    material.end();
    if(meshHelper.hasTexture()){
        meshHelper.getTexturePtr()->unbind();
    }
    ofPopMatrix();
    
    if(ofGetGLProgrammableRenderer()){
    	glPopAttrib();
    }
    
    ofDisableDepthTest();
    light.disable();
    ofDisableLighting();
    ofDisableSeparateSpecularLight();
     */
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == '1'){
		currentMode = PARTICLE_MODE_ATTRACT;
		currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse"; 		
	}
	if( key == '2'){
		currentMode = PARTICLE_MODE_REPEL;
		currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse"; 				
	}
	if( key == '3'){
		currentMode = PARTICLE_MODE_NEAREST_POINTS;
		currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force"; 						
	}
	if( key == '4'){
		currentMode = PARTICLE_MODE_NOISE;
		currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation"; 						
		resetParticles();
	}
    if( key == '5'){
		currentMode = PARTICLE_MODE_KINECT;
		currentModeStr = "KINECT ACTION";
		resetParticles();
	}
		
	if( key == ' ' ){
		resetParticles();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::updateKinect() {
    
    g_Context.WaitOneUpdateAll(g_UserGenerator);
    // print the torso information for the first user already tracking
    nUsers=MAX_NUM_USERS;
    g_UserGenerator.GetUsers(aUsers, nUsers);
    for(XnUInt16 i=0; i<nUsers; i++) {
        if(g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i])) {
            
            g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_TORSO,torsoJoint);
            g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_LEFT_HAND,leftHand);
            g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_RIGHT_HAND,rightHand);
            
            XnSkeletonJointPosition torsoPos;
            XnSkeletonJointPosition leftHandPos;
            XnSkeletonJointPosition rightHandPos;
            g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[i], XN_SKEL_TORSO, torsoPos);
            g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[i], XN_SKEL_LEFT_HAND, leftHandPos);
            g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[i], XN_SKEL_RIGHT_HAND, rightHandPos);
            
            XnPoint3D torsoPoint;
            XnPoint3D leftHandPoint;
            XnPoint3D righthandPoint;
            torsoPoint = torsoPos.position;
            leftHandPoint = leftHandPos.position;
            righthandPoint = rightHandPos.position;
            
            g_DepthGenerator.ConvertRealWorldToProjective(1, &torsoPoint, &torsoPoint);
            g_DepthGenerator.ConvertRealWorldToProjective(1, &leftHandPoint, &leftHandPoint);
            g_DepthGenerator.ConvertRealWorldToProjective(1, &righthandPoint, &righthandPoint);
            
            // UPDATE FORCE FIELD
            //boidMachine.setPosForceField(aUsers[i - 1], torsoPoint.X * xFactor, torsoPoint.Y * xFactor, leftHandPoint.X * xFactor, leftHandPoint.Y * xFactor, righthandPoint.X * xFactor, righthandPoint.Y * xFactor);
        }
    }
}

void ofApp::setupKinect() {
    
    XnStatus nRetVal = XN_STATUS_OK;
    xn::EnumerationErrors errors;
    
    const char *fn = NULL;
    
    if    (fileExists(SAMPLE_XML_PATH)) fn = SAMPLE_XML_PATH;
    else if (fileExists(SAMPLE_XML_PATH_LOCAL)) fn = SAMPLE_XML_PATH_LOCAL;
    else {
        printf("Could not find '%s' nor '%s'. Aborting.\n" , SAMPLE_XML_PATH, SAMPLE_XML_PATH_LOCAL);
        return XN_STATUS_ERROR;
    }
    printf("Reading config from: '%s'\n", fn);
    
    
    nRetVal = g_Context.InitFromXmlFile(fn, g_scriptNode, &errors);
    if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
    {
        XnChar strError[1024];
        errors.ToString(strError, 1024);
        printf("%s\n", strError);
        return (nRetVal);
    }
    else if (nRetVal != XN_STATUS_OK)
    {
        printf("Open failed: %s\n", xnGetStatusString(nRetVal));
        return (nRetVal);
    }
    
    nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_USER, g_UserGenerator);
    if (nRetVal != XN_STATUS_OK)
    {
        nRetVal = g_UserGenerator.Create(g_Context);
        CHECK_RC(nRetVal, "Find user generator");
    }
    
    nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
    if (nRetVal != XN_STATUS_OK)
	{
		printf("No depth node exists! Check your XML.");
		return 1;
	}
    g_DepthGenerator.GetMetaData(g_depthMD);
    
    XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected;
    if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
    {
        printf("Supplied user generator doesn't support skeleton\n");
        return 1;
    }
    nRetVal = g_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
    CHECK_RC(nRetVal, "Register to user callbacks");
    nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationStart(UserCalibration_CalibrationStart, NULL, hCalibrationStart);
    CHECK_RC(nRetVal, "Register to calibration start");
    nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete(UserCalibration_CalibrationComplete, NULL, hCalibrationComplete);
    CHECK_RC(nRetVal, "Register to calibration complete");
    
    if (g_UserGenerator.GetSkeletonCap().NeedPoseForCalibration())
    {
        g_bNeedPose = TRUE;
        if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
        {
            printf("Pose required, but not supported\n");
            return 1;
        }
        nRetVal = g_UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected(UserPose_PoseDetected, NULL, hPoseDetected);
        CHECK_RC(nRetVal, "Register to Pose Detected");
        g_UserGenerator.GetSkeletonCap().GetCalibrationPose(g_strPose);
    }
    
    g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
    
    nRetVal = g_Context.StartGeneratingAll();
    CHECK_RC(nRetVal, "StartGenerating");
    
    
    printf("Starting to run\n");
    if(g_bNeedPose)
    {
        printf("Assume calibration pose\n");
    }
    
	/*
     g_scriptNode.Release();
     g_UserGenerator.Release();
     g_Context.Release();
     */
}

