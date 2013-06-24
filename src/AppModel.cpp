/*
 *  AppModel.cpp
 *  emptyExample
 *
 *  Created by gameover on 22/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#include "AppModel.h"

//--------------------------------------------------------------
AppModel::AppModel() {

    LOG_NOTICE("Constructing AppModel");
	//_copyOfViewport = new ViewPort(640, 480);

    _padLength = 5;
	_currentScene = NULL;


	_patterns.clear();
	vector<ofRectangle*> pattern;

    // none
    _patterns.insert( pair< string, vector<ofRectangle*> >("0_none", pattern) );

    // single
	pattern.push_back(new ofRectangle(0,0,1920,1080));
	pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
	_patterns.insert( pair< string, vector<ofRectangle*> >("1_single", pattern) );
    pattern.clear();

    // double
	pattern.push_back(new ofRectangle(0,270,960,540));
    pattern.push_back(new ofRectangle(960,270,960,540));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    _patterns.insert( pair< string, vector<ofRectangle*> >("2_double", pattern) );
    pattern.clear();

    // triple
	pattern.push_back(new ofRectangle(0,360,640,360));
    pattern.push_back(new ofRectangle(640,360,640,360));
    pattern.push_back(new ofRectangle(1280,360,640,360));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    _patterns.insert( pair< string, vector<ofRectangle*> >("3_triple", pattern) );
    pattern.clear();

    // quad
	pattern.push_back(new ofRectangle(0,0,960,540));
    pattern.push_back(new ofRectangle(960,0,960,540));
    pattern.push_back(new ofRectangle(0,540,960,540));
    pattern.push_back(new ofRectangle(960,540,960,540));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    _patterns.insert( pair< string, vector<ofRectangle*> >("4_quad", pattern) );
    pattern.clear();

    // quadinset
    pattern.push_back(new ofRectangle(0,0,1920,1080));
	pattern.push_back(new ofRectangle(0,0,640,360));
    pattern.push_back(new ofRectangle(0,360,640,360));
    pattern.push_back(new ofRectangle(0,720,640,360));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    _patterns.insert( pair< string, vector<ofRectangle*> >("5_quadinset", pattern) );
    pattern.clear();

    // sextet
	pattern.push_back(new ofRectangle(0,180,640,360));
    pattern.push_back(new ofRectangle(640,180,640,360));
    pattern.push_back(new ofRectangle(1280,180,640,360));
    pattern.push_back(new ofRectangle(0,540,640,360));
    pattern.push_back(new ofRectangle(640,540,640,360));
    pattern.push_back(new ofRectangle(1280,540,640,360));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    pattern.push_back(new ofRectangle(-1920,-1080,0,0));
    _patterns.insert( pair< string, vector<ofRectangle*> >("6_sextet", pattern) );
    pattern.clear();

}

//--------------------------------------------------------------
AppModel::~AppModel() {

    LOG_NOTICE("Destroying AppModel - TODO...make this proper!!!");

}

/********************************************************
 *      Getters and setters for Scenes               	*
 ********************************************************/

