#include "TextController.h"

TextController::TextController() {

    LOG_NOTICE("Constructing TextController");

    registerStates();

    for (int i = 0; i < MAX_SERVERS; i++) {
        string portName = boost::any_cast<string>(_appModel->getProperty("port" + ofToString(i+1)));
        int port = ofToInt(portName);
        LOG_NOTICE("Attempt server setup on port: " + portName);
        _appModel->addServer("port" + ofToString(i), port);
    }

}

TextController::~TextController() {
    LOG_NOTICE("Destructing TextController");
}

// state registration
//--------------------------------------------------------------
void TextController::registerStates() {

	LOG_VERBOSE("Registering States");

	registerState(kTEXTCONTROLLER_INIT, "kTEXTCONTROLLER_INIT");
	registerState(kTEXTCONTROLLER_READY, "kTEXTCONTROLLER_READY");

	_appModel->registerStatefulClass(typeid(*this).name());

	setState(kTEXTCONTROLLER_INIT);

}

//--------------------------------------------------------------
void TextController::update() {

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
