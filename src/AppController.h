/*
 *  AppController.h
 *  emptyExample
 *
 *  Created by gameover on 22/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#ifndef _H_APPCONTROLLER
#define _H_APPCONTROLLER

#include "ofMain.h"

#include "AppModel.h"
#include "BaseState.h"
#include "DataController.h"
#include "KeyboardController.h"
#include "MouseController.h"
#include "MidiController.h"
#include "VideoController.h"
#include "IOSController.h"
#include "OscController.h"

#include "AppView.h"
#include "GuiView.h"
#include "DebugView.h"

enum {
	kAPPCONTROLLER_INITING,
	kAPPCONTROLLER_LOADING,
	kAPPCONTROLLER_CALIBRATE,
	kAPPCONTROLLER_RUNNING,
	kAPPCONTROLLER_EXITING
};

class AppController : public ofBaseApp, public BaseState {

public:

	AppController();
	~AppController();

	void registerStates();

	void update();
	void draw();

    void exit();

private:

	DataController*			_dataController;
	KeyboardController*		_keyboardController;
	MouseController*		_mouseController;
	MidiController*			_midiController;
	OscController*			_oscController;
	VideoController*		_videoController;
	IOSController*           _IOSController;

	AppView*				_appView;
	GuiView*				_guiView;
	DebugView*              _debugView;

    float controlWidth;
    float controlHeight;
    float outputWidth;
    float outputHeight;

};

#endif
