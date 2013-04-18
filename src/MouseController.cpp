/*
 *  MouseController.cpp
 *  Bunyip
 *
 *  Created by gameover on 25/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#include "MouseController.h"

MouseController::MouseController() {

	LOG_NOTICE("Constructing MouseController");

	//registerStates();

	ofAddListener(ofEvents().mouseDragged, this, &MouseController::mouseMoved);
	ofAddListener(ofEvents().mouseDragged, this, &MouseController::mouseDragged);
	ofAddListener(ofEvents().mousePressed, this, &MouseController::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &MouseController::mouseReleased);

}

MouseController::~MouseController() {

	LOG_NOTICE("Destroying MouseController");

	ofRemoveListener(ofEvents().mouseDragged, this, &MouseController::mouseMoved);
	ofRemoveListener(ofEvents().mouseDragged, this, &MouseController::mouseDragged);
	ofRemoveListener(ofEvents().mousePressed, this, &MouseController::mousePressed);
	ofRemoveListener(ofEvents().mouseReleased, this, &MouseController::mouseReleased);

}

//--------------------------------------------------------------
void MouseController::mouseMoved(ofMouseEventArgs &e) {

}

//--------------------------------------------------------------
void MouseController::mouseDragged(ofMouseEventArgs &e) {

}

//--------------------------------------------------------------
void MouseController::mousePressed(ofMouseEventArgs &e) {

}

//--------------------------------------------------------------
void MouseController::mouseReleased(ofMouseEventArgs &e) {

}
