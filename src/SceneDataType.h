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

#include "ofxTCPServer.h"

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

class TextCue {

public:

    TextCue(){
        LOG_VERBOSE("Creating TextCue (no init)");
    };

    TextCue(string msg, int timestamp) : _msg(msg), _timestamp(timestamp){
        LOG_VERBOSE("Creating TextCue: " + _msg + ofToString(_timestamp));
    };

    ~TextCue(){
        LOG_VERBOSE("Destroying TextCue");
    }

    void setMessage(string msg) {_msg = msg;};
    void setTimestamp(int timestamp) {_timestamp = timestamp;};

    void setCue(string msg, int timestamp) {
        _msg = msg;
        _timestamp = timestamp;
    };

    string& getMessage() {return _msg;};
    int& getTimestamp() {return _timestamp;};

private:

    string _msg;
    int _timestamp;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & _msg;
		ar & _timestamp;
	};

};

class TextObject {
public:

    TextObject(){
        LOG_VERBOSE("Creating TextObject (no init)");
        _midiNote = _midiChannel = _lastRecordTime = _lastPlayTime = _currentCueIndex = -1;
        _fileName = _filePath = _text = _name = "";
        _lastCue = _currentCue = NULL;
        _bIsRecording = _bIsPlaying = false;
        _targetScreenWidth = 1024.0f;
        _targetScreenHeight = 768.0f;
        _fbo = NULL;
        renderFBO = true;
        int fontSize = 30;
        float lineHeight = (float)fontSize + 4.0f;
        float letterSpacing = 1.035f;
        ofTrueTypeFont::setGlobalDpi(72);
        verdana.loadFont("fonts/verdana.ttf", fontSize, true, true);
        verdana.setLineHeight(lineHeight);
        verdana.setLetterSpacing(letterSpacing);
    };

//    TextObject(int port, string text, vector<TextCue*> textCues)
//               :_port(port), _text(text), _textCues(textCues){
//        LOG_VERBOSE("Creating TextObject (with init)");
//    };

    ~TextObject(){
        LOG_VERBOSE("Destroying TextObject");
        _midiNote = _midiChannel = _lastRecordTime = _lastPlayTime = _currentCueIndex = NULL;
        _text.clear();
        _fileName.clear();
        _filePath.clear();
        _name.clear();
        _textCues.clear();
        _bIsRecording = _bIsPlaying = false;
        delete _lastCue;
        delete _currentCue;
        delete _fbo;
        _lastCue = _currentCue = NULL;
    };

    void startRecording() {
        if (!_bIsRecording && !_bIsPlaying) {
            LOG_VERBOSE("Start recording");
            _textCues.clear();
            _bIsRecording = true;
            _lastRecordTime = ofGetElapsedTimeMillis();
            addCue("START^" + _text);
        } else {
            if (_bIsRecording) LOG_ERROR("We are already recording!");
            if (_bIsPlaying) LOG_ERROR("We are playing!");
        }
    };

    void addCue(string msg) {

        _currentCue = new TextCue(msg, ofGetElapsedTimeMillis() - _lastRecordTime);

        if (_bIsRecording) {
            LOG_VERBOSE("Add cue");
            _lastRecordTime = ofGetElapsedTimeMillis();
            _textCues.push_back(_currentCue);
        } else {
            LOG_ERROR("Not recording!");
        }

    };

    void stopRecording() {
        if (_bIsRecording) {
            LOG_VERBOSE("Stop recording");
            addCue("STOP^");
            _bIsRecording = false;
        } else {
            LOG_ERROR("Not recording!");
        }
    };

    void startPlaying() {
        if (!_bIsRecording && !_bIsPlaying) {
            if (_textCues.size() < 1) {
                LOG_VERBOSE("Nothing to play! Try recording first!");
                return;
            }
            LOG_VERBOSE("Start playing: " + ofToString(_textCues.size()));

            rewind();
            _lastPlayTime = ofGetElapsedTimeMillis();
            _currentTime = 0;
            _bIsPlaying = true;

        } else {
            if (_bIsRecording) LOG_ERROR("We are recording!");
            if (_bIsPlaying) LOG_ERROR("We are already playing!");
        }
    };

    int getCueIndex(int proximateRangeMillis = 0) {
        int cueIndex = -1;
        if (_currentCueIndex < 0 || _currentCueIndex > _textCues.size()) {
            stopPlaying();
            return cueIndex;
        }
        if (proximateRangeMillis == 0) {
            //LOG_VERBOSE("Get exact cue index...");
            int nextTime = _textCues[_currentCueIndex]->getTimestamp();
            if (ofGetElapsedTimeMillis() - _lastPlayTime >= nextTime) {
                _currentTime += (ofGetElapsedTimeMillis() - _lastPlayTime);
                _lastPlayTime = ofGetElapsedTimeMillis();
                cueIndex = _currentCueIndex;
                _currentCueIndex++;
            }
        } else {
            LOG_VERBOSE("Get proximate cue index...");

        }
        return cueIndex;
    };

    TextCue* getCue() {

        if (_bIsPlaying) {
            int cueIndex = getCueIndex();
            if (cueIndex != -1) _currentCue = _textCues[cueIndex];
        }

        _lastCue = _currentCue;
        _currentCue = NULL;
        return _lastCue;

    }

    void stopPlaying() {
        if (_bIsPlaying) {
            LOG_VERBOSE("Stop playing");
            _bIsPlaying = false;
        } else {
            LOG_ERROR("Not playing!");
        }
    };

    void rewind() {
        LOG_VERBOSE("Rewind");
        _currentCueIndex = 0;
    };

    int getDuration() {
        //LOG_VERBOSE("Get duration");
        int duration = 0;
        for (int i = 0; i < _textCues.size(); i++) {
            duration += _textCues[i]->getTimestamp();
        }
        return duration;
    };

    float getPosition() {
        //LOG_VERBOSE("Get position");
        float pct = 1.0f; // when recording we just return 1.0 ie., the end
        if (_bIsPlaying) {
            pct = 0.0f;
            pct = (_currentTime + ofGetElapsedTimeMillis() - _lastPlayTime)/(float)getDuration();
        }
        return pct;
    };

    void setPosition(float pct) {
        LOG_VERBOSE("Set duration");
    };

    void clear() {_textCues.clear();};

    void setText(string text) {
        _text = parseText(text);
        LOG_VERBOSE("Set text: \n" + _text);
    };

    void setName(string& name) {_name = name;};

    void setFileName(string fileName) {_fileName = fileName;};
    void setFilePath(string filePath) {_filePath = filePath;};

    string getFileName() {return _fileName;};
    string getFilePath() {return _filePath;};

    string getName() {return _name;};
    string getText() {return _text;};

    bool getIsRecording() {return _bIsRecording;};
    bool getIsPlaying() {return _bIsPlaying;};

    void setMidiNote(int note) {_midiNote = note;};
    void setMidiChannel(int channel) {_midiChannel = channel;};

    int getMidiNote() {return _midiNote;};
    int getMidiChannel() {return _midiChannel;};

    void setTargetScreenWidth(float targetScreenWidth) {_targetScreenWidth = targetScreenWidth;};
    void setTargetScreenHeight(float targetScreenHeight) {_targetScreenHeight = targetScreenHeight;};
    float getTargetScreenWidth() {return _targetScreenWidth;};
    float getTargetScreenHeight() {return _targetScreenHeight;};

    void setFBO(ofFbo* fbo) {_fbo = fbo;};
    ofFbo* getFBO() {return _fbo;};

    ofRectangle getBounds() {return verdana.getStringBoundingBox(_text, 0, 30);};

    float scrollY;
    bool renderFBO;

private:

    string _name;
    string _text;

    string _fileName;
    string _filePath;

    vector<TextCue*> _textCues;

    bool _bIsRecording;
    bool _bIsPlaying;

    int _lastRecordTime;
    int _lastPlayTime;

    int _currentCueIndex;
    int _currentTime;

    int _midiNote;
    int _midiChannel;

    float _targetScreenWidth;
    float _targetScreenHeight;

    TextCue* _lastCue;
    TextCue* _currentCue;

    ofFbo* _fbo;


	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & _name;
		ar & _text;
		ar & _fileName;
		ar & _filePath;
        ar & _textCues;
        ar & _lastRecordTime;
        ar & _currentCueIndex;
        ar & _midiNote;
        ar & _midiChannel;
        ar & _targetScreenWidth;
	};

    ofTrueTypeFont verdana;

    //--------------------------------------------------------------
    string parseText(string text) {

        // make 'word' chunks of text (space delimited)
        vector<string>words = ofSplitString(text, " ");

        // clear text...
        text = "";

        ofRectangle bounds;

        // ...and re-fill text string checking bounds at each word
        for (int i = 0; i < words.size(); i++) {

            // get bounds of next word in vector of text
            string test = text + words[i] + " ";
            bounds = verdana.getStringBoundingBox(test, 0, 0);

            // test if next word would take us over width of application
            if (bounds.width > _targetScreenWidth) {
                // if it does insert a carriage return & a space
                text += "\n" + words[i] + " ";
            } else {
                // else just insert a space
                text += words[i] + " ";
            }

        }

        return text;

    };
};

class TextServer {

public:
    TextServer() {
        LOG_VERBOSE("Creating TextServer");
        _name = "";
        _port = NULL;
        _preset = NULL;
        _server = NULL;
	};

	~TextServer() {
		LOG_VERBOSE("Destroying TextServer");
        _name = "";
        _port = NULL;
        _preset = NULL;
        _server = NULL;
	};

    void setPort(int port) {_port = port;};
    void setName(string& name) {_name = name;};

    void setPreset(TextObject* preset) {_preset = preset;};
    void setServer(ofxTCPServer* server) {_server = server;};

    int getPort() {return _port;};
    string getName() {return _name;};

    TextObject* getPreset() {return _preset;};
    ofxTCPServer* getServer() {return _server;};

private:

    int _port;
    string _name;

    TextObject* _preset;
    ofxTCPServer* _server;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & _port;
		ar & _name;
	};

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

//    vector<TextObject*> _prompters;
//    vector<TextObject*>& getPrompters() {return _prompters;};
//    _prompters.clear();
