/*
 *  VideoController.cpp
 *  Bunyip
 *
 *  Created by gameover on 24/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#include "VideoController.h"

//--------------------------------------------------------------
VideoController::VideoController() {

	LOG_NOTICE("Constructing VideoController");

    registerStates();

    _functionModel->registerFunction("VideoController::showVideoSettings", MakeDelegate(this, &VideoController::showVideoSettings));

    numberOfMovies = numberOfMoviesLoaded = 0;
	currentlyLoadingVideo = -1;

	// init camera grabbers
	map< string, ofVideoGrabber* >& cameras = _appModel->getCameras();

	for (int i = 0; i < MAX_CAMERAS; i++) {
	    LOG_NOTICE("Init Camera " + ofToString(i));// + " at " + ofToString(camWidth) + " x " + ofToString(camHeight));
		ofVideoGrabber* camera = new ofVideoGrabber();
		camera->setVerbose(true);

        camera->setRequestedMediaSubType(VI_MEDIASUBTYPE_UYVY);

        string deviceName = "Decklink Video Capture";
        if(i > 0) deviceName += " (" + ofToString(i+1) + ")";
        cout << "SETTING UP: " << deviceName << endl;

        camera->setDeviceID(deviceName);

//		if(i < 3) camera->setDeviceID(5 + i);
//        if(i >= 3) camera->setDeviceID(5 + i + 1);

        if(i == 0){
            camera->initGrabber(1920, 1080);
        }else if(i == 4){
            camera->initGrabber(1280, 720);
            //camera->videoSettings();
        }else{
            camera->initGrabber(720, 576);
        }
        camera->getDevices();
        //camera->listDevices();
        cameras.insert(pair< string, ofVideoGrabber* >("camera_"+ofToString(i+1), camera));
	}
//assert(false);
}

//--------------------------------------------------------------
VideoController::~VideoController() {
	LOG_NOTICE("Destroying VideoController");
}

// state registration
//--------------------------------------------------------------
void VideoController::registerStates() {

	LOG_VERBOSE("Registering States");

    registerState(kVIDEOCONTROLLER_READY, "kVIDEOCONTROLLER_READY");
	registerState(kVIDEOCONTROLLER_LOADING, "kVIDEOCONTROLLER_LOADING");

	_appModel->registerStatefulClass(typeid(*this).name());

	setState(kVIDEOCONTROLLER_READY);

}

//--------------------------------------------------------------
void VideoController::showVideoSettings(int cameraIndex){
    string cameraName = "camera_"+ofToString(cameraIndex);
    LOG_NOTICE("Showing Video Settings for " + cameraName);
    ofVideoGrabber* camera = _appModel->getCamera(cameraName);
    if(camera != NULL){
        camera->videoSettings();
    }else{
        LOG_ERROR(cameraName + " does not exist!");
    }
}

//--------------------------------------------------------------
void VideoController::update() {

    Scene* scene = _appModel->getCurrentScene();
    if (scene == NULL) return;

    vector<VideoObject*> videoObjects = scene->getVideos();

    map< string, ofVideoGrabber* >& cameras = _appModel->getCameras();

    if(boost::any_cast<bool>(_appModel->getProperty("showDebug"))){
        map< string, ofVideoGrabber* >::iterator it;
        for (it = cameras.begin(); it != cameras.end(); it++) {
            ofVideoGrabber* camera = it->second;
            if (camera == NULL) continue;
            camera->update();
        }
    }

    for (int i = 0; i < videoObjects.size(); i++) {
        switch (videoObjects[i]->_inputType) {
            case GO_VIDEO_CAMERA:
                if (!videoObjects[i]->_bAssigned) {
                    LOG_VERBOSE("Assign camera");
                    videoObjects[i]->_bAssigned = true;
                    videoObjects[i]->_camera = _appModel->getCamera(videoObjects[i]->_videoPath);
                }
                if(!boost::any_cast<bool>(_appModel->getProperty("showDebug")) &&
                   !videoObjects[i]->_updated){
                    videoObjects[i]->_camera->update();
                    videoObjects[i]->_updated = true;
                }
                break;
            case GO_VIDEO_PLAYER:
                if (!videoObjects[i]->_bAssigned) {
                    LOG_VERBOSE("Assign player: " + videoObjects[i]->_videoName);
                    videoObjects[i]->_bAssigned = true;
                    map< string, ofxThreadedVideo* >& allVideos = _appModel->getAllVideos();
                    map< string, ofxThreadedVideo* >::iterator it = allVideos.find(videoObjects[i]->_videoName);
                    if (it == allVideos.end()) {
                        if(videoObjects[i]->_player == NULL){
                            LOG_VERBOSE("New player for: " + videoObjects[i]->_videoName);
                            videoObjects[i]->_player = new ofxThreadedVideo();
                            videoObjects[i]->_player->setPixelFormat(OF_PIXELS_BGRA);
                        }
                        videoObjects[i]->_player->loadMovie(videoObjects[i]->_videoPath);
                        ofAddListener(videoObjects[i]->_player->threadedVideoEvent, this, &VideoController::threadedVideoEvent);
                        allVideos.insert(pair< string, ofxThreadedVideo* >(videoObjects[i]->_videoName, videoObjects[i]->_player));
                    } else {
                        videoObjects[i]->_player = it->second;
                    }
                }
                if(videoObjects[i]->_player->getVolume() != videoObjects[i]->_fVolume) videoObjects[i]->_player->setVolume(videoObjects[i]->_fVolume);
                if(videoObjects[i]->_player->getPan() != videoObjects[i]->_fPan) videoObjects[i]->_player->setPan(videoObjects[i]->_fPan);
                //if(!videoObjects[i]->_updated && !videoObjects[i]->isFrameNew()){
                    videoObjects[i]->_player->update();
                    videoObjects[i]->_updated = true;
                //}
                break;
        }
        videoObjects[i]->_updated = false;
    }

    //for (int i = 0; i < videoObjects.size(); i++) videoObjects[i]->_updated = false;
}

//--------------------------------------------------------------
void VideoController::threadedVideoEvent(ofxThreadedVideoEvent & e) {

    ofxThreadedVideo * video = e.video;

    if(e.eventType == VIDEO_EVENT_LOAD_OK){
        LOG_NOTICE("Finsished loading: " + video->getMoviePath());
        video->setVolume(1.0f);
        video->setPan(0.0f);
        video->setLoopState(OF_LOOP_NONE);
    }else{
        LOG_ERROR("Error loading:" + video->getEventTypeAsString(e.eventType));
    }

}
