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

    _shaderDeinterlace.load("shaders/deinterlace");
    _shaderDegrade.load("shaders/degrade");

    _appModel->setRenderTexture(getViewFBOTexture());

    crop720 = boost::any_cast<int>(_appModel->getProperty("crop720"));
    crop1280 = boost::any_cast<int>(_appModel->getProperty("crop1280"));
    crop1920 = boost::any_cast<int>(_appModel->getProperty("crop1920"));
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

            if(videoObjects[i]->_inputType == GO_VIDEO_PLAYER){
                videoObjects[i]->_player->draw(x, y, w, h);
            }else if(videoObjects[i]->_inputType == GO_VIDEO_CAMERA){
                float cW = videoObjects[i]->_camera->getWidth();
                float cH = videoObjects[i]->_camera->getHeight();
                if(cW == 720){
                    drawCamera(videoObjects[i], x, y, w, h, cW, cH, crop720);
                }else if(cW == 1280){
                    drawCamera(videoObjects[i], x, y, w, h, cW, cH, crop1280);
                }else{
                    drawCamera(videoObjects[i], x, y, w, h, cW, cH, crop1920);
                }
            }
        }
    }
    end();

}

void AppView::drawCamera(VideoObject* videoObject, float x, float y, float w, float h, float cW, float cH, float cPixels){

    if(videoObject->_bDegraded){

        if(videoObject->jamFBO.getWidth() != cW || videoObject->jamFBO.getHeight() != cH){
            videoObject->jamFBO.allocate(cW, cH);
        }

        if(ofGetElapsedTimeMillis() - videoObject->lastTime > videoObject->rMillis) videoObject->generateRandom();
        if(ofGetElapsedTimeMillis() - videoObject->lastJam > videoObject->jamMillis){
            videoObject->lastJam = ofGetElapsedTimeMillis();
            videoObject->doJam = ofRandom(10);
            videoObject->jamFBO.begin();
            {
                videoObject->_camera->getTextureReference().draw(0,0);
            }
            videoObject->jamFBO.end();
        }
        _shaderDegrade.begin();
            {
                if(videoObject->doJam < 3.5){
                    videoObject->jamFBO.getTextureReference().drawSubsection(x, y, w, h, cPixels, cPixels, cW - 2.0 * cPixels, cH - 2.0 * cPixels);
                }else{
                    videoObject->_camera->getTextureReference().drawSubsection(x, y, w, h, cPixels, cPixels, cW - 2.0 * cPixels, cH - 2.0 * cPixels);
                }

                _shaderDegrade.setUniform3f("rColor1", videoObject->rColor1.x, videoObject->rColor1.y, videoObject->rColor1.z);
                _shaderDegrade.setUniform3f("rColor2", videoObject->rColor2.x, videoObject->rColor2.y, videoObject->rColor2.z);
                _shaderDegrade.setUniform1f("rWidth", videoObject->rFactor);
                _shaderDegrade.setUniform1f("rHeight", videoObject->rFactor);
            }
        _shaderDegrade.end();

    }else{
        _shaderDeinterlace.begin();
        videoObject->_camera->getTextureReference().drawSubsection(x, y, w, h, cPixels, cPixels, cW - 2.0 * cPixels, cH - 2.0 * cPixels);
        _shaderDeinterlace.end();
    }
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
