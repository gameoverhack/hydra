/*
 *  AppView.cpp
 *  emptyExample
 *
 *  Created by gameover on 22/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#include "AppView.h"

#ifdef FENSTER
AppView::AppView(float x, float y, float width, float height, ofxFenster* window, string windowTitle) : BaseView(x, y, width, height, window, windowTitle) {
#else
AppView::AppView(float x, float y, float width, float height) : BaseView(x, y, width, height) {
#endif

	LOG_NOTICE("Constructing AppView");

    bCustomFullscreen = false;

    _functionModel->registerFunction("AppView::toggleFullscreen", MakeDelegate(this, &AppView::toggleFullscreen));
    _functionModel->registerFunction("AppView::changePattern", MakeDelegate(this, &AppView::changePattern));

    _shader.load(ofToDataPath("shaders/deinterlace.vert"), ofToDataPath("shaders/deinterlace.frag"));

//    currentPatternIndex = -1;
//    currentPattern = nextPattern = NULL;
}

AppView::~AppView() {
	LOG_NOTICE("Destroying AppView");
}

void AppView::registerStates() {

	LOG_VERBOSE("Registering States");

	registerState(kAPPVIEW_INIT, "kAPPVIEW_INIT");
	registerState(kAPPVIEW_SCENE, "kAPPVIEW_SCENE");
	registerState(kAPPVIEW_CALIBRATE, "kAPPVIEW_CALIBRATE");
	registerState(kAPPVIEW_LOAD, "kAPPVIEW_LOAD");

	_appModel->registerStatefulClass(typeid(*this).name());

	setState(kAPPVIEW_LOAD);

}

void AppView::update() {

    Scene* scene = _appModel->getCurrentScene();
    if (scene == NULL) return;

    vector<VideoObject*> videoObjects = scene->getVideos();

    string patternName = scene->getPatternName();
    if (patternName == "") return;

    vector<ofRectangle*> currentPattern = _appModel->getPattern(patternName);

    begin();
    {
        ofSetColor(255,255,255,255);
        for (int i = 0; i < videoObjects.size(); i++) {
            //if(!videoObjects[i]->isFrameNew()) continue;
            float x = currentPattern[i]->x;
            float y = currentPattern[i]->y;
            float w = currentPattern[i]->width;
            float h = currentPattern[i]->height;

            switch(videoObjects[i]->_inputType) {
                case GO_VIDEO_PLAYER:
                    videoObjects[i]->_player->draw(x,y,w,h);
                    break;
                case GO_VIDEO_CAMERA:
                    if(videoObjects[i]->_camera->getWidth() == 720){
                        _shader.begin(); // 1 is TP1 (ie presenter)
                        videoObjects[i]->_camera->getTextureReference().drawSubsection(x, y, w * 1024.0/720.0f, h, 12, 5, w - 10, h - 10);
                        _shader.end();
                    }else{
                        _shader.begin();
                        videoObjects[i]->_camera->draw(x,y,w,h);
                        _shader.end();
                    }
                    break;

            }
        }
    }
    end();
}

void AppView::toggleFullscreen() {
#ifdef FENSTER
    ofxFensterManager::get()->setActiveWindow(_window);
    ofToggleFullscreen();
#else
    LOG_VERBOSE("Trying to force fullscreen on Windows XP");

	int x = 0;
	int y = 0;
	int width = 1920 * 2;
	int height = 1080;
    long windowStyle;

	HWND vWnd  = FindWindow(NULL,  (LPCSTR)"hydra");

	if(!bCustomFullscreen){

		RECT r;

		if(GetWindowRect(vWnd, &r)){
			//more accurate as it takes windows elements into account
			originalRect.x = r.left;
			originalRect.y = r.top;
			originalRect.width = r.right - r.left;
			originalRect.height = r.bottom - r.top;
		}else{
			//fallback
			originalRect.x = 0;
			originalRect.y = 0;
			originalRect.width = ofGetWidth();
			originalRect.height = ofGetHeight();
		}

        originalStyle = GetWindowLong(vWnd, GWL_STYLE);
        windowStyle = originalStyle;

        windowStyle &= ~WS_OVERLAPPEDWINDOW;
        windowStyle |= WS_POPUP;


        bCustomFullscreen = true;
	}else{

        x = originalRect.x;
        y = originalRect.y;
        width = originalRect.width;
        height = originalRect.height;
        windowStyle = originalStyle;
        bCustomFullscreen = false;
	}

    SetWindowLong(vWnd, GWL_STYLE, windowStyle);
    SetWindowPos(vWnd, HWND_TOP, x, y, width, height, SWP_FRAMECHANGED);

#endif
}

void AppView::changePattern() {

    LOG_NOTICE("Changing pattern");
//
//    vector<goThreadedVideo*>& videos = _appModel->getVideos();
//    vector<goVideoGrabber*>& cameras = _appModel->getCameras();
//
//    if (videos.size() == 0 || cameras.size() == 0) return;
//
//    currentPatternIndex = (int)ofRandom(_patterns.size());
//
//    LOG_NOTICE("Selecting pattern " + ofToString(currentPatternIndex));
//
//    nextPattern = _patterns[currentPatternIndex];
//
//    for (int i = 0; i < nextPattern->layout.size(); i++) {
//        float flip = ofRandom(1);
//        if (flip <= 0.5) {
//            LOG_VERBOSE("Assign player");
//            nextPattern->layout[i].player = videos[(int)ofRandom(videos.size())];
//            nextPattern->layout[i].player->setPaused(false);
//            nextPattern->layout[i].inputType = GO_VIDEO_PLAYER;
//        } else {
//            LOG_VERBOSE("Assign camera");
//            nextPattern->layout[i].camera = cameras[(int)ofRandom(cameras.size())];
//            nextPattern->layout[i].inputType = GO_VIDEO_CAMERA;
//        }
//    }

}
