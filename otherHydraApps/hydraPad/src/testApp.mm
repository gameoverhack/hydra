#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
    
    appState = IOS_APP_INIT;
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    vector<ofxiPhoneExternalDisplayMode> displayModes;
    displayModes = ofxiPhoneExternalDisplay::getExternalDisplayModes();
    ostringstream os;
    if(displayModes.size() > 0){
        for(int i = 0; i < displayModes.size(); i++){
            os << displayModes[i].width << " x " << displayModes[i].height << " aspect: " << displayModes[i].pixelAspectRatio << endl;
        }
        displayModesAsString = os.str();
    }
    
    // get our own ip address and use last part as heartbeat
    vector<string> ipParts = ofSplitString(getIPAddress(), ".");
    appIP = ofToInt(ipParts[ipParts.size() - 1]);
    
    // setup osc
    oscReceiver.setup(6666);
    oscSender.setup("192.168.1.99", 6667);
    
    ftp.setup("192.168.1.99", "hydra", "hydra");
    ftp.setVerbose(true);
    ofAddListener(ftp.iosFTPEvent, this, &testApp::ftpEvent);
    downloadDirectory = "";
    
    dir.reset();
    dir.allowExt("mp4");
    dir.listDir(ftp.getApplicationPath());
    
    appState = IOS_APP_WAIT;
    bShowInfo = true;
    
    ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT);
    
    bMirror = true;
    
    lastHeartBeat = ofGetElapsedTimeMillis();
    
    ofBackground(0, 0, 0);
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    currentFile = "";
    currentFrame = totalFrames = 0;
    
    if(appState == IOS_APP_SYNC){
        ftp.update();
        if(ftp.getNumRequestsInQueue() == 0){
            listApplicationDirectory();
        }
    }
    
    if(appState == IOS_APP_PLAY){
        unsigned found = vid.getMoviePath().find_last_of("/");
        currentFile = vid.getMoviePath().substr(found);
        currentFrame = vid.getCurrentFrame();
        totalFrames = vid.getTotalNumFrames();
    }
    
    if(appState == IOS_APP_SYNC){
        if(ftp.getIsBusy()) currentFile = ftp.getStatus();
    }
    
    if(ofGetElapsedTimeMillis() - lastHeartBeat > 1000){
		ofxOscMessage m;
		m.setAddress("/app/heartbeat");
		m.addIntArg(appIP);
        m.addIntArg(appState);
        m.addStringArg(currentFile);
        m.addIntArg(currentFrame);
        m.addIntArg(totalFrames);
		oscSender.sendMessage(m);
        lastHeartBeat = ofGetElapsedTimeMillis();
	}
    
    while(oscReceiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
        
        if(m.getAddress() == "/app/sync"){
            appState = IOS_APP_SYNC;
            downloadDirectory = m.getArgAsString(0);
            if(downloadDirectory == "") downloadDirectory = "/";
            ftp.listDir(downloadDirectory);
        }
        
        if(m.getAddress() == "/app/play"){
            appState = IOS_APP_PLAY;
            cout << "Play: " << m.getArgAsString(0) << endl;
            playMovie(m.getArgAsString(0));
        }
        
        if(m.getAddress() == "/app/stop"){
            if(appState == IOS_APP_PLAY){
                cout << "Stop" << endl;
                vid.stop();
                appState = IOS_APP_WAIT;
            }
        }
        
        if(m.getAddress() == "/app/list"){
            cout << "List" << endl;
            listApplicationDirectory();
        }
        
        if(m.getAddress() == "/app/delete"){
            cout << "Delete" << endl;
            listApplicationDirectory();
            dir.remove(true);
            listApplicationDirectory();
        }
        
        if(m.getAddress() == "/app/mirror"){
            cout << "Mirror: " << (bool)ofToInt(m.getArgAsString(0)) << endl;
            bMirror = (bool)ofToInt(m.getArgAsString(0));
        }
        
    }
    
    if(appState == IOS_APP_PLAY){
        vid.update();
        if(vid.getIsMovieDone()){
            appState = IOS_APP_WAIT;
        }
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofSetColor(255, 255, 255);
    if(appState == IOS_APP_PLAY){
        if(bMirror){
            vid.draw(ofGetWidth(), 0, -ofGetWidth(), ofGetHeight());
        }else{
            vid.draw(0, 0, ofGetWidth(), ofGetHeight());
        }
    }
    
    if(!bShowInfo) return;
    ostringstream os;
    os << "APP: " << getIOSAppStateAsString(appState) << " " << currentFile << endl;
    os << "DIS: " << displayModesAsString;
    os << "FPS: " << ofGetFrameRate() << endl;
    if(ftp.getIsBusy()) os << "FTP: " << ftp.getStatus() << endl;
    
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(os.str(), 20, 20);
    
}

