/*
 *  MidiController.cpp
 *  Bunyip
 *
 *  Created by gameover on 23/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#include "MidiController.h"

//--------------------------------------------------------------
MidiController::MidiController() {

	LOG_NOTICE("Constructing MidiController");

	_midiIn.openPort();
    //_midiIn.setVerbose(true);

	_lastEvent.channel = -99;

    ofAddListener(_midiIn.newMessageEvent, this, &MidiController::midiMessage);

}

//--------------------------------------------------------------
MidiController::~MidiController() {
	LOG_NOTICE("Destructing MidiController");
}

//--------------------------------------------------------------
void MidiController::update() {
	if (_lastEvent.channel != -99) {
		MidiMessage midiMessage(_lastEvent.port, _lastEvent.channel, _lastEvent.status, _lastEvent.byteOne, _lastEvent.byteTwo, _lastEvent.timestamp);
		_midiModel->executeFunction(midiMessage);
		_lastEvent.channel = -99;
	}
}

//--------------------------------------------------------------
void MidiController::midiMessage(ofxMidiEventArgs &args) {
	_lastEvent = args;
}
