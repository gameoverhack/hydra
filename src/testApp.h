#pragma once

//#define USE_VECTOR 1

#include "ofMain.h"
#include "ofxFensterManager.h"
#include "goThreadedVideo.h"

class RenderWindow: public ofxFensterListener {
public:

    RenderWindow() { ofLogVerbose() << "renderWindow created";};
	~RenderWindow() { ofLogVerbose() << "renderWindow destroyed";};

	void draw() {renderFBO.draw(0,0);};

	ofFbo& getRenderFBO() {return renderFBO;};

private:

	ofFbo renderFBO;

};

class testApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseMoved(int x, int y, ofxFenster* win);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void mouseMovedEvent(ofMouseEventArgs &args);

    void success(string & path);
    void error(int & errCode);

    ofxFenster* renderer;
    RenderWindow* renderWindow;

#ifdef USE_VECTOR
    vector<goThreadedVideo*> videos;
#else
    goThreadedVideo ** videos;
#endif
    ofDirectory dir;
    int numberOfMovies;
    int numberOfMoviesLoaded;
    int currentlyLoadingVideo;
};