//--------------------------------------------------------------
void testApp::ftpEvent(ofxIOSimpleFTPEvent & e){
    
    cout << "Recieving FTP event: " << e.eventTypeAsString << " for " << e.object << endl;
    
    switch (e.eventType) {
        case IOSFTP_EVENT_LIST_OK:
        {
            for(int i = 0; i < e.files.size(); i++){
                cout << "Add Get file to ftp queue: " << e.files[i] << endl;
                //cout <<  e.object << " " << downloadDirectory << " " << e.files[i].name.rfind(".mp4") <<  endl;
                if(e.object == downloadDirectory && e.files[i].name.rfind(".mp4") != string::npos){
                    ftp.getFile(e.files[i], e.files[i].name, false);
                }
            }
            //cout << ftp.getNumRequestsInQueue() << endl;
            break;
        }
        case IOSFTP_EVENT_GET_OK:
        {
            cout << "Got file: " << e.files[0] << endl;
            break;
        }
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void testApp::playMovie(string name){
    string path = "";
    for(int i = 0; i < dir.size(); i++){
        if(dir.getPath(i).rfind(name) != string::npos){
            path = dir.getPath(i);
            break;
        }
    }
    if(path != ""){
        vid.loadMovie(path);
        vid.play();
        vid.setLoopState(OF_LOOP_NONE);
    }else{
        ofLogError() << "No movie of that name on device";
        // TODO: send error back to server!
    }
}

//--------------------------------------------------------------
void testApp::listApplicationDirectory(){
    appState = IOS_APP_LIST;
    dir.reset();
    dir.allowExt("mp4");
    dir.listDir(ftp.getApplicationPath());
    ofxOscMessage m;
    m.setAddress("/app/list");
    m.addIntArg(appIP);
    for(int i = 0; i < dir.size(); i++){
        m.addStringArg(dir.getName(i));
    }
    oscSender.sendMessage(m);
    appState = IOS_APP_WAIT;
}

//--------------------------------------------------------------
string testApp::getIPAddress(){
    
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    NSString *wifiAddress = nil;
    NSString *cellAddress = nil;
    
    // retrieve the current interfaces - returns 0 on success
    if(!getifaddrs(&interfaces)) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while(temp_addr != NULL) {
            sa_family_t sa_type = temp_addr->ifa_addr->sa_family;
            if(sa_type == AF_INET || sa_type == AF_INET6) {
                NSString *name = [NSString stringWithUTF8String:temp_addr->ifa_name];
                NSString *addr = [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr)]; // pdp_ip0     
                NSLog(@"NAME: \"%@\" addr: %@", name, addr); // see for yourself
                
                if([name isEqualToString:@"en0"]) {
                    // Interface is the wifi connection on the iPhone
                    wifiAddress = addr;    
                } else
                    if([name isEqualToString:@"pdp_ip0"]) {
                        // Interface is the cell connection on the iPhone
                        cellAddress = addr;    
                    }
            }
            temp_addr = temp_addr->ifa_next;
        }
        // Free memory
        freeifaddrs(interfaces);
    }
    NSString *addr = wifiAddress ? wifiAddress : cellAddress;
    return [addr ? addr : @"0.0.0.0" cStringUsingEncoding: NSASCIIStringEncoding];
}

//--------------------------------------------------------------
string testApp::getIOSAppStateAsString(IOSAppState appState){
    switch (appState) {
        case IOS_APP_INIT:
            return "IOS_APP_INIT";
            break;
        case IOS_APP_SYNC:
            return "IOS_APP_SYNC";
            break;
        case IOS_APP_PLAY:
            return "IOS_APP_PLAY";
            break;
        case IOS_APP_LIST:
            return "IOS_APP_LIST";
            break;
        case IOS_APP_WAIT:
            return "IOS_APP_WAIT";
            break;
    }
};

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch){
    bShowInfo = !bShowInfo;
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs & touch){
    if(ofxiPhoneGetOrientation() == OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT){
        ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_PORTRAIT);
        //ofxiPhoneExternalDisplay::mirrorOn();
        ofxiPhoneExternalDisplay::displayOnExternalScreenWithPreferredDisplayMode();
    }else{
        ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT);
        //ofxiPhoneExternalDisplay::mirrorOn();
        //ofxiPhoneExternalDisplay::displayOnExternalScreenWithPreferredDisplayMode();
        ofxiPhoneExternalDisplay::displayOnDeviceScreen();
    }
    
}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::lostFocus(){

}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){
    //ofxiPhoneSetOrientation((ofOrientation)newOrientation);
}

