/*
 *  MouseController.h
 *  Bunyip
 *
 *  Created by gameover on 25/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#ifndef _H_MOUSECONTROLLER
#define _H_MOUSECONTROLLER

#include "AppModel.h"

class MouseController {

public:

	MouseController();
	~MouseController();

	//void registerStates();

private:

	void mouseMoved(ofMouseEventArgs &e);
	void mouseDragged(ofMouseEventArgs &e);
	void mousePressed(ofMouseEventArgs &e);
	void mouseReleased(ofMouseEventArgs &e);

};

#endif