//--------------------------------------------------------------
bool AppModel::loadScenes(string path) {
    LOG_VERBOSE("Attempt to load scene file: " + ofToDataPath(path));

    _currentScene = NULL;
    _scenes.clear();

    vector<Scene*> * scenes = new vector<Scene*>;

    if (loadVector(path, scenes)) {
        LOG_VERBOSE("Load success");

        for (int i = 0; i < scenes->size(); i++) {
            Scene* scene = scenes->at(i);
            vector<VideoObject*>& videoObjects = scene->getVideos();
            for (int j = 0; j < videoObjects.size(); j++) {
                if (videoObjects[j]->_inputType == GO_VIDEO_CAMERA) videoObjects[j]->_bAssigned = false;
                if (videoObjects[j]->_inputType == GO_VIDEO_PLAYER) {
                    LOG_VERBOSE("Load video: " + videoObjects[j]->_videoName);
                    map< string, ofxThreadedVideo* >::iterator it = _allVideos.find(videoObjects[j]->_videoName);
                    if (it == _allVideos.end()) {
                        videoObjects[j]->_bAssigned = true;
                        videoObjects[j]->_player = new ofxThreadedVideo();
                        videoObjects[j]->_player->setPixelFormat(OF_PIXELS_BGRA);
                        videoObjects[j]->_player->loadMovie(videoObjects[j]->_videoPath);
                        videoObjects[j]->_player->update();
                        while(videoObjects[j]->_player->isLoading()) {
                            videoObjects[j]->_player->update();
                            videoObjects[j]->_player->draw();
                        }
                        videoObjects[j]->_player->setLoopState(OF_LOOP_NONE);
                        _allVideos.insert(pair< string, ofxThreadedVideo* >(videoObjects[j]->_videoName, videoObjects[j]->_player));
                    } else {
                        videoObjects[j]->_player = it->second;
                    }


                }
            }
            addScene(scene);
        }

        char timestamp[255];
        sprintf(timestamp, "%02i%02i%i%02i%02i%02i", ofGetDay(), ofGetMonth(), ofGetYear(), ofGetHours(), ofGetMinutes(), ofGetSeconds());

        string backupPath = path + timestamp;
        saveScenes(backupPath);

        return true;
    } else {
        LOG_VERBOSE("Load error");
        return false;
    }
}

//--------------------------------------------------------------
bool AppModel::saveScenes(string path) {
    LOG_VERBOSE("Attempt to save scene file: " + ofToDataPath(path));

    vector<Scene*> * scenes = new vector<Scene*>;
    map<string, Scene*>::iterator it = _scenes.begin();

//    if (it == _scenes.end()) {
//        LOG_WARNING("No scenes to save!!!");
//        return false;
//    }

    for (it = _scenes.begin(); it != _scenes.end(); it++) scenes->push_back(it->second);

    if (saveVector(path, scenes)) {
        LOG_VERBOSE("Save success");
        return true;
    } else {
        LOG_VERBOSE("Save error");
        return false;
    }
}

//--------------------------------------------------------------
void AppModel::addScene(Scene * scene) {
	_scenes.insert(pair<string, Scene *>(scene->getName(), scene));
}

//--------------------------------------------------------------
void AppModel::delScene(Scene * scene) {

    if (scene == NULL) return;
    map<string, Scene *>::iterator it = _scenes.find(scene->getName());

    if (it == _scenes.end()) return;

    if (it->second == _currentScene) _currentScene = NULL;

    _scenes.erase(it);

}

//--------------------------------------------------------------
void AppModel::delAllScenes() {
    _currentScene = NULL;
    _scenes.clear();
}

Scene *AppModel::getScene(int sceneIndex) {
	//assert(sceneIndex < _scenes.size());
	int index = 0;
	map<string, Scene *>::iterator iter = _scenes.begin();
	if (iter == _scenes.end()) iter->second = NULL;
	for (iter = _scenes.begin(); iter != _scenes.end(); iter++, index++) if (index == sceneIndex) break;
	return iter->second;

}

Scene *AppModel::getScene(string sceneName) {

	map<string, Scene *>::iterator iter;
	iter = _scenes.find(sceneName);
	if(iter == _scenes.end()){
		LOG_ERROR("Attempted to get invalid scene name " + sceneName);
		return NULL;
	}
	return iter->second;

}

//void AppModel::setCurrentSceneToNone() {
//	_currentScene = NULL;
//}

//bool AppModel::deleteScene() {
//
//	if (_currentScene != NULL) {
//
//		LOG_NOTICE("Deleting current scene");
//
//		map<string, Scene *>::iterator iter;
//		map<string, Scene *>::iterator del;
//
//		bool renumber = false;
//		int	sceneIndex = 0;
//
//		for(iter = _scenes.begin(); iter != _scenes.end(); iter++) {
//			if (renumber) {
//				string name = "scene" + ofToString(sceneIndex);
//				LOG_VERBOSE("Renaming: " + iter->second->getName() + " to " + name);
//				const_cast<string &>(iter->first) = name;
//				iter->second->setName(name);
//			}
//			if (iter->second == _currentScene) {
//				LOG_VERBOSE("Flag for delete: " + iter->second->getName());
//				del = iter;
//				renumber = true;
//			} else {
//				LOG_VERBOSE("Not renaiming.: " + iter->second->getName());
//				sceneIndex++;
//			}
//		}
//
////		_currentScene->unregisterFunctions();
//		_currentScene = NULL;
//
//		delete del->second;
//		_scenes.erase(del);
//
//		return true;
//
//	} else {
//
//		LOG_NOTICE("No current scene to delete!");
//		return false;
//
//	}
//
//}

