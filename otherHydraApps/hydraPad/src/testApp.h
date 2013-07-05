#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofxiPhoneExternalDisplay.h"

#include "ofxIOSimpleFTP.h"
#include "ofxOsc.h"

#import <ifaddrs.h>
#import <arpa/inet.h>

enum IOSAppState{
    IOS_APP_INIT = 0,
    IOS_APP_SYNC,
    IOS_APP_PLAY,
    IOS_APP_LIST,
    IOS_APP_WAIT
};

class testApp : public ofxiPhoneApp{
	
public:
    
    void setup();
    void update();
    void draw();
    void exit();
	
    void touchDown(ofTouchEventArgs & touch);
    void touchMoved(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);
    void touchDoubleTap(ofTouchEventArgs & touch);
    void touchCancelled(ofTouchEventArgs & touch);

    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(int newOrientation);

    string getIOSAppStateAsString(IOSAppState appState);
    
    void playMovie(string path);
    void ftpEvent(ofxIOSimpleFTPEvent & e);
    void listApplicationDirectory();
    
    string getIPAddress();
    
    int appIP;
    
    IOSAppState appState;
    
    ofxIOSimpleFTP ftp;
    
    string downloadDirectory;
    ofVideoPlayer vid;
    
    ofDirectory dir;
    
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;
    
    string currentFile;
    int totalFrames, currentFrame;
    string displayModesAsString;
    bool bShowInfo;
    bool bMirror;
    
    int lastHeartBeat;
    
};


