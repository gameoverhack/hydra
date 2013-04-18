/*
 *  DebugView.h
 *  Bunyip
 *
 *  Created by gameover on 23/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */


#ifndef _H_DEBUGVIEW
#define _H_DEBUGVIEW

#include "BaseView.h"
#include "AppModel.h"
#include "KeyboardModel.h"

class DebugView : public BaseView {

public:

	DebugView(float x, float y, float width, float height, ofxFenster* window = NULL, string windowTitle = "Untitled");

	void update();

private:

    void drawCameraViews();
    void drawPrompterViews();
    void drawOutputView();

    ofTrueTypeFont verdana;

    vector<ofFbo*> _prompterScreens;

};

#endif
