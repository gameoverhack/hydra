#include "IOSController.h"

IOSController::IOSController() {

    LOG_NOTICE("Constructing IOSController");

    registerStates();

    oscReceiver.setup(6667);

}

IOSController::~IOSController() {
    LOG_NOTICE("Destructing IOSController");
}

// state registration
//--------------------------------------------------------------
void IOSController::registerStates() {

	LOG_VERBOSE("Registering States");

	registerState(kIOSCONTROLLER_INIT, "kIOSCONTROLLER_INIT");
	registerState(kIOSCONTROLLER_READY, "kIOSCONTROLLER_READY");

	_appModel->registerStatefulClass(typeid(*this).name());

	setState(kIOSCONTROLLER_INIT);

}

//--------------------------------------------------------------
void IOSController::update() {

    map<int, IOSVideoPlayer*>& iosVideoPlayers = _appModel->getIOSVideoPlayers();

    for(map<int, IOSVideoPlayer*>::iterator it = iosVideoPlayers.begin(); it != iosVideoPlayers.end(); ++it){
        IOSVideoPlayer * iosVideoPlayer = it->second;
        if(iosVideoPlayer->iosAppState != IOS_APP_OFF && ofGetElapsedTimeMillis() - iosVideoPlayer->lastHeartBeat > 3000){
            LOG_WARNING("IOS OSC Device gone offline: " + ofToString(it->first));
//            iosVideoPlayers.erase(it);
            iosVideoPlayer->iosAppState = IOS_APP_OFF;
            iosVideoPlayer->files.clear();
            iosVideoPlayer->currentFile = "";
            iosVideoPlayer->currentFrame = iosVideoPlayer->totalFrames = 0;
            iosVideoPlayer->bNeedsDisplayUpdate = true;
        }
    }

    while(oscReceiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);

        int iosIPID =  m.getArgAsInt32(0);

        //cout << "Received: " << m.getAddress() << " from iosIPID: " << iosIPID << " ";

        if(m.getAddress() == "/app/heartbeat"){
            IOSAppState iosAppState = (IOSAppState)m.getArgAsInt32(1);
            string currentFile = m.getArgAsString(2);
            int currentFrame = m.getArgAsInt32(3);
            int totalFrames = m.getArgAsInt32(4);
            map<int, IOSVideoPlayer*>::iterator it = iosVideoPlayers.find(iosIPID);
            //cout << iosAppState << " " << currentFile << " " << currentFrame << " / " << totalFrames << endl;
            if(it == iosVideoPlayers.end()){
                LOG_VERBOSE("Creating IOS Device");
                IOSVideoPlayer * iosVideoPlayer = new IOSVideoPlayer;
                iosVideoPlayer->lastHeartBeat = ofGetElapsedTimeMillis();
                iosVideoPlayer->iosAppState = iosAppState;
                iosVideoPlayer->oscSender->setup("192.168.1." + ofToString(iosIPID), 6666);
                iosVideoPlayers[iosIPID] = iosVideoPlayer;
                _appModel->sendIOSOsc("/app/list", iosIPID);
            }else{
                IOSVideoPlayer * iosVideoPlayer = it->second;
                if(iosVideoPlayer->iosAppState == IOS_APP_OFF){
                    LOG_VERBOSE("Re-list IOS Device");
                    _appModel->sendIOSOsc("/app/list", iosIPID);
                }
                iosVideoPlayer->lastHeartBeat = ofGetElapsedTimeMillis();
                iosVideoPlayer->iosAppState = iosAppState;
                iosVideoPlayer->currentFile = currentFile;
                iosVideoPlayer->currentFrame = currentFrame;
                iosVideoPlayer->totalFrames = totalFrames;
            }
        }

        if(m.getAddress() == "/app/list"){
            cout << "List directory received files: " << endl;
            iosVideoPlayers[iosIPID]->files.clear();
            for(int i = 1; i < m.getNumArgs(); i++){
                cout << m.getArgAsString(i) << endl;
                iosVideoPlayers[iosIPID]->files.push_back(m.getArgAsString(i));
            }
            iosVideoPlayers[iosIPID]->bNeedsDisplayUpdate = true;
        }
    }

}
