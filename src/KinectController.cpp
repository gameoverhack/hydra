#include "KinectController.h"

//--------------------------------------------------------------
KinectController::KinectController(){

    LOG_NOTICE("Constructing KinectController");

    registerStates();

    oscReceiver.setup(6669);
}

//--------------------------------------------------------------
KinectController::~KinectController(){
    LOG_NOTICE("Destructing KinectController");
}

// state registration
//--------------------------------------------------------------
void KinectController::registerStates() {

	LOG_VERBOSE("Registering States");

	registerState(kKINECTCONTROLLER_INIT, "kKINECTCONTROLLER_INIT");
	registerState(kKINECTCONTROLLER_READY, "kKINECTCONTROLLER_READY");

	_appModel->registerStatefulClass(typeid(*this).name());

	setState(kKINECTCONTROLLER_INIT);

}

//--------------------------------------------------------------
void KinectController::update() {

    KinectVideoPlayer& kinectVideoPlayer = _appModel->getKinectVideoPlayer();
    if(kinectVideoPlayer.isSetup){
        if(kinectVideoPlayer.kinectAppState != KINECT_APP_OFF && ofGetElapsedTimeMillis() - kinectVideoPlayer.lastHeartBeat > 3000){
            LOG_WARNING("Kinect OSC Device gone offline");
            //kinectVideoPlayer.isSetup = false;
            kinectVideoPlayer.kinectAppState = KINECT_APP_OFF;
            kinectVideoPlayer.files.clear();
            kinectVideoPlayer.currentFile = "";
            kinectVideoPlayer.currentFrame = kinectVideoPlayer.totalFrames = 0;
            kinectVideoPlayer.bNeedsDisplayUpdate = true;
        }
    }
    while(oscReceiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);

        int kinectIPID = m.getArgAsInt32(0);

        //cout << "Received: " << m.getAddress() << " from kinectIPID: " << kinectIPID << " ";

        if(m.getAddress() == "/app/heartbeat"){
            KinectAppState kinectAppState = (KinectAppState)m.getArgAsInt32(1);
            string currentFile = m.getArgAsString(2);
            int currentFrame = m.getArgAsInt32(3);
            int totalFrames = m.getArgAsInt32(4);
            //cout << kinectAppState << " " << currentFile << " " << currentFrame << " / " << totalFrames << endl;
            if(!kinectVideoPlayer.isSetup){
                kinectVideoPlayer.lastHeartBeat = ofGetElapsedTimeMillis();
                kinectVideoPlayer.oscSender->setup("192.168.1." + ofToString(kinectIPID), 6668);
                kinectVideoPlayer.kinectAppState = kinectAppState;
                kinectVideoPlayer.isSetup = true;
                _appModel->sendAllKinectOsc("/app/list", "");
            }else{
                if(kinectVideoPlayer.kinectAppState == KINECT_APP_OFF){
                    _appModel->sendAllKinectOsc("/app/list", "");
                }
                kinectVideoPlayer.lastHeartBeat = ofGetElapsedTimeMillis();
                kinectVideoPlayer.kinectAppState = kinectAppState;
                kinectVideoPlayer.currentFile = currentFile;
                kinectVideoPlayer.currentFrame = currentFrame;
                kinectVideoPlayer.totalFrames = totalFrames;
            }
        }

        if(m.getAddress() == "/app/list"){
            cout << "List directory received files: " << endl;
            kinectVideoPlayer.files.clear();
            for(int i = 1; i < m.getNumArgs(); i++){
                cout << m.getArgAsString(i) << endl;
                kinectVideoPlayer.files.push_back(m.getArgAsString(i));
            }
            kinectVideoPlayer.bNeedsDisplayUpdate = true;
        }
    }

//    map<int, IOSVideoPlayer*>& iosVideoPlayers = _appModel->getIOVideoPlayers();
//
////    if(ofGetFrameNum() % 120 == 0 && bStressTest){
////        vector<string> & files = (iosVideoPlayers.begin())->second->files;
////        string file = files[ofRandom(files.size() - 1)];
////        _appModel->sendAllIPADosc("/app/play", file);
////    }
//
//    while(oscReceiver.hasWaitingMessages()){
//		// get the next message
//		ofxOscMessage m;
//		oscReceiver.getNextMessage(&m);
//
//        int iosIPID =  m.getArgAsInt32(0);
//
//        //cout << "Received: " << m.getAddress() << " from iosIPID: " << iosIPID << " ";
//
//        if(m.getAddress() == "/app/heartbeat"){
//            string ipadAppState = _appModel->getIOSAppStateAsString((IOSAppState)m.getArgAsInt32(1));
//            string currentFile = m.getArgAsString(2);
//            int currentFrame = m.getArgAsInt32(3);
//            int totalFrames = m.getArgAsInt32(4);
//            map<int, IOSVideoPlayer*>::iterator it = iosVideoPlayers.find(iosIPID);
//            //cout << ipadAppState << " " << currentFile << " " << currentFrame << " / " << totalFrames << endl;
//            if(it == iosVideoPlayers.end()){
//                IOSVideoPlayer * iosVideoPlayer = new IOSVideoPlayer;
//                iosVideoPlayer->oscSender->setup("192.168.1." + ofToString(iosIPID), 6666);
//                iosVideoPlayers[iosIPID] = iosVideoPlayer;
//                _appModel->sendIPADosc("/app/list", iosIPID);
//            }else{
//                IOSVideoPlayer * iosVideoPlayer = it->second;
//                iosVideoPlayer->currentFile = currentFile;
//                iosVideoPlayer->currentFrame = currentFrame;
//                iosVideoPlayer->totalFrames = totalFrames;
//            }
//        }
//
//        if(m.getAddress() == "/app/list"){
//            cout << "List directory received files: " << endl;
//            iosVideoPlayers[iosIPID]->files.clear();
//            for(int i = 1; i < m.getNumArgs(); i++){
//                cout << m.getArgAsString(i) << endl;
//                iosVideoPlayers[iosIPID]->files.push_back(m.getArgAsString(i));
//            }
//            iosVideoPlayers[iosIPID]->bNeedsDisplayUpdate = true;
//        }
//    }

}
