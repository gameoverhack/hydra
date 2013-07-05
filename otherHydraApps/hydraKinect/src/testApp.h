#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxThreadedVideo.h"
#include "ofxOsc.h"
#include "ofxCv.h"
#include "ofxBlur.h"
#include "ViewPort.h"
#include "ofxXmlSettings.h"
#import <ifaddrs.h>
#import <arpa/inet.h>

enum KinectAppState{
    KINECT_APP_READY = 0,
    KINECT_APP_WHITE,
    KINECT_APP_IMAGE,
    KINECT_APP_PLAY,
    KINECT_APP_LIST
};

class testApp : public ofBaseApp{

public:
    
	void setup();
	void update();
	void draw();
    void exit();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

    void playMovie(string fileName);
    void setFade(float _fade, int _time);
    
    void loadSettings();
    void saveSettings();
    void resetSettings();
    
    string getIPAddress();
    string getKinectAppStateAsString(KinectAppState appState);
    
    ofxOpenNI openNIDevice;
    ofxOpenNIDepthThreshold depthThreshold;
    ofFbo fbo, renderFBO;
    ofxThreadedVideo vid;
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;
    ofxBlur blurShader;
    ofShader shader;
    ofxCv::ContourFinder contourFinder;
    ViewPort * viewPort;
    ofxXmlSettings settings;
    ofDirectory dir;
    ofImage image;
    
    KinectAppState appState;
    int appIP;
    string currentFile;
    int totalFrames, currentFrame;
    
    bool bUseContour;
    bool bShowWhite;
    bool bShowImage;
    float controlW, controlH;
    float renderW, renderH;
    float outputW, outputH;
    int minDepth, maxDepth;
    float smoothing;
    int threshold;
    float blur;
    int time;
    int lastTime;
    float fade;
    float currentFade;
    float lastFade;
    bool bUseRealKinect;
    
    int lastHeartBeat;
    
    static inline void smoothPoints(vector <cv::Point> &contourIn, vector <cv::Point> &contourOut, float smoothPct) {
        
        int length = MIN(contourIn.size(), contourOut.size());
        
        float invPct = 1.0 - smoothPct;
        
        //first copy the data
        for(int i = 0; i < length; i++){
            contourOut[i] = contourIn[i];
        }
        
        //then smooth the contour
        //we start at 1 as we take a small pct of the prev value
        for(int i = 1; i < length; i++){
            contourOut[i] = contourOut[i] * smoothPct   +   contourOut[i-1] * invPct;
        }
        
    };
    
    
    
};