//bool AppModel::addScene(string name, Scene* scene) {
//
//	LOG_NOTICE("Adding new scene");
//	//string name = "scene" + ofToString((int)_scenes.size());
//	//Scene * scene = new Scene(name, 0, 0, 1024.0f, 768.0f);
//	setScene(scene);
//	return setCurrentScene(name);
//
//}

bool AppModel::rewindScene(Scene* scene) {
    if (scene == NULL) return false;
    vector<VideoObject*>& videoObjects = scene->getVideos();
    for (int i =0; i < videoObjects.size(); i++) {
        switch (videoObjects[i]->_inputType) {
            case GO_VIDEO_CAMERA:
                // nothing
                break;
            case GO_VIDEO_PLAYER:
                if (videoObjects[i]->_player != NULL) {
                    videoObjects[i]->_player->setFrame(0);
                    //videoObjects[i]->_player->setVolume(0);
                }
                break;
        }
    }
    return true;
}

bool AppModel::rewindAll() {
    map<string, Scene *>::iterator iter;
	for(iter = _scenes.begin(); iter != _scenes.end(); iter++) {
	    Scene* scene = iter->second;
	    if (scene == NULL) continue;
	    rewindScene(scene);
	}
    return true;
}

bool AppModel::setupScene(Scene* scene) {
    if (scene == NULL) return false;
    vector<VideoObject*>& videoObjects = scene->getVideos();
    for (int i =0; i < videoObjects.size(); i++) {
        switch (videoObjects[i]->_inputType) {
            case GO_VIDEO_CAMERA:
                // nothing
                break;
            case GO_VIDEO_PLAYER:
                //videoObjects[i]->_player->setFrame(0);
                if (videoObjects[i]->_player != NULL) {
                    videoObjects[i]->_player->setVolume(videoObjects[i]->_fVolume);
                    videoObjects[i]->_player->setPan(videoObjects[i]->_fPan);
                    if (videoObjects[i]->_player->getIsMovieDone()) videoObjects[i]->_player->setFrame(0);
                }
                break;
        }
    }
    return true;
}

bool AppModel::setCurrentScene(string sceneName) {

    Scene * lastScene = _currentScene;


	map<string, Scene *>::iterator sceneIter;
	sceneIter = _scenes.find(sceneName);
	if (sceneIter != _scenes.end()) {

		_currentScene = sceneIter->second; // TODO: DO we have to dereference this? TEST IT.
//		_currentScene->registerFunctions();

		LOG_NOTICE("Set current scene to " + sceneName);
		setupScene(_currentScene);
		//rewindScene(lastScene); // rewind the last scene
		return true;

	} else {
		LOG_ERROR("Unable to set current scene to '" + sceneName + "' because its not in the map");
	}

	return false;
}

bool AppModel::nextScene() {

	if (_currentScene == NULL) {
		if (_scenes.size() > 0) {
			setCurrentScene(_scenes.begin()->first);
			return true;
		} else return false;
	}

	string nextname;
	// find current scene in map
	map<string, Scene *>::iterator iter;
	for(iter = _scenes.begin(); iter != _scenes.end(); iter++) {
		if(iter->first == _currentScene->getName()){
			iter++; // found current, increment to next
			// is iter end? set name to first, else use iter name
			nextname = (iter == _scenes.end() ? _scenes.begin()->first : iter->first);
			setCurrentScene(nextname);
			LOG_VERBOSE("Next scene: " + nextname);
			return true;
		}
	}

	_currentScene = NULL;
	return false; // should never get here, can probably just be void return
}

