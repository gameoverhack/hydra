/*
 *  DebugView.cpp
 *  Bunyip
 *
 *  Created by gameover on 23/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#include "DebugView.h"

//--------------------------------------------------------------
DebugView::DebugView(float x, float y, float width, float height, ofxFenster* window, string windowTitle) : BaseView(x, y, width, height, window, windowTitle) {

	LOG_NOTICE("Constructing DebugView");

    _prompterScreens.resize(MAX_SERVERS);

    int fontSize = 30;
    float lineHeight = (float)fontSize + 4.0f;
    float letterSpacing = 1.035f;
    ofTrueTypeFont::setGlobalDpi(72);
    verdana.loadFont("fonts/verdana.ttf", fontSize, true, true);
    verdana.setLineHeight(lineHeight);
    verdana.setLetterSpacing(letterSpacing);
}

//--------------------------------------------------------------
void DebugView::update() {

	string msg = "FPS: " + ofToString(ofGetFrameRate()) + "\n\n";

	if (boost::any_cast<bool>(_appModel->getProperty("showProps"))) {
		msg += _appModel->getAllPropsAsList() + "\n";
		msg += _keyModel->getAllKeyboardUsageAsList(false) + "\n";
		msg += _appModel->getAllStatesAsList() + "\n";
	}

    Scene* scene = _appModel->getCurrentScene();
    if (scene != NULL){
        vector<VideoObject*> videoObjects = scene->getVideos();
        for (int i = 0; i < videoObjects.size(); i++) {
            msg += videoObjects[i]->getInfo() + "\n";
        }
    }

	begin();
	{
	    ofNoFill();

        glPushMatrix();
        ofSetColor(255,255,255,255);
        glTranslatef(40, 40, 0);
        glScalef(0.3f, 0.3f, 1.0f);
        drawCameraViews();
        glPopMatrix();

        glPushMatrix();
        ofSetColor(255,255,255,255);
        glTranslatef(40, (2*480.0f+3.0f)*0.3f + 60, 0);
        glScalef(0.3f, 0.3f, 1.0f);
        drawOutputView();
        ofRect(0,0,1920,1080);
        glPopMatrix();

        glPushMatrix();
        ofSetColor(255, 255, 255);
        ofDrawBitmapString(msg, 40, 700);
        ofSetColor(255, 255, 255);
        glPopMatrix();
	}

	end();

}

void DebugView::drawOutputView() {

    Scene* scene = _appModel->getCurrentScene();
    if (scene == NULL) return;

    vector<VideoObject*> videoObjects = scene->getVideos();

    string patternName = scene->getPatternName();
    if (patternName == "") return;

    vector<ofRectangle*> currentPattern = _appModel->getPattern(patternName);

    ofSetColor(255,255,255,255);
    for (int i = 0; i < videoObjects.size(); i++) {

        float x = currentPattern[i]->x;
        float y = currentPattern[i]->y;
        float w = currentPattern[i]->width;
        float h = currentPattern[i]->height;

        switch(videoObjects[i]->_inputType) {
            case GO_VIDEO_PLAYER:
                videoObjects[i]->_player->draw(x,y,w,h);
                break;
            case GO_VIDEO_CAMERA:
                videoObjects[i]->_camera->draw(x,y,w,h);
                break;

        }
    }
}

void DebugView::drawCameraViews() {
    map< string, ofVideoGrabber* >& cameras = _appModel->getCameras();
    map< string, ofVideoGrabber* >::iterator it;
    int position = 0;
    for (it = cameras.begin(); it != cameras.end(); it++, position++) {
        ofVideoGrabber* camera = it->second;
        if (camera == NULL) continue;
        int camWidth = 720;//camera->getWidth();
        int camHeight = 480;//camera->getHeight();
        int offsetX = camWidth;
        if(position == 0 || position == 2) offsetX = 0;
        if(position == 4) offsetX = camWidth * 2;
        int offsetY = (position < 2 ? 0 : camHeight);
        ofNoFill();
        ofSetColor(255, 255, 255);
        if (boost::any_cast<bool>(_appModel->getProperty("showDebug"))) {
            camera->draw(offsetX,offsetY,camWidth,camHeight);
        }else{
            ofSetColor(255, 0, 0);
        }
        ofRect(offsetX,offsetY,camWidth,camHeight);
    }
}

void DebugView::drawPrompterViews() {
    map< string, TextServer* >& servers = _appModel->getServers();
    map< string, TextServer* >::iterator it;

    int fboOffset = 0;
    int promptIndex = 0;
    for (it = servers.begin(); it != servers.end(); it++, promptIndex++) {
        TextServer* server = it->second;
        ofxTCPServer* tcpServer = server->getServer();
        if (tcpServer->getNumClients() < 1) continue;
        TextObject* preset = server->getPreset();
        if (preset == NULL) continue;
        ofFbo* fbo = preset->getFBO();
        if (preset->renderFBO == true && preset->getText() != "") {
            preset->renderFBO = false;
            if (fbo != NULL) delete fbo;
            if (_prompterScreens[promptIndex] == NULL) {
                _prompterScreens[promptIndex] = new ofFbo();
                _prompterScreens[promptIndex]->allocate(preset->getTargetScreenWidth(), preset->getTargetScreenHeight());
            }
            ofRectangle bounds = preset->getBounds();
            fbo = new ofFbo();
            fbo->allocate(bounds.width, bounds.height, GL_RGB);
            int nFBO = ceil((float)bounds.height/preset->getTargetScreenHeight());
            fbo->begin();
            {
                glPushMatrix();
                glClearColor(1.0, 1.0, 1.0, 1.0);
                glClear(GL_COLOR_BUFFER_BIT);
                ofSetColor(0, 0, 0, 255);
                verdana.drawString(preset->getText(), 0, 30); // 30 is font size
                glPopMatrix();
            }
            fbo->end();
            preset->setFBO(fbo);
        }
        if (fbo == NULL) continue;

        string recString = "";
        for (int i = 0; i < tcpServer->getNumClients(); i++) {
            recString = tcpServer->receive(i);
            if (recString != "") break;
        }
        vector<string> recVec = ofSplitString(recString, "^");

        if (recVec.size() > 0) {
            if (recVec[0] == "Y") preset->scrollY = ofToFloat(recVec[1]);
        }

        _prompterScreens[promptIndex]->begin();
        {
            glPushMatrix();
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT);
            ofSetColor(255,255,255,255);
            glTranslatef(0.0f, -preset->scrollY, 0.0f);
            fbo->draw(0,0);
            glPopMatrix();
        }
        _prompterScreens[promptIndex]->end();

        glPushMatrix();
        glTranslatef(fboOffset,0,0);
        _prompterScreens[promptIndex]->draw(0,0);
        glPopMatrix();
        fboOffset += preset->getTargetScreenWidth();

    }
}
