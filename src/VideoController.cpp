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
    numberOfMovies = numberOfMoviesLoaded = 0;
	currentlyLoadingVideo = -1;

	// init camera grabbers
	map< string, ofVideoGrabber* >& cameras = _appModel->getCameras();

    int camWidth = boost::any_cast<int>(_appModel->getProperty("grabberWidth"));
    int camHeight = boost::any_cast<int>(_appModel->getProperty("grabberHeight"));

    //cameras.resize(MAX_CAMERAS);

	for (int i = 0; i < MAX_CAMERAS; i++) {
	    LOG_NOTICE("Init Camera " + ofToString(i) + " at " + ofToString(camWidth) + " x " + ofToString(camHeight));
		ofVideoGrabber* camera = new ofVideoGrabber();
		camera->setVerbose(true);

		if(i < 3) camera->setDeviceID(5 + i);
        if(i >= 3) camera->setDeviceID(5 + i + 1);

        camera->setRequestedMediaSubType(VI_MEDIASUBTYPE_UYVY);

        if(i == 0){
            camera->initGrabber(1920, 1080);
        }else if(i == 2){
            camera->initGrabber(1280, 720);
        }else{
            camera->initGrabber(720, 576);
        }

        cameras.insert(pair< string, ofVideoGrabber* >("camera_"+ofToString(i+1), camera));
	}

}

//--------------------------------------------------------------
VideoController::~VideoController() {
	LOG_NOTICE("Destroying VideoController");

//	// get appmodel ref to our video storage vector
//    vector<goThreadedVideo*>& videos = _appModel->getVideos();
//
//    // delete any old videos
//    if (videos.size() > 0) {
//         for (int i = 0; i < videos.size(); i++) {
//            LOG_VERBOSE("Deleting video: " + ofToString(i));
//            videos[i]->close();
//            delete videos[i];
//         }
//         videos.clear();
//    }
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
void VideoController::loadFolder(string path) {

//    setState(kVIDEOCONTROLLER_LOADING);
//
//    // list folder
//    dir.allowExt("mov");
//	numberOfMovies = dir.listDir(ofToDataPath(path));
//
//    // check for error
//    if (numberOfMovies == 0) {
//        LOG_ERROR("No movies in folder!!!");
//        setState(kVIDEOCONTROLLER_READY);
//        return;
//    } else if (numberOfMovies > 24) {
//        LOG_WARNING("LIMITING NUMBER OF MOVIES TO LOAD TO 24!!! SEEM TO HAVE A PROBLEM WITH MORE THAN THIS....");
//        numberOfMovies = 24;
//    }
//
//    // get appmodel ref to our video storage vector
//    vector<goThreadedVideo*>& videos = _appModel->getVideos();
//
//    // delete any old videos
//    if (videos.size() > 0) {
//         for (int i = 0; i < videos.size(); i++) {
//            videos[i]->close();
//            delete videos[i];
//         }
//         videos.clear();
//    }
//
//    // resize so usign the vector is quick (better than push)
//    videos.resize(numberOfMovies);
//
//    // allocate instances/memory
//    for (int i = 0; i < numberOfMovies; i++) {
//		videos[i] = new goThreadedVideo();
//	}
//
//	// use some variables to track where we're upto loading the movies
//	numberOfMoviesLoaded = 0;
//	currentlyLoadingVideo = -1;

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
                        if (videoObjects[i]->_player == NULL) videoObjects[i]->_player = new ofxThreadedVideo();
                        videoObjects[i]->_player->setPixelFormat(OF_PIXELS_BGRA);
                        videoObjects[i]->_player->loadMovie(videoObjects[i]->_videoPath);
                        ofAddListener(videoObjects[i]->_player->threadedVideoEvent, this, &VideoController::threadedVideoEvent);
                        allVideos.insert(pair< string, ofxThreadedVideo* >(videoObjects[i]->_videoName, videoObjects[i]->_player));
                    } else {
                        videoObjects[i]->_player = it->second;
                    }
                }
                if(videoObjects[i]->_player->getVolume() != videoObjects[i]->_fVolume) videoObjects[i]->_player->setVolume(videoObjects[i]->_fVolume);
                if(videoObjects[i]->_player->getPan() != videoObjects[i]->_fPan) videoObjects[i]->_player->setPan(videoObjects[i]->_fPan);
                if(!videoObjects[i]->_updated && !videoObjects[i]->isFrameNew()){
                    videoObjects[i]->_player->update();
                    videoObjects[i]->_updated = true;
                }
                break;
        }
        for (int i = 0; i < videoObjects.size(); i++) videoObjects[i]->_updated = false;
    }



//    vector<goThreadedVideo*>& videos = _appModel->getVideos();
//    vector<ofVideoGrabber*>& cameras = _appModel->getCameras();
//
//    if (checkState(kVIDEOCONTROLLER_LOADING)) {
//        // check if there are more movies to load
//        if (currentlyLoadingVideo < numberOfMoviesLoaded && numberOfMovies > numberOfMoviesLoaded) {
//            // increment the current video counter and start loading the movie
//            currentlyLoadingVideo++;
//            LOG_NOTICE("Started loading: " + dir.getPath(currentlyLoadingVideo));
//            videos[currentlyLoadingVideo]->loadMovie(dir.getPath(currentlyLoadingVideo));
//            // attach event listeners so we know when the movie is loaded
//            ofAddListener(videos[currentlyLoadingVideo]->success, this, &VideoController::success);
//            ofAddListener(videos[currentlyLoadingVideo]->error, this, &VideoController::error);
//        }
//
//        // check if we're currently loading a movie
//        if (currentlyLoadingVideo == numberOfMoviesLoaded && numberOfMovies > numberOfMoviesLoaded) {
//            // use psuedo draw and update to force the loading of movies without actually having to be showing them onscreen
//            // this is an ugly hack I use to load multiple movies without having to actually be drawing them to screen
//            videos[currentlyLoadingVideo]->psuedoUpdate();
//            videos[currentlyLoadingVideo]->psuedoDraw();
//        }
//
//        if (numberOfMovies == numberOfMoviesLoaded) {
//            setState(kVIDEOCONTROLLER_READY);
//        }
//    }
//
//    if (checkState(kVIDEOCONTROLLER_READY)) {
//        // 'really' update movies that are already loaded
//        for (int i = 0; i < numberOfMoviesLoaded; i++) {
//            videos[i]->update();
//        }
//    }
//
//    for (int i = 0; i < cameras.size(); i++) {
//        cameras[i]->update();
//    }

}

//--------------------------------------------------------------
void VideoController::threadedVideoEvent(ofxThreadedVideoEvent & e) {

    ofxThreadedVideo * video = e.video;

//    vector<goThreadedVideo*>& videos = _appModel->getVideos();
//

    if(e.eventType == VIDEO_EVENT_LOAD_OK){
        LOG_NOTICE("Finsished loading: " + video->getPath());
        video->setVolume(1.0f);
        video->setPan(0.0f);
        video->setLoopState(OF_LOOP_NONE);
    }else{
        LOG_ERROR("Error loading:" + video->getEventTypeAsString(e.eventType));
    }


//	//remove the event listeners
//	ofRemoveListener(video->success, this, &VideoController::success);
//	ofRemoveListener(video->error, this, &VideoController::error);

//	// increment the counter for total number of movies loaded...this will cause the if statement in update to start loading again
//    videos[currentlyLoadingVideo]->setPaused(true);
//    videos[currentlyLoadingVideo]->setPosition(0.0f);
//
//	numberOfMoviesLoaded++;

}

////--------------------------------------------------------------
//void VideoController::error(const void * sender, int & errCode) {
//
//    ofxThreadedVideo * video = (ofxThreadedVideo*)sender;
//
////    vector<goThreadedVideo*>& videos = _appModel->getVideos();
////
//	LOG_ERROR("Error loading:" + ofToString(errCode));
////	//remove the event listeners
//	ofRemoveListener(video->success, this, &VideoController::success);
//	ofRemoveListener(video->error, this, &VideoController::error);
////	// increment the counter for total number of movies loaded...this will cause the if statement in update to start loading again
////	numberOfMoviesLoaded++;
//}
