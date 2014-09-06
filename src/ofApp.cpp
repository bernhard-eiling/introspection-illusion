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

//vector<ofPoint> forceFieldsPoint;

// (float speed, float sep, float ali, float coh)
//BoidMachine boidMachineFast(2.0, 10.0, 3.0, 1.0);
//BoidMachine boidMachineFast(2.0, 5.0, 0.5, 0.1);
//BoidMachine boidMachineSlow(1.0, 5.0, 0.5, 0.1);
BoidMachine boidMachineFast(1.0, 5.0, 0.5, 0.1);
/*
BoidMachine boidMachineSlow(5.0, 10.0, 1.0, 0.3);
BoidMachine boidMachineFast(10.0, 10.0, 3.0, 1.0);
 */

//vector<demoParticle> p;
float xFactor = 1024.0 / 640.0;
float yFactor = 768.0 / 480.0;
bool useKinect = false;

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
    boidMachineFast.removeForceField(nId);
    //boidMachineSlow.removeForceField(nId - 1);
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
        boidMachineFast.addForceField(nId);
        //boidMachineSlow.addForceField(nId);
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
    
	currentModeStr = "KINECT ACTION";
	resetParticles();

    model1.loadModel("particle1.dae", true);
    model2.loadModel("particle2.dae", true);
    model3.loadModel("particle3.dae", true);
    model4.loadModel("particle4.dae", true);
    model5.loadModel("particle5.dae", true);
    model6.loadModel("particle6.dae", true);
    //bigMama.loadModel("bigMama.dae", true);
    modelArray.push_back(model1);
    modelArray.push_back(model2);
    modelArray.push_back(model3);
    modelArray.push_back(model4);
    modelArray.push_back(model5);
    modelArray.push_back(model6);
    
    //boidMachineFast.setModel(bigMama);
    boidMachineFast.setModelArray(modelArray);

    if (useKinect) setupKinect();
    
    ////////////////////////
    // TESTING
    boidMachineFast.addForceField(2);
    boidMachineFast.setPosForceField(2, 400, 400);
}

//--------------------------------------------------------------
void ofApp::resetParticles(){

	attractPoints.clear();
	for(int i = 0; i < 4; i++){
		attractPoints.push_back( ofPoint( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) ) );
	}
	attractPointsWithMovement = attractPoints;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (useKinect) updateKinect();
    boidMachineFast.update();
    //boidMachineSlow.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(0, 0);

	ofSetColor(230);
    string fpsStr = "FPS: " + ofToString(ofGetFrameRate(), 3);
    ofDrawBitmapString(fpsStr, 10,20);

    boidMachineFast.draw();
    //boidMachineSlow.draw();
    
    ///////////////
    // KINECT
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofEnableDepthTest();
}

///////////////////////////////////
// KINECT
//
void ofApp::updateKinect() {
    
    g_Context.WaitOneUpdateAll(g_UserGenerator);
    nUsers=MAX_NUM_USERS;
    g_UserGenerator.GetUsers(aUsers, nUsers);
    
    for(XnUInt16 i = 0; i < nUsers; i++) {
        if(g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i])) {
            
            g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_TORSO,torsoJoint);
            XnSkeletonJointPosition torsoPos;
            g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[i], XN_SKEL_TORSO, torsoPos);
            
            XnPoint3D torsoPoint;
            torsoPoint = torsoPos.position;
            
            g_DepthGenerator.ConvertRealWorldToProjective(1, &torsoPoint, &torsoPoint);
            
            //////////////////////
            // UPDATE FORCE FIELD
            boidMachineFast.setPosForceField(aUsers[i], torsoPoint.X * xFactor, torsoPoint.Y * xFactor);
            //boidMachineSlow.setPosForceField(aUsers[i], torsoPoint.X * xFactor, torsoPoint.Y * xFactor);
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