bool AppModel::previousScene() {

	if (_currentScene == NULL) {
		if (_scenes.size() > 0) {
			setCurrentScene(_scenes.rbegin()->first);
			return true;
		} else return false;
	}

	string previousname;
	// find current scene in map
	map<string, Scene *>::iterator iter;
	for(iter = _scenes.begin(); iter != _scenes.end(); iter++){
		if(iter->first == _currentScene->getName()){
			iter--; // found current, decrement to previous
			// is iter end? set name to reverse first (ie last), else use iter name
			previousname = (iter == _scenes.end() ? _scenes.rbegin()->first : iter->first);
			setCurrentScene(previousname);
			LOG_VERBOSE("Previous scene: " + previousname);
			return true;
		}
	}

	_currentScene = NULL;
	return false; // should never get here, can probably just be void return
}

Scene * AppModel::getCurrentScene(){
	//assert(_currentScene != NULL);
	return _currentScene;
}

map<string, Scene*>& AppModel::getScenes() {
	return _scenes;
}

map< string, vector<ofRectangle*> >& AppModel::getPatterns() {
    return _patterns;
}

vector<ofRectangle*>& AppModel::getPattern(string patternName) {
    map< string, vector<ofRectangle*> >::iterator it;
	it = _patterns.find(patternName);
	assert(it != _patterns.end());
	return it->second;
}

int AppModel::getPatternIndex(string patternName) {
    map< string, vector<ofRectangle*> >::iterator it;
    int index = 0;
    int patternIndex = -1;
    for (it = _patterns.begin(); it != _patterns.end(); it++, index++) {
        if (it->first == patternName) {
            patternIndex = index;
            break;
        }
    }
    if (patternIndex == -1) LOG_WARNING("Pattern name not found returning an invalid index!!!");
    return patternIndex;
}

map< string, ofVideoGrabber* >& AppModel::getCameras() {
    return _cameras;
}

ofVideoGrabber* AppModel::getCamera(string cameraName) {
    map< string, ofVideoGrabber* >::iterator it;
	it = _cameras.find(cameraName);
	assert(it != _cameras.end());
	return it->second;
}

int AppModel::listVideoFolder(string path) {

    // list folder
    _videoDirectory.reset();
    _videoDirectory.allowExt("mov");
	int numberOfMovies = _videoDirectory.listDir(ofToDataPath(path));

    // check for error
    if (numberOfMovies == 0) LOG_ERROR("No movies in folder!!!" + ofToDataPath(path));

    return numberOfMovies;
}

map<int, IOSVideoPlayer*>& AppModel::getIOVideoPlayers(){
    return iosVideoPlayers;
}

//--------------------------------------------------------------
void AppModel::sendAllIPADosc(string address, string arg){
    ofxOscMessage m;
    m.setAddress(address);
    m.addStringArg(arg);
    cout << "SendAll: " << address << " : " << arg << endl;
    for(map<int, IOSVideoPlayer*>::iterator it = iosVideoPlayers.begin(); it != iosVideoPlayers.end(); it++){
        it->second->oscSender->sendMessage(m);
    }
}

//--------------------------------------------------------------
void AppModel::sendIPADosc(string address, int iosIPID, string arg){
    if(iosVideoPlayers.find(iosIPID) == iosVideoPlayers.end()){
        ofLogError() << "Ipad not connected on: " << iosIPID;
        return;
    }
    ofxOscMessage m;
    m.setAddress(address);
    m.addStringArg(arg);
    cout << "SendOne: " << iosIPID << " : " << address << " : " << arg << endl;
    iosVideoPlayers[iosIPID]->oscSender->sendMessage(m);
}

