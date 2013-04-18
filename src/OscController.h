/*
 *  OscController.h
 *  Bunyip
 *
 *  Created by gameover on 1/09/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#ifndef _H_OSCCONTROLLER
#define _H_OSCCONTROLLER

#include "ofMain.h"

#include "ofxOsc.h"

#include "AppModel.h"
#include "OscModel.h"

class OscController : public ofBaseApp {

public:

	OscController();
	~OscController();

	void	update();

private:

	ofxOscReceiver	_oscIn;
	ofxOscSender	_oscOut;

};

#endif
