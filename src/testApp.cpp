#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetVerticalSync(false);
	//ofSetFrameRate(500);

	ofBackground(0,0,0);

    ofSetLogLevel(OF_LOG_VERBOSE);

	// get the paths to a bunch of movies
	dir.allowExt("mov");
	numberOfMovies = dir.listDir("D:/CHORUS_HYDRA/06.11.2011.Convert/H264");
	numberOfMovies = 24;
	ofLogVerbose() << "Attempt to load" << numberOfMovies;

	// create as many instances of goVideoPlayer as there are movies
#ifdef USE_VECTOR
	videos.resize(numberOfMovies);
#else
	videos = new goThreadedVideo*[numberOfMovies];
#endif

	for (int i = 0; i < numberOfMovies; i++) {
		videos[i] = new goThreadedVideo();
	}

	// use some variables to track where we're upto loading the movies
	numberOfMoviesLoaded = 0;
	currentlyLoadingVideo = -1;

    renderWindow = new RenderWindow();

    ofFbo& renderFBO = renderWindow->getRenderFBO();
    renderFBO.allocate(1920,1080);

    renderer = ofxFensterManager::get()->createFenster(1920,0,1920,1080);
    renderer->addListener(renderWindow);
    renderer->setBackgroundColor(0,0,0);
    renderer->setWindowTitle("renderer");
    renderer->setBorder(false);

}


//--------------------------------------------------------------
void testApp::update()
{
    //ofBackground(0,0,0);

	// check if there are more movies to load
	if (currentlyLoadingVideo < numberOfMoviesLoaded && numberOfMovies > numberOfMoviesLoaded) {
		// increment the current video counter and start loading the movie
		currentlyLoadingVideo++;
		ofLogVerbose() << "Start loading:" << dir.getPath(currentlyLoadingVideo);
		videos[currentlyLoadingVideo]->loadMovie(dir.getPath(currentlyLoadingVideo));
		// attach event listeners so we know when the movie is loaded
		ofAddListener(videos[currentlyLoadingVideo]->success, this, &testApp::success);
		ofAddListener(videos[currentlyLoadingVideo]->error, this, &testApp::error);
	}

	// check if we're currently loading a movie
	if (currentlyLoadingVideo == numberOfMoviesLoaded && numberOfMovies > numberOfMoviesLoaded) {
		// use psuedo draw and update to force the loading of movies without actually having to be showing them onscreen
		// this is an ugly hack I use to load multiple movies without having to actually be drawing them to screen
		videos[currentlyLoadingVideo]->psuedoUpdate();
		videos[currentlyLoadingVideo]->psuedoDraw();
	}

	// 'really' update movies that are already loaded
	for (int i = 0; i < numberOfMoviesLoaded; i++) {
		videos[i]->update();
	}

    ofFbo& renderFBO = renderWindow->getRenderFBO();

    renderFBO.begin();
    {
         ofSetColor(255,255,255);

        // 'really' draw movies that are already loaded
        int xOffset = 0;
        int yOffset = 0;
        for (int i = 0; i < numberOfMoviesLoaded; i++) {
            // using a quick and dirty tiling method
            if (i > 0) xOffset += 320;
            if (xOffset > renderFBO.getWidth() - 320) {
                xOffset = 0;
                yOffset += 240;
            }
            //
            videos[i]->draw(xOffset, yOffset, 320, 240);
        }
    }
    renderFBO.end();
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofSetColor(0, 255, 0);
	string msg = "FPS: " + ofToString(ofGetFrameRate());
	ofDrawBitmapString(msg, 20,20);
}

void testApp::success(string & path) {
	ofLogVerbose() << "Finsished loading:" << path;
	//remove the event listeners
	ofRemoveListener(videos[currentlyLoadingVideo]->success, this, &testApp::success);
	ofRemoveListener(videos[currentlyLoadingVideo]->error, this, &testApp::error);
	// increment the counter for total number of movies loaded...this will cause the if statement in update to start loading again
	if (currentlyLoadingVideo == 15) {
	    videos[currentlyLoadingVideo]->setVolume(255);
	} else {
        videos[currentlyLoadingVideo]->setVolume(90);
	}
	numberOfMoviesLoaded++;
}

void testApp::error(int & errCode) {
	ofLogVerbose() << "Error loading:" << errCode;
	//remove the event listeners
	ofRemoveListener(videos[currentlyLoadingVideo]->success, this, &testApp::success);
	ofRemoveListener(videos[currentlyLoadingVideo]->error, this, &testApp::error);
	// increment the counter for total number of movies loaded...this will cause the if statement in update to start loading again
	numberOfMoviesLoaded++;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
	//cout << (0x0400) << endl;
	//cout << (101 | OF_KEY_MODIFIER) << " " << key << endl;
	if(key=='f')
		renderer->toggleFullscreen();
	if(key == 'c')
		cout << "CLIPBOARD DATA: " << ofxFensterManager::get()->getClipboard() << endl;
}

//this only works if testApp is extending ofxFensterListener and not ofBaseApp
void testApp::mouseMoved(int x, int y, ofxFenster* win)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
	cout << "GOT SOME FILES: "<<endl;
	std::vector<string>::iterator it = dragInfo.files.begin();
	while(it != dragInfo.files.end()){
		cout << *it << endl;
		++it;
	}
}

void testApp::mouseMovedEvent(ofMouseEventArgs &args)
{
	//cout << "MOUSE WAS MOVED" << endl;
}