//--------------------------------------------------------------
string AppModel::getIOSAppStateAsString(IOSAppState appState){
    switch (appState) {
        case APP_INIT:
            return "APP_INIT";
            break;
        case APP_SYNC:
            return "APP_SYNC";
            break;
        case APP_PLAY:
            return "APP_PLAY";
            break;
        case APP_LIST:
            return "APP_LIST";
            break;
        case APP_WAIT:
            return "APP_WAIT";
            break;
    }
}
//bool AppModel::addServer(string portName, int port) {
//    ofxTCPServer* server = new ofxTCPServer();
//    if (server->setup(port, false)) {
//        LOG_VERBOSE("Server SUCCESS on port: " + portName);
//        TextServer* textServer = new TextServer();
//        textServer->setPort(port);
//        textServer->setName(portName);
//        textServer->setServer(server);
//        _servers.insert(pair< string, TextServer* >(portName, textServer));
//        return true;
//    } else {
//        LOG_VERBOSE("Server FAILED on port: " + portName);
//        return false;
//    }
//}
//
//bool AppModel::delServer(string portName) {
//    map< string, TextServer* >::iterator it = _servers.find(portName);
//    if (it == _servers.end()) return false;
//    _servers.erase(it);
//    return true;
//}
//
//map< string, TextServer* >& AppModel::getServers() {
//    return _servers;
//}
//
//TextServer* AppModel::getServer(string portName) {
//    map< string, TextServer* >::iterator it;
//	it = _servers.find(portName);
//	assert(it != _servers.end());
//	return it->second;
//}
//
//TextServer* AppModel::getServer(int portIndex) {
//	assert(portIndex < _servers.size());
//	int index = 0;
//	map< string, TextServer* >::iterator iter;
//	iter->second = NULL;
//	for (iter = _servers.begin(); iter != _servers.end(); iter++, index++) if (index == portIndex) break;
//	return iter->second;
//}

void AppModel::registerStatefulClass(string className) {

	LOG_NOTICE("Registering Stateful Class: " + className);

	map<string, string>::iterator it;
	it = _allStates.find(className);
	assert(it == _allStates.end()); // make sure we don't got it already

	_allStates.insert(pair<string, string>(className, "NONE"));

}

void AppModel::setRemoteState(string className, string state) {

	if (className.find("BaseState") != string::npos) return;

	map<string, string>::iterator it;
	it = _allStates.find(className);
	assert(it != _allStates.end()); // make sure we got it

	it->second = state;

}

string AppModel::getRemoteState(string className) {

	map<string, string>::iterator it;
	it = _allStates.find(className);
	assert(it != _allStates.end()); // make sure we got it

	return it->second;

}

string AppModel::getAllStatesAsList() {

	ostringstream out;

	map<string, string>::iterator it;
	for (it = _allStates.begin(); it != _allStates.end(); it++) {
		out << it->first << " [" << it->second << "]" << endl;
	}

	return out.str();

}

/********************************************************
 * Getters and setters for simple int, float, string	*
 * properties using boost::any and std::map. These		*
 * should NOT be used where efficiency really really	*
 * matters but still seem pretty quick!					*
 ********************************************************/

// set any property in a map to propVal
//--------------------------------------------------------------
void AppModel::setProperty(string propName, boost::any propVal) {

	if (_anyProps.count(propName) != 0 && !_anyProps[propName].empty()) {
		assert (_anyProps[propName].type() == propVal.type());	// don't let properties change types once init'd
	}

	_anyProps[propName] = propVal;

}

//--------------------------------------------------------------
void AppModel::adjustIntProperty(string propName, int amount) {

	setProperty(propName, boost::any_cast<int>(_anyProps[propName]) + amount);

}

//--------------------------------------------------------------
void AppModel::adjustFloatProperty(string propName, float amount) {

	setProperty(propName, boost::any_cast<float>(_anyProps[propName]) + amount);

}

// get any int property in a map (no cast necessary)
//--------------------------------------------------------------
void AppModel::getProperty(string propName, int & propVal) {

	assert(_anyProps.count(propName) != 0);	// if it ain't there abort
	assert(is_int(_anyProps[propName]));		// if it propVal ref is not same type abort
	propVal = boost::any_cast<int>(_anyProps[propName]);

}

// get any float property in a map (no cast necessary)
//--------------------------------------------------------------
void AppModel::getProperty(string propName, float & propVal) {

	assert(_anyProps.count(propName) != 0);	// if it ain't there abort
	assert(is_float(_anyProps[propName]));	// if it propVal ref is not same type abort
	propVal = boost::any_cast<float>(_anyProps[propName]);

}

// get any string property in a map (no cast necessary)
//--------------------------------------------------------------
void AppModel::getProperty(string propName, string & propVal) {

	assert(_anyProps.count(propName) != 0);	// if it ain't there abort
	assert(is_string(_anyProps[propName]));	// if it propVal ref is not same type abort
	propVal = boost::any_cast<string>(_anyProps[propName]);

}

