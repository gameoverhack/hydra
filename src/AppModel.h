/*
 *  AppModel.h
 *  emptyExample
 *
 *  Created by gameover on 22/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#ifndef _H_APPMODEL
#define	_H_APPMODEL

#define MAX_CAMERAS 4
#define MAX_SERVERS 3

#include "goThreadedVideo.h"
#include "goVideoGrabber.h"

#include "Singleton.h"
#include "FunctionModel.h"
#include "SceneDataType.h"
#include "SerializationUtils.h"

#include "ofxNetwork.h"

#include <boost/any.hpp>

#include <map>
#include <vector>
#include <assert.h>
#include <string>

using std::map;
using std::vector;
using std::string;

class AppModel {

public:

	AppModel();
	~AppModel();

	//void					registerStates();

	// scene and sequence getter/setters

	bool                    loadScenes(string path);
	bool                    saveScenes(string path);

	void					addScene(Scene* scene);
	void                    delScene(Scene* scene);
	void                    delAllScenes();

	bool					setCurrentScene(string sceneName);

	Scene*					getCurrentScene();

	Scene*					getScene(string sceneName);
	Scene*					getScene(int sceneIndex);

	map<string, Scene*>&	getScenes();

    bool					nextScene();
	bool					previousScene();

    bool                    rewindScene(Scene* scene);
    bool                    rewindAll();
    bool                    setupScene(Scene* scene);
//	bool					deleteScene();
//	bool					addScene();

	int						getNumberOfScenes() {return _scenes.size();};

    // generic property getter/setters
	bool					hasProperty(string propName);
	void					setProperty(string propName, boost::any propVal);

	// grrr
	void					adjustIntProperty(string propName, int amount);
	void					adjustFloatProperty(string propName, float amount);

	void					getProperty(string propName, int & propVal);
	void					getProperty(string propName, float & propVal);
	void					getProperty(string propName, string & propVal);

	boost::any				getProperty(string propName);

	void					toggleBooleanProperty(string propName);

	string					getAllPropsAsList();
	map<string, string>		getAllPropsNameTypeAsMap();

	void					registerStatefulClass(string className);
	void					setRemoteState(string className, string state);
	string					getRemoteState(string className);

	string					getAllStatesAsList();

//    void                    copyViewPort(ViewPort* toCopy);
//    void                    pasteViewPort(ViewPort* toPaste);

    // video types
    map< string, goVideoGrabber* >&     getCameras();
    goVideoGrabber*                     getCamera(string cameraName);
    int                                 listVideoFolder(string path);
    ofDirectory&                        getVideoDirectory() {return _videoDirectory;};

    // patterns
    map< string, vector<ofRectangle*> >&        getPatterns();
    vector<ofRectangle*>&                       getPattern(string patternName);
    int                                         getPatternIndex(string patternName);

    // text/servers
    int                             listTextFolder(string path);
    ofDirectory&                    getTextDirectory() {return _textDirectory;};

    map< string, string >&          getTexts();
    string                          getText(string fileName);

    void					        addPreset(TextObject* preset);
	void                            delPreset(TextObject* preset);
	void                            delAllPresets();

    bool                            loadPresets(string path);
    bool                            savePresets(string path);

    map< string, TextObject* >&     getPresets();
    TextObject*                     getPreset(string presetName);
    TextObject*                     getPreset(int presetIndex);

    bool                            addServer(string portName, int port);
    bool                            delServer(string portName);

    map< string, TextServer* >&     getServers();
    TextServer*                     getServer(string portName);
    TextServer*                     getServer(int portIndex);

    map< string, goThreadedVideo* >&       getOffScreenVideos() {return _offScreenVideos;};
    map< string, goThreadedVideo* >&       getAllVideos() {return _allVideos;};

private:

	inline string			pad(string & objectName);
	int						_padLength;

	inline bool			    is_int(const boost::any & operand);
	inline bool			    is_float(const boost::any & operand);
	inline bool			    is_string(const boost::any & operand);
	inline bool			    is_char_ptr(const boost::any & operand);
	inline bool			    is_bool(const boost::any & operand);

	map<string, boost::any>	_anyProps;

	Scene*					_currentScene;
	map<string, Scene*>	    _scenes;

	map<string, string>		_allStates;

    // video types
    map< string, goVideoGrabber* >     _cameras;

//    ViewPort*               _copyOfViewport;

    // patterns
    map< string, vector<ofRectangle*> > _patterns;

    ofDirectory                         _videoDirectory;
    ofDirectory                         _textDirectory;

    map< string, TextObject* >          _presets;

    map< string, TextServer* >          _servers;

    map< string, string >               _texts;

    map< string, goThreadedVideo* >     _offScreenVideos;
    map< string, goThreadedVideo* >     _allVideos;


};

typedef Singleton<AppModel> AppModelSingleton;					// Global declaration

static AppModel * _appModel	= AppModelSingleton::Instance();	// this is my idea of a compromise:
																// no defines, keeps introspection
																// but allows data sharing between classes
#endif
