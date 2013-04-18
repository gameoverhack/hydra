/*
 *  OscController.cpp
 *  Bunyip
 *
 *  Created by gameover on 1/09/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#include "OscController.h"

OscController::OscController() {

	LOG_NOTICE("Constructing OscController");

	_oscIn.setup( 60000 );

	_oscOut.setup( "10.1.1.10", 60001);

}

OscController::~OscController() {

	LOG_NOTICE("Destroying OscController");

}

void OscController::update() {

	// do rec osc commands
	while( _oscIn.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		_oscIn.getNextMessage(&m);

		cout << m.getAddress() << " :: " << m.getArgTypeName(0) << endl;

		switch (m.getNumArgs()) {
			case 0:
			{
				OscMessage oscMessage(m.getAddress(), (double)0);
				_oscModel->executeFunction(oscMessage);
				break;
			}
			case 1:
			{
				switch (m.getArgType(0)) {
					case OFXOSC_TYPE_FLOAT:
					{
						OscMessage oscMessage(m.getAddress(), m.getArgAsFloat(0), (double)0);
						_oscModel->executeFunction(oscMessage);
						break;
					}
					case OFXOSC_TYPE_INT32:
					{
						OscMessage oscMessage(m.getAddress(), m.getArgAsInt32(0), (double)0);
						_oscModel->executeFunction(oscMessage);
						break;
					}
					case OFXOSC_TYPE_STRING:
					{
						OscMessage oscMessage(m.getAddress(), m.getArgAsString(0), (double)0);
						_oscModel->executeFunction(oscMessage);
						break;
					}
					default:
						LOG_WARNING("OSC Execute cannot process this type of oscArgument: " + m.getArgTypeName(0));
						break;
				}
				break;
			}
			case 2:
			{
				if (m.getArgType(0) != m.getArgType(0)) {
					LOG_WARNING("OSC Execute can only handle two oscArguments of the same type!");
					break;
				}

				switch (m.getArgType(0)) {
					case OFXOSC_TYPE_FLOAT:
					{
						OscMessage oscMessage(m.getAddress(), m.getArgAsFloat(0), m.getArgAsFloat(1), (double)0);
						_oscModel->executeFunction(oscMessage);
						break;
					}
					case OFXOSC_TYPE_INT32:
					{
						OscMessage oscMessage(m.getAddress(), m.getArgAsInt32(0), m.getArgAsInt32(1), (double)0);
						_oscModel->executeFunction(oscMessage);
						break;
					}
					case OFXOSC_TYPE_STRING:
					{
						OscMessage oscMessage(m.getAddress(), m.getArgAsString(0), m.getArgAsString(1), (double)0);
						_oscModel->executeFunction(oscMessage);
						break;
					}
					default:
						LOG_WARNING("OSC Execute cannot process these types of oscArgument: " + m.getArgTypeName(0));
						break;
				}
				break;
			}
			default:
				LOG_WARNING("OSC Execute cannot process this number of arguments!");
				break;
		}

	}

	// do send osc commands


}
