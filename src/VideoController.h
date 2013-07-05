/*
 *  VideoController.h
 *  Bunyip
 *
 *  Created by gameover on 24/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#ifndef _H_VIDEOCONTROLLER
#define _H_VIDEOCONTROLLER

#include "AppModel.h"
#include "Logger.h"
#include "BaseState.h"

#include "ofxThreadedVideo.h"

enum {
    kVIDEOCONTROLLER_READY,
	kVIDEOCONTROLLER_LOADING
};

class VideoController : public BaseState {

public:

	VideoController();
	~VideoController();

	void update();
	void registerStates();

    void loadFolder(string path);

	void threadedVideoEvent(ofxThreadedVideoEvent & e);

    void showVideoSettings(int cameraIndex);

private:

    ofDirectory dir;
    int numberOfMovies;
    int numberOfMoviesLoaded;
    int currentlyLoadingVideo;

};

#endif
