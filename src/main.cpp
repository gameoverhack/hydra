#include "ofMain.h"
#include "AppController.h"
#include "ofxFensterManager.h"
#include "Logger.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
	ofSetupOpenGL(ofxFensterManager::get(), 900, 1024, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	LOGGER->setLogLevel(JU_LOG_VERBOSE);
	ofRunFensterApp(new AppController());
}

