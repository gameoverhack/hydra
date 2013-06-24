#include "IOSController.h"

IOSController::IOSController() {

    LOG_NOTICE("Constructing IOSController");

    registerStates();

//    for (int i = 0; i < MAX_SERVERS; i++) {
//        string portName = boost::any_cast<string>(_appModel->getProperty("port" + ofToString(i+1)));
//        int port = ofToInt(portName);
//        LOG_NOTICE("Attempt server setup on port: " + portName);
//        _appModel->addServer("port" + ofToString(i), port);
//    }

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

//    map< string, TextServer* >& servers = _appModel->getServers();
//    map< string, TextServer* >::iterator it;
//
//    for (it = servers.begin(); it != servers.end(); it++) {
//        TextServer* server = it->second;
//        ofxTCPServer* tcpServer = server->getServer();
//        if (tcpServer->getNumClients() < 1) continue;
//        TextObject* preset = server->getPreset();
//        if (preset == NULL) continue;
//        TextCue* cue = preset->getCue();
//        if (cue == NULL) continue;
//        //LOG_VERBOSE("Send to " + it->first + " :: " + cue->getMessage());
//        if (cue->getMessage() == "STOP^" && preset->getIsPlaying()) preset->stopPlaying();
//        tcpServer->sendToAll(cue->getMessage());
//    }

}