// get any ANY property in a map (cast necessary -> use boost::any_cast<TYPE>(property))
//--------------------------------------------------------------
boost::any AppModel::getProperty(string propName) {
    //cout << propName << endl;
	assert(_anyProps.count(propName) != 0); // if it ain't there abort
	return _anyProps[propName];

}

// has any ANY property in a map?
//--------------------------------------------------------------
bool AppModel::hasProperty(string propName) {

	return (_anyProps.count(propName) == 0 ? false : true);

}

void AppModel::toggleBooleanProperty(string propName) {
	bool prop = boost::any_cast<bool>(getProperty(propName));
	setProperty(propName, !prop);
}

// return a list of ALL properties -> useful for debug
//--------------------------------------------------------------
string AppModel::getAllPropsAsList() {

	string propsList = "";

	map<string, boost::any>::iterator anyIT;
	for (anyIT = _anyProps.begin(); anyIT != _anyProps.end(); anyIT++) {

		string valAsString;
		string propAsString = anyIT->first;
		if (is_int(anyIT->second) == true) {
			valAsString = ofToString(boost::any_cast<int>(anyIT->second));
		}
		if (is_float(anyIT->second) == true) {
			valAsString = ofToString(boost::any_cast<float>(anyIT->second));
		}
		if (is_string(anyIT->second) == true) {
			valAsString = boost::any_cast<string>(anyIT->second);
		}
		if(is_bool(anyIT->second) == true){
			valAsString = ((boost::any_cast<bool>)(anyIT->second) ? "true" : "false");
		}

		propsList += pad(propAsString) + " = " + pad(valAsString) + " type: " + anyIT->second.type().name() + "\n";
	}

	return propsList;

}

//--------------------------------------------------------------
map<string, string> AppModel::getAllPropsNameTypeAsMap() {
	/*
	 returning as name,type instead of name,boost::any
	 beacuse we'd have to expose the is_int/etc to check the any types.
	 */
	map<string, string> retmap;

	map<string, boost::any>::iterator iter;
	for (iter = _anyProps.begin(); iter != _anyProps.end(); iter++) {
		/* add the name and type to the map */
		if (is_int(iter->second)){
			retmap.insert(pair<string,string>(iter->first, "int"));
		}
		if (is_float(iter->second)){
			retmap.insert(pair<string,string>(iter->first, "float"));
		}
		if (is_string(iter->second)){
			retmap.insert(pair<string,string>(iter->first, "string"));
		}
		if(is_bool(iter->second)){
			retmap.insert(pair<string,string>(iter->first, "bool"));
		}
	}
	return retmap;
}

//--------------------------------------------------------------
inline string AppModel::pad(string & t_string) {

	// possibly a more elegant sprintf solution for this but can't work out how to
	// dynamically set sprintf(objWithWhiteSpace, "%" + ofToString(_padLength) + "s", objectName) ???

	string paddedString = t_string;
	int padLength = 0;

	// check length and adjust overall pad if the objectName is longer than the current padLength
	if (t_string.size() > _padLength) _padLength = t_string.size();

	padLength = _padLength - t_string.size();

	for (int i = 0; i < padLength; i++) paddedString += " ";

	return paddedString;

}

// check type int
//--------------------------------------------------------------
inline bool AppModel::is_int(const boost::any & operand) {
    return operand.type() == typeid(int);
}

// check type float
//--------------------------------------------------------------
inline bool AppModel::is_float(const boost::any & operand) {
    return operand.type() == typeid(float);
}

// check type string
//--------------------------------------------------------------
inline bool AppModel::is_string(const boost::any & operand) {
    return operand.type() == typeid(string);
}

// check type bool
//--------------------------------------------------------------
inline bool AppModel::is_bool(const boost::any & operand) {
	return operand.type() == typeid(bool);
}

// check type char *
//--------------------------------------------------------------
inline bool AppModel::is_char_ptr(const boost::any & operand) {
    try {
		boost::any_cast<const char *>(operand);
        return true;
    } catch(const boost::bad_any_cast &) {
        return false;
    }
}
