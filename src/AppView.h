/*
 *  AppView.h
 *  emptyExample
 *
 *  Created by gameover on 22/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#ifndef _H_APPVIEW
#define _H_APPVIEW

#include "BaseView.h"
#include "BaseState.h"
#include "SceneDataType.h"

enum {
	kAPPVIEW_INIT,
	kAPPVIEW_SCENE,
	kAPPVIEW_CALIBRATE,
	kAPPVIEW_LOAD
};

class AppView : public BaseView, public BaseState {

public:

#ifdef FENSTER
	AppView(float x, float y, float width, float height, ofxFenster* window = NULL, string windowTitle = "Untitled");	//ctor
#else
    AppView(float x, float y, float width, float height);
#endif
    ~AppView();								//dtor

	void registerStates();

	void update();
	//void draw();

private:

    void toggleFullscreen();
    void changePattern();

    ofShader _shader;

    bool bCustomFullscreen;
    ofRectangle originalRect;
    long originalStyle;

//    vector<VideoPattern*> _patterns;
//
//    int currentPatternIndex;
//
//    VideoPattern * currentPattern;
//    VideoPattern * nextPattern;

protected:


};

#endif
