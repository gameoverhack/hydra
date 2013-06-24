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

    float controlWidth = boost::any_cast<float>(_appModel->getProperty("controlWidth"));
    float controlHeight = boost::any_cast<float>(_appModel->getProperty("controlHeight"));
    float outputWidth = boost::any_cast<float>(_appModel->getProperty("outputWidth"));
    float outputHeight = boost::any_cast<float>(_appModel->getProperty("outputHeight"));

	int x = 0;
	int y = 0;
	int width = controlWidth + outputWidth;
	int height = MAX(controlHeight, outputHeight);
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
