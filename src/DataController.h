/*
 *  DataController.h
 *  Bunyip
 *
 *  Created by gameover on 24/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#ifndef _H_DATACONTROLLER
#define _H_DATACONTROLLER

#include <string>
using std::string;

#include "ofxXmlSettings.h"
#include "goDirList.h"

#include "BaseState.h"
#include "Logger.h"
#include "AppModel.h"
#include "PropertyXMLParser.h"
#include "PropertyXMLBuilder.h"


enum {
	kDATACONTROLLER_READY,
	kDATACONTROLLER_PROPERTY_PARSING,
	kDATACONTROLLER_PROPERTY_BUILDING,
	kDATACONTROLLER_SCENE_PARSING,
	kDATACONTROLLER_SCENE_BUILDING,
	kDATACONTROLLER_FINISHED
};

class DataController : public BaseState {

public:

	DataController();
	~DataController();

	void registerStates();

	void loadProperties(string configFilePath);
	void saveProperties(string configFilePath = "");

private:

	string					_configFilePath;
	string					_scenesFilePath;

};

#endif
