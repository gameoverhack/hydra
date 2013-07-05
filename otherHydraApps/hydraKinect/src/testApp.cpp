#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
    
    bUseContour = false;
    bShowWhite = false;
    bShowImage = false;
    minDepth = 1500;
    maxDepth = 2800;
    renderW = 640.0f;
    renderH = 480.0f;
    outputW = 1024.0f;
    outputH = 768.0f;
    controlW = 1680;
    controlH = 1050;
    smoothing = 0.1;
    threshold = 127;
    bUseRealKinect = true;
    lastFade = currentFade = fade = 1.0;
    time = 0;
    lastTime = ofGetElapsedTimeMillis();
    blur = 1.0f;
    lastHeartBeat = ofGetElapsedTimeMillis();
    
    loadSettings();
    
    if(bUseRealKinect){
        openNIDevice.setup();
    }else{
        openNIDevice.setupFromONI("test2012-02-21-21-21-21-026.oni");
    }
    
    openNIDevice.setLogLevel(OF_LOG_VERBOSE);
    openNIDevice.addDepthGenerator();
    openNIDevice.addImageGenerator();
    openNIDevice.setRegister(true);
    //openNIDevice.setMirror(true);
    openNIDevice.start();
    
    dir.allowExt("mov");
    dir.listDir("movies");
    
    // setup osc
    oscReceiver.setup(6668);
    oscSender.setup("192.168.1.99", 6669);
    
    // get our own ip address and use last part as heartbeat
    vector<string> ipParts = ofSplitString(getIPAddress(), ".");
    appIP = ofToInt(ipParts[ipParts.size() - 1]);
    
    depthThreshold = ofxOpenNIDepthThreshold(minDepth, maxDepth);
    openNIDevice.addDepthThreshold(depthThreshold);

    fbo.allocate(renderW, renderH);
    renderFBO.allocate(renderW, renderH);
    blurShader.setup(renderW, renderH);
    blurShader.setScale(1.0f);
    
    //vid.loadMovie(dir.getPath(0));
    
    contourFinder.setMinAreaRadius(10);
    contourFinder.setMaxAreaRadius(1000);
    contourFinder.setThreshold(threshold);
    
    shader.load("shaders/blurandmerge");
    
    viewPort = new ViewPort(renderW, renderH);
    viewPort->loadViewport();
    
    ofSetFullscreen(true);
    
    appState = KINECT_APP_READY;
    
    ofBackground(0, 0, 0);
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    currentFile = "";
    currentFrame = totalFrames = 0;
    
    vid.update();
    openNIDevice.update();
    
    if(appState == KINECT_APP_PLAY && vid.isPlaying()){
        currentFile = vid.getMovieName();
        currentFrame = vid.getCurrentFrame();
        totalFrames = vid.getTotalNumFrames();
    }
    
    if(appState == KINECT_APP_PLAY && vid.getIsMovieDone()){
        appState = KINECT_APP_READY;
        vid.stop();
    }
    
    if(ofGetElapsedTimeMillis() - lastHeartBeat > 1000){
		ofxOscMessage m;
		m.setAddress("/app/heartbeat");
		m.addIntArg(appIP);
        m.addIntArg(appState);
        m.addStringArg(currentFile);
        m.addIntArg(currentFrame);
        m.addIntArg(totalFrames);
        m.addIntArg(minDepth);
        m.addIntArg(maxDepth);
        m.addIntArg(blur);
        m.addIntArg(threshold);
        m.addFloatArg(smoothing);
        m.addIntArg(bUseContour);
		oscSender.sendMessage(m);
        lastHeartBeat = ofGetElapsedTimeMillis();
	}
    
    while(oscReceiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
        
        if(m.getAddress() == "/app/list"){
            cout << "List" << endl;
            ofxOscMessage s;
            s.setAddress("/app/list");
            s.addIntArg(appIP);
            for (int i = 0; i < dir.size(); i++) {
                s.addStringArg(dir.getName(i));
            }
            oscSender.sendMessage(s);
        }
        
        if(m.getAddress() == "/app/play"){
            cout << "Play: " << m.getArgAsString(0) << endl;
            playMovie(m.getArgAsString(0));
        }
        
        if(m.getAddress() == "/app/stop"){
            cout << "Stop" << endl;
            appState = KINECT_APP_READY;
            vid.stop();
        }
        
        if(m.getAddress() == "/app/minDepth"){
            minDepth = ofToInt(m.getArgAsString(0));
            minDepth = CLAMP(minDepth, 0, maxDepth);
            cout << "minDepth: " << minDepth << endl;
            if(openNIDevice.getNumDepthThresholds() == 1){
                ofxOpenNIDepthThreshold & dt = openNIDevice.getDepthThreshold(0);
                dt.setNearThreshold(minDepth);
            }
            saveSettings();
        }
        
        if(m.getAddress() == "/app/maxDepth"){
            maxDepth = ofToInt(m.getArgAsString(0));
            maxDepth = CLAMP(maxDepth, minDepth, 10000);
            cout << "maxDepth: " << maxDepth << endl;
            if(openNIDevice.getNumDepthThresholds() == 1){
                ofxOpenNIDepthThreshold & dt = openNIDevice.getDepthThreshold(0);
                dt.setFarThreshold(maxDepth);
            }
            saveSettings();
        }
        
        if(m.getAddress() == "/app/blur"){
            blur = ofToInt(m.getArgAsString(0));
            cout << "blur: " << blur << endl;
            blur = CLAMP(blur, 0.0f, 2.0f);
            blurShader.setScale(blur);
            saveSettings();
        }
        
        if(m.getAddress() == "/app/threshold"){
            threshold = ofToInt(m.getArgAsString(0));
            cout << "threshold: " << threshold << endl;
            threshold = CLAMP(threshold, 0, 255);
            contourFinder.setThreshold(threshold);
            saveSettings();
        }
        
        if(m.getAddress() == "/app/smoothing"){
            smoothing = ofToInt(m.getArgAsString(0));
            cout << "smoothing: " << smoothing << endl;
            smoothing = CLAMP(smoothing, 0.0f, 1.0f);
            saveSettings();
        }
        
        if(m.getAddress() == "/app/contour"){
            bUseContour = (bool)ofToInt(m.getArgAsString(0));
            cout << "contour: " << bUseContour << endl;
            saveSettings();
        }
        
        if(m.getAddress() == "/app/white"){
            bShowWhite = (bool)ofToInt(m.getArgAsString(0));
            cout << "show white: " << bShowWhite << endl;
            if(bShowWhite) bShowImage = false;
        }
        
        if(m.getAddress() == "/app/image"){
            bShowImage = (bool)ofToInt(m.getArgAsString(0));
            cout << "show image: " << bShowImage << endl;
            if(bShowImage) bShowWhite = false;
        }
        
        if(m.getAddress() == "/app/fade"){
            vector<string> fadeParts = ofSplitString(m.getArgAsString(0), ":");
            if(fadeParts.size() == 2){
                setFade(ofToFloat(fadeParts[0]), ofToInt(fadeParts[1]));
                lastFade = currentFade;
                lastTime = ofGetElapsedTimeMillis();
                cout << "set fade: " << fade << endl;
            }else{
                cout << "invalid fade: " << m.getArgAsString(0) << endl;
            }
            
        }
        
        if(m.getAddress() == "/app/snapshot"){
            image.setFromPixels(openNIDevice.getImagePixels().getPixels(), openNIDevice.getImagePixels().getWidth(), openNIDevice.getImagePixels().getHeight(), OF_IMAGE_COLOR);
        }
    }
    
}

