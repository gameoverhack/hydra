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
	map< string, goVideoGrabber* >& cameras = _appModel->getCameras();

    int camWidth = boost::any_cast<int>(_appModel->getProperty("grabberWidth"));
    int camHeight = boost::any_cast<int>(_appModel->getProperty("grabberHeight"));

    //cameras.resize(MAX_CAMERAS);

	for (int i = 0; i < MAX_CAMERAS; i++) {
	    LOG_NOTICE("Init Camera " + ofToString(i) + " at " + ofToString(camWidth) + " x " + ofToString(camHeight));
		goVideoGrabber* camera = new goVideoGrabber();
		camera->setVerbose(true);
        camera->setDeviceID(i);
        camera->setRequestedMediaSubType(VI_MEDIASUBTYPE_YUY2);
        if (i == 2) { // TP3 loungeroom cam 1
            camera->setDesiredFrameRate(10);
            camera->initGrabber(160, 120);
        } else if (i == 3) { // TP3 loungeroom Cam 2
            camera->setDesiredFrameRate(13);
            camera->initGrabber(320, 240);
        } else {
            camera->setDesiredFrameRate(25);
            camera->initGrabber(camWidth, camHeight);
        }

        cameras.insert(pair< string, goVideoGrabber* >("camera_"+ofToString(i+1), camera));
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

    map< string, goVideoGrabber* >& cameras = _appModel->getCameras();

    map< string, goVideoGrabber* >::iterator it;
    for (it = cameras.begin(); it != cameras.end(); it++) {
        goVideoGrabber* camera = it->second;
        if (camera == NULL) continue;
        camera->update();
    }

    for (int i = 0; i < videoObjects.size(); i++) {
        switch (videoObjects[i]->_inputType) {
            case GO_VIDEO_CAMERA:
                if (!videoObjects[i]->_bAssigned) {
                    LOG_VERBOSE("Assign camera");
                    videoObjects[i]->_bAssigned = true;
                    videoObjects[i]->_camera = _appModel->getCamera(videoObjects[i]->_videoPath);
                }
                break;
            case GO_VIDEO_PLAYER:
                if (!videoObjects[i]->_bAssigned) {
                    LOG_VERBOSE("Assign player: " + videoObjects[i]->_videoName);
                    videoObjects[i]->_bAssigned = true;
                    map< string, goThreadedVideo* >& allVideos = _appModel->getAllVideos();
                    map< string, goThreadedVideo* >::iterator it = allVideos.find(videoObjects[i]->_videoName);
                    if (it == allVideos.end()) {
                        if (videoObjects[i]->_player == NULL) videoObjects[i]->_player = new goThreadedVideo();
                        videoObjects[i]->_player->loadMovie(videoObjects[i]->_videoPath);
                        ofAddListener(videoObjects[i]->_player->success, this, &VideoController::success);
                        ofAddListener(videoObjects[i]->_player->error, this, &VideoController::error);
                        allVideos.insert(pair< string, goThreadedVideo* >(videoObjects[i]->_videoName, videoObjects[i]->_player));
                    } else {
                        videoObjects[i]->_player = it->second;
                    }
                }
                videoObjects[i]->_player->update();
                break;
        }
    }



//    vector<goThreadedVideo*>& videos = _appModel->getVideos();
//    vector<goVideoGrabber*>& cameras = _appModel->getCameras();
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
void VideoController::success(const void * sender, string & path) {

    goThreadedVideo * video = (goThreadedVideo*)sender;

//    vector<goThreadedVideo*>& videos = _appModel->getVideos();
//
	LOG_NOTICE("Finsished loading: " + video->getCurrentlyPlaying());
//	//remove the event listeners
	ofRemoveListener(video->success, this, &VideoController::success);
	ofRemoveListener(video->error, this, &VideoController::error);
	video->setVolume(255);
	video->setLoopState(OF_LOOP_NONE);
//	// increment the counter for total number of movies loaded...this will cause the if statement in update to start loading again
//    videos[currentlyLoadingVideo]->setPaused(true);
//    videos[currentlyLoadingVideo]->setPosition(0.0f);
//
//	numberOfMoviesLoaded++;

}

//--------------------------------------------------------------
void VideoController::error(const void * sender, int & errCode) {

    goThreadedVideo * video = (goThreadedVideo*)sender;

//    vector<goThreadedVideo*>& videos = _appModel->getVideos();
//
	LOG_ERROR("Error loading:" + ofToString(errCode));
//	//remove the event listeners
	ofRemoveListener(video->success, this, &VideoController::success);
	ofRemoveListener(video->error, this, &VideoController::error);
//	// increment the counter for total number of movies loaded...this will cause the if statement in update to start loading again
//	numberOfMoviesLoaded++;
}
