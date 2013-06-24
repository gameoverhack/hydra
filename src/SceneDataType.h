/*
 *  SceneDataType.h
 *  Bunyip
 *
 *  Created by gameover on 26/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#ifndef _H_SCENEDATATYPE
#define _H_SCENEDATATYPE

#include "FunctionModel.h"
#include "SerializationUtils.h"
#include "ofxOsc.h"

#include <map>
#include <vector>
#include <assert.h>
#include <string>

enum {
    GO_VIDEO_NONE,
    GO_VIDEO_CAMERA,
    GO_VIDEO_PLAYER
};

enum {
    GO_SCALE_INTO,
    GO_SCALE_CROP
};

//string videoType[] = {
//    "GO_VIDEO_NONE",
//    "GO_VIDEO_CAMERA",
//    "GO_VIDEO_PLAYER"
//};
//
//string cropType[] = {
//    "GO_SCALE_INTO",
//    "GO_SCALE_CROP",
//};

class VideoObject {

public:

    VideoObject(){
        LOG_VERBOSE("Creating VideoObject (no init)");
        _player = NULL;
        _camera = NULL;
        _overlay = NULL;
        _videoPath = "";
        _inputType = GO_VIDEO_NONE;
        _cropType = GO_SCALE_INTO;
        _bAssigned = _bOffscreen = _bDegraded = false;
        _fVolume = 1.0f;
        _fPan = 0.0f;
        _updated = false;
    };
//    VideoObject(int inputType, int cropType, ofRectangle* position, goBaseVideo* video)
//                :_inputType(inputType), _cropType(cropType), _position(position), _video(video){
//        LOG_VERBOSE("Creating VideoObject (with init)");
//    };

    ~VideoObject(){
        LOG_VERBOSE("Destroying VideoObject");
        //_position // how to delete?
        if (_player != NULL && _inputType == GO_VIDEO_PLAYER) {
            //_player->close();
            //delete _player;
            _player = NULL;
        }
        if (_overlay != NULL && _inputType == GO_VIDEO_PLAYER) {
            //_overlay->close();
            //delete _overlay;
            _overlay = NULL;
        }
        _inputType = _cropType = NULL;
    };

    bool isFrameNew(){
        if (_player != NULL && _inputType == GO_VIDEO_PLAYER) {
            return _player->isFrameNew();
        }
        if (_overlay != NULL && _inputType == GO_VIDEO_PLAYER) {
            return _overlay->isFrameNew();
        }
        if (_camera != NULL && _inputType == GO_VIDEO_CAMERA) {
            return _camera->isFrameNew();
        }
        return false;
    };

    string getInfo(){

        ostringstream os;
        os << "Name: " << _videoName;// << endl;
        //os << "Type: " << videoType[_inputType] << endl;
        //os << "Crop: " << cropType[_cropType] << endl;
        os << " Assigned: " << string(_bAssigned ? "TRUE" : "FALSE");// << endl;
        os << " Offscreen: " << string(_bOffscreen ? "TRUE" : "FALSE");// << endl;
        os << " Degraded: " << string(_bDegraded ? "TRUE" : "FALSE");// << endl;
        os << " Volume: " << _fVolume;// << endl;
        os << " Pan: " << _fPan;// << endl;

        return os.str();

    };

    int _inputType;
    int _cropType;

    bool _bOffscreen;
    bool _bDegraded;
    float _fVolume;
    float _fPan;

    bool _updated;
    bool _bAssigned;

    string _videoPath;
    string _videoName;

    ofxThreadedVideo* _player;
    ofxThreadedVideo* _overlay;
    ofVideoGrabber*   _camera;

private:

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & _inputType;
		ar & _cropType;
        ar & _bAssigned;
        ar & _bOffscreen;
        ar & _bDegraded;
        ar & _fVolume;
        ar & _fPan;
        ar & _videoPath;
        ar & _videoName;
	};

};

enum IOSAppState{
    APP_INIT = 0,
    APP_SYNC,
    APP_PLAY,
    APP_LIST,
    APP_WAIT
};

class IOSVideoPlayer{

public:

    IOSVideoPlayer(){
        oscSender = new ofxOscSender;
    };
    ~IOSVideoPlayer(){
        if(oscSender != NULL){
            delete oscSender;
            oscSender = NULL;
            files.clear();
        }
    }

    ofxOscSender* oscSender;
    vector<string> files;
    bool bNeedsDisplayUpdate;
    string currentFile;
    int currentFrame;
    int totalFrames;
    IOSAppState iosAppState;
};

class Scene {

public:

	Scene() {
        LOG_VERBOSE("Creating scene");
        _name = "";
        _patternName = "";
        _layoutPosition = 0;
        _midiNote = _midiChannel = -1;
	};

	~Scene() {
		LOG_VERBOSE("Destroying scene");
		_name.clear();
		_videos.clear();
	};

    void setName(string& name) {_name = name;};
    void setPatternName(string patternName) {_patternName = patternName;};
    void setLayoutPosition(int layoutPosition) {_layoutPosition = layoutPosition;};

    string getName() {return _name;};
    string getPatternName() {return _patternName;};
    int getLayoutPosition() {return _layoutPosition;};

    void setMidiNote(int note) {_midiNote = note;};
    void setMidiChannel(int channel) {_midiChannel = channel;};

    int getMidiNote() {return _midiNote;};
    int getMidiChannel() {return _midiChannel;};

    vector<VideoObject*>& getVideos() {return _videos;};

private:

    string _name;
    string _patternName;
    int _layoutPosition;

    int _midiNote;
    int _midiChannel;

    vector<VideoObject*> _videos;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & _name;
		ar & _patternName;
        ar & _layoutPosition;
        ar & _midiNote;
        ar & _midiChannel;
        ar & _videos;
	};

};

#endif;
