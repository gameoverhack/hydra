#include "Config.h"
#include "ofMain.h"
#include "AppController.h"
#include "Logger.h"
#include "ofAppGlutWindow.h"

#ifndef FENSTER
    #include "ofAppGlutWindow.h"
#else
    #include "ofxFensterManager.h"
#endif



//========================================================================
int main( ){

LOGGER->setLogLevel(JU_LOG_VERBOSE);

#ifndef FENSTER
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1920 * 2, 1080, OF_WINDOW);
	ofRunApp( new AppController());
#else
	ofSetupOpenGL(ofxFensterManager::get(), 900, 1024, OF_WINDOW);
	ofRunFensterApp(new AppController());
#endif

}