//--------------------------------------------------------------
void testApp::setFade(float _fade, int _time){
    fade = CLAMP(_fade, 0.0f, 1.0f);
    time = CLAMP(_time, 0, 1000000);
    lastFade = currentFade;
    lastTime = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
void testApp::playMovie(string fileName){
    string filePath = "";
    for(int i = 0; i < dir.size(); i++){
        cout << dir.getName(i) << endl;
        if(dir.getName(i) == fileName){
            filePath = dir.getPath(i);
            break;
        }
    }
    if(filePath != ""){
        appState = KINECT_APP_PLAY;
        vid.loadMovie(filePath);
        vid.play();
        vid.setLoopState(OF_LOOP_NONE);
    }else{
        appState = KINECT_APP_READY;
    }
}



//--------------------------------------------------------------
void testApp::draw(){
    
    ofSetColor(255, 255, 255, 255);
    
    if(openNIDevice.getNumDepthThresholds() == 1){
        ofxOpenNIDepthThreshold & dt = openNIDevice.getDepthThreshold(0);
        ofPushMatrix();
        ofScale(0.5, 0.5);
        openNIDevice.drawDebug();
        dt.drawROI();
        ofPopMatrix();
        
        blurShader.begin();
        {
            ofClear(0.0f, 0.0f, 0.0f, 1.0f);
            if(bUseContour){
                ofFill();
                ofPushMatrix();
                ofPixels & dp = dt.getMaskPixels();
                contourFinder.findContours(dp);
                ofScale(renderW/dp.getWidth(), renderH/dp.getHeight());
                for(int c = 0; c < contourFinder.getContours().size(); c++){
                    vector<cv::Point> vec = contourFinder.getContour(c);
                    vector<cv::Point> vecC = contourFinder.getContour(c);
                    smoothPoints(vecC, vec, smoothing);
                    
                    ofSetColor(255,255,255,0);
                    
                    ofBeginShape();
                    for(int i = 0; i < vec.size(); i++){
                        ofVertex(vec[i].x, vec[i].y);
                    }
                    ofEndShape();
                    
                }
                ofNoFill();
                ofPopMatrix();
            }else{
                dt.getMaskTextureReference().draw(0, 0, fbo.getWidth(), fbo.getHeight());
            }
        }
        blurShader.end(false);

        
        fbo.begin();
        {
            ofClear(0.0f, 0.0f, 0.0f, 1.0f);
            
            ofPushMatrix();

            if(bShowImage){
                image.draw(0, 0, renderW, renderH);
            }else if(!bShowWhite){
                renderFBO.begin();
                ofClear(0.0f, 0.0f, 0.0f, 1.0f);
                vid.draw(0, 0, renderW, renderH);
                renderFBO.end();
                
                shader.begin();
                shader.setUniformTexture("imagesource", renderFBO.getTextureReference(), 1);
                shader.setUniformTexture("blurmask", blurShader.getTextureReference(), 2);
                renderFBO.draw(0,0);
                shader.end();
            }else{
                ofSetColor(255, 255, 255, 255);
                ofFill();
                ofRect(0, 0, outputW, outputH);
                ofNoFill();
            }
            

            ofPopMatrix();
            
        }
        fbo.end();
        
        
        
        if(currentFade != fade){
            int timeDiff = ofGetElapsedTimeMillis() - lastTime;
            if(timeDiff <= time){
                float pct = (float)timeDiff/(float)time;
                currentFade = lastFade + (fade - lastFade) * pct;
            }else{
                currentFade = fade;
            }
        }
        
        float color = 255.0f * currentFade;

        ofPushMatrix();
        
        vid.draw(0, 240, 320, 240);
        dt.getMaskTextureReference().draw(0, 480, 320, 240);
        ofSetColor(color);
        fbo.draw(320, 240, 320, 240);
        ofSetColor(255, 255, 255, 255);
        viewPort->drawViewport(320, 240, 320, 240);
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(controlW + viewPort->getX(), viewPort->getY());
        ofScale(outputW/renderW, outputH/renderH);
        viewPort->warp();
        ofSetColor(color);
        fbo.draw(0, 0);
        ofPopMatrix();
        
    }
    
    
    ofSetColor(0, 255, 0, 255);
    ostringstream os;
    os << "hydraKinect" << endl << endl;
    
    os << "FPS: " << ofGetFrameRate() << endl << endl;
    os << "MIN Depth: " << minDepth << " MAX Depth: " << maxDepth << endl; 
    os  << (string)(bUseContour ? "CONTOUR" : "NO CONTOUR") 
        << " threshold: " << threshold 
        << " smoothing: " << smoothing 
        << " blur: " << blur << endl << endl;
    
    os << "Key Commands" << endl;
    os << "============" << endl;
    os << "up/down - minDepth" << endl;
    os << "right/left - maxDepth" << endl;
    os << "'</>' - blur" << endl;
    os << "':/\"' - smoothing (when contour is used)" << endl;
    os << "'{/}' - threshold (should not need to adjust)" << endl;
    os << "'w' - show white" << endl;
    os << "'s' - take snapshot" << endl;
    os << "'i' - show snapshot" << endl;
    os << "'c' - use contours" << endl;
    os << "'f' - fullscreen" << endl;
    os << "'t' - reset keystone, position, size settings" << endl;
    os << "'r' - reset depth, threshold, smoothing, contour etc" << endl << endl;
    os << "Mouse Commands" << endl;
    os << "==============" << endl;
    os << "Adjust keystone/warp by dragging corners of red rect" << endl;
    os << "Holding down Apple key will lock x or y axis (axis scale)" << endl;
    os << "Holding down Shift key will adjust all corners (free scale)" << endl;
    os << "Move whole viewport with click and drag inside red rect" << endl << endl << endl;
    
    os  << appIP << " " 
        << getKinectAppStateAsString(appState) << " " 
        << currentFile << " " 
        << currentFrame << " " 
        << totalFrames << " " 
        << minDepth << " " 
        << maxDepth << " " 
        << blur << " " 
        << threshold << " " 
        << smoothing << " "
        << bUseContour << endl;
    
    ofDrawBitmapString(os.str(), 660, 20);

    
}

//--------------------------------------------------------------
void testApp::exit(){
    
    openNIDevice.stop();
    
}

//--------------------------------------------------------------
void testApp::loadSettings(){
    settings.loadFile("settings.xml");
    bUseContour = settings.getAttribute("config", "bUseContour", false);
    outputW = settings.getAttribute("config", "outputW", 1024.0f);
    outputH = settings.getAttribute("config", "outputH", 768.0f);
    renderW = settings.getAttribute("config", "renderW", 640.0f);
    renderH = settings.getAttribute("config", "renderH", 480.0f);
    controlW = settings.getAttribute("config", "controlW", 1680.0f);
    controlH = settings.getAttribute("config", "controlH", 1050.0f);
    minDepth = settings.getAttribute("config", "minDepth", 1500);
    maxDepth = settings.getAttribute("config", "maxDepth", 2500);
    threshold = settings.getAttribute("config", "threshold", 127);
    smoothing = settings.getAttribute("config", "smoothing", 0.1f);
    blur = settings.getAttribute("config", "blur", 1.0f);
    bUseRealKinect = settings.getAttribute("config", "real", true);
    blurShader.setScale(blur);
    contourFinder.setThreshold(threshold);
}

//--------------------------------------------------------------
void testApp::saveSettings(){
    settings.loadFile("settings.xml");
    settings.removeTag("config");
    settings.addTag("config");
    settings.addAttribute("config", "bUseContour", bUseContour, 0);
    settings.addAttribute("config", "outputW", outputW, 0);
    settings.addAttribute("config", "outputH", outputH, 0);
    settings.addAttribute("config", "renderW", renderW, 0);
    settings.addAttribute("config", "renderH", renderH, 0);
    settings.addAttribute("config", "controlW", controlW, 0);
    settings.addAttribute("config", "controlH", controlH, 0);
    settings.addAttribute("config", "minDepth", minDepth, 0);
    settings.addAttribute("config", "maxDepth", maxDepth, 0);
    settings.addAttribute("config", "threshold", threshold, 0);
    settings.addAttribute("config", "smoothing", smoothing, 0);
    settings.addAttribute("config", "blur", blurShader.getScale(), 0);
    settings.addAttribute("config", "real", bUseRealKinect, 0);
    settings.saveFile("settings.xml");
}

//--------------------------------------------------------------
void testApp::resetSettings(){
    bUseContour = false;
    minDepth = 1500;
    maxDepth = 2800;
    renderW = 640.0f;
    renderH = 480.0f;
    outputW = 1024.0f;
    outputH = 768.0f;
    controlW = 1680;
    controlH = 1050;
    threshold = 127;
    smoothing = 0.1;
    blur = 1.0f;
    bUseRealKinect = true;
    lastFade = currentFade = fade = 1.0;
    time = 0;
    lastTime = ofGetElapsedTimeMillis();
    blurShader.setScale(blur);
    contourFinder.setThreshold(threshold);
    saveSettings();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    bool bDirtyDepth = false;

    switch (key) {
        case '1':
            setFade(1.0f, 3000);
            break;
        case '2':
            setFade(0.0f, 3000);
            break;
        case '3':
            setFade(0.5f, 3000);
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'p':
            if(dir.size() > 0) playMovie(dir.getName((int)ofRandom(dir.size())));
            break;
        case 'o':
            vid.stop();
            break;
        case 'w':
            bShowWhite = !bShowWhite;
            if(bShowWhite) bShowImage = false;
            break;
        case 'i':
            bShowImage = !bShowImage;
            if(bShowImage) bShowWhite = false;
            break;
        case 's':
            image.setFromPixels(openNIDevice.getImagePixels().getPixels(), openNIDevice.getImagePixels().getWidth(), openNIDevice.getImagePixels().getHeight(), OF_IMAGE_COLOR);
            break;
        case 'c':
            bUseContour = !bUseContour;
            break;
        case 'r':
            resetSettings();
            break;
        case 't':
            viewPort->reset(outputW, outputH);
            break;
        case OF_KEY_UP:
            minDepth += 10;
            minDepth = CLAMP(minDepth, 0, maxDepth);
            bDirtyDepth = true;
            break;
        case OF_KEY_DOWN:
            minDepth -= 10;
            minDepth = CLAMP(minDepth, 0, maxDepth);
            bDirtyDepth = true;
            break;
        case OF_KEY_RIGHT:
            maxDepth += 10;
            maxDepth = CLAMP(maxDepth, 0, 10000);
            bDirtyDepth = true;
            break;
        case OF_KEY_LEFT:
            maxDepth -= 10;
            maxDepth = CLAMP(maxDepth, minDepth, 10000);
            bDirtyDepth = true;
            break;
        case ',':
        case '<':
            blur = blurShader.getScale();
            blur -= 0.1;
            blur = CLAMP(blur, 0.0f, 2.0f);
            blurShader.setScale(blur);
            break;
        case '.':
        case '>':
            blur = blurShader.getScale();
            blur += 0.1;
            blur = CLAMP(blur, 0.0f, 2.0f);
            blurShader.setScale(blur);
            break;
        case '[':
        case '{':
            threshold -= 1;
            threshold = CLAMP(threshold, 0, 255);
            contourFinder.setThreshold(threshold);
            break;
        case ']':
        case '}':
            threshold += 1;
            threshold = CLAMP(threshold, 0, 255);
            contourFinder.setThreshold(threshold);
            break;
        case ';':
        case ':':
            smoothing -= 0.05;
            smoothing = CLAMP(smoothing, 0.0f, 1.0f);
            break;
        case '\'':
        case '\"':
            smoothing += 0.05;
            smoothing = CLAMP(smoothing, 0.0f, 1.0f);
            break;
        default:
            break;
    }
    if(bDirtyDepth){
        if(openNIDevice.getNumDepthThresholds() == 1){
            ofxOpenNIDepthThreshold & dt = openNIDevice.getDepthThreshold(0);
            dt.setNearThreshold(minDepth);
            dt.setFarThreshold(maxDepth);
        }
    }
    
    saveSettings();
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    if(x > controlW){
        ofHideCursor();
    }else{
        ofShowCursor();
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    viewPort->mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    viewPort->mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    viewPort->mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
string testApp::getIPAddress(){
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    string wifiAddress = "";
    string cellAddress = "";
    
    // retrieve the current interfaces - returns 0 on success
    if(!getifaddrs(&interfaces)) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while(temp_addr != NULL) {
            sa_family_t sa_type = temp_addr->ifa_addr->sa_family;
            if(sa_type == AF_INET || sa_type == AF_INET6) {
                string name = temp_addr->ifa_name;
                string addr = inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr); // pdp_ip0     
                cout << "NAME: " << name << " ADDR: " << addr << endl;
                if(name == "en0" || name == "en1") {
                    // Interface is the wifi connection on the iPhone
                    wifiAddress = addr;    
                } else
                    if(name == "pdp_ip0") {
                        // Interface is the cell connection on the iPhone
                        cellAddress = addr;    
                    }
            }
            temp_addr = temp_addr->ifa_next;
        }
        // Free memory
        freeifaddrs(interfaces);
    }
    string addr = wifiAddress != "" ? wifiAddress : cellAddress;
    return addr != "" ? addr : "0.0.0.0";
}

//--------------------------------------------------------------
string testApp::getKinectAppStateAsString(KinectAppState state){
    switch (state) {
        case KINECT_APP_READY:
            return "KINECT_APP_READY";
            break;
        case KINECT_APP_WHITE:
            return "KINECT_APP_WHITE";
            break;
        case KINECT_APP_IMAGE:
            return "KINECT_APP_IMAGE";
            break;
        case KINECT_APP_PLAY:
            return "KINECT_APP_PLAY";
            break;
        case KINECT_APP_LIST:
            return "KINECT_APP_LIST";
            break;
        default:
            return "UNKNOWN";
            break;
    }
}