/*
 *  GuiView.cpp
 *  emptyExample
 *
 *  Created by gameover on 22/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#include "GuiView.h"

//--------------------------------------------------------------
GuiView::GuiView(float x, float y, float width, float height, ofxFenster* window, string windowTitle) : BaseView(x, y, width, height, window, windowTitle) {

	LOG_NOTICE("Constructing GuiView");

    _functionModel->registerFunction("GuiView::autoMidiMap", MakeDelegate(this, &GuiView::autoMidiMap));

	for (int i = 0; i < 20; i++) {
        _midiModel->registerEvent(1, 144, i, 0, kMIDI_ANY, "port 0 jump to preset " + ofToString(i), "GuiView::autoMidiMap", 1, i);
        _midiModel->registerEvent(2, 144, i+20, 0, kMIDI_ANY, "port 1 jump to preset " + ofToString(i), "GuiView::autoMidiMap", 2, i);
        _midiModel->registerEvent(3, 144, i+40, 0, kMIDI_ANY, "port 2 jump to preset", "GuiView::autoMidiMap", 3, i);
	}
	for (int i = 0; i < 40; i++) {
        _midiModel->registerEvent(4, 144, i+60, 0, kMIDI_ANY, "jump to video" + ofToString(i), "GuiView::autoMidiMap", 4, i);
	}

    for (int i = 0; i < 20; i++) {
        _midiModel->registerEvent(5, 144, i+100, 0, kMIDI_ANY, "special trigger" + ofToString(i), "GuiView::autoMidiMap", 5, i);
	}
}

//--------------------------------------------------------------
GuiView::~GuiView() {
	LOG_NOTICE("Destroying GuiView");
}

//--------------------------------------------------------------
void GuiView::registerStates() {

	LOG_VERBOSE("Registering States");

	registerState(kGUIVIEW_INIT, "kGUIVIEW_INIT");
	registerState(kGUIVIEW_SCENE, "kGUIVIEW_SCENE");
	registerState(kGUIVIEW_CALIBRATE, "kGUIVIEW_CALIBRATE");
	registerState(kGUIVIEW_LOAD, "kGUIVIEW_LOAD");

	_appModel->registerStatefulClass(typeid(*this).name());

	setState(kGUIVIEW_LOAD);

}

void GuiView::autoMidiMap(int port, int byteTwo) {
    LOG_VERBOSE("Auto Midi Map: " + ofToString (port) + " :: " + ofToString(byteTwo));
    hEvents * events = hEvents::getInstance();
    switch (port) {
        case 1: // select preset
        {
            LOG_VERBOSE("Text MIDI TRIGGER PORT 1");
//            hEventArgs args;
//            args.eventName = "selectPreset_0";
//            TextObject* preset = _appModel->getPreset(byteTwo);
//            if (preset == NULL) return;
//            args.strings.push_back(preset->getName());
//            selectPreset(args);
//            preset->rewind();
//            preset->startPlaying();
            break;
        }
        case 2: // select preset
        {
            LOG_VERBOSE("Text MIDI TRIGGER PORT 2");
//            hEventArgs args;
//            args.eventName = "selectPreset_1";
//            TextObject* preset = _appModel->getPreset(byteTwo);
//            if (preset == NULL) return;
//            args.strings.push_back(preset->getName());
//            selectPreset(args);
//            preset->rewind();
//            preset->startPlaying();
            break;
        }
        case 3: // select preset
        {
            LOG_VERBOSE("Text MIDI TRIGGER PORT 3");
//            hEventArgs args;
//            args.eventName = "selectPreset_2";
//            TextObject* preset = _appModel->getPreset(byteTwo);
//            if (preset == NULL) return;
//            args.strings.push_back(preset->getName());
//            selectPreset(args);
//            preset->rewind();
//            preset->startPlaying();
            break;
        }
        case 4: // select video
        {
            LOG_VERBOSE("Video MIDI TRIGGER");
            hEventArgs args;
            args.eventName = "selectScene";
            Scene* scene = _appModel->getScene(byteTwo);
            if (scene == NULL) return;
            args.strings.push_back(scene->getName());
            selectScene(args);
            break;
        }
        case 5:
        {
            LOG_VERBOSE("SPECIAL TRIGGER");
            Scene* scene = _appModel->getCurrentScene();
            if (scene == NULL) return;
            switch (byteTwo) {
                case 0: // rewind current scene
                    _appModel->rewindScene(scene);
                    break;
                case 1: // rewind current scene
                    _appModel->rewindAll();
                    break;

            }
            break;
        }

    }
}

//--------------------------------------------------------------
void GuiView::setup() {

    LOG_NOTICE("Setup GuiView");

    hGui * gui = hGui::getInstance();
    gui->setup("fonts/DIN.otf", 9);
    gui->setFixedFont("fonts/lmmonolt10-bold.otf", 10, 0);

    // create main panels
    hPanel * mainscenesPanel = gui->addPanel("Application", NULL, HGUI_ABSOLUTE_POSITION, gui->margin1, gui->margin1+gui->margin3+5, 883, 990, true);
    mainscenesPanel->setVisibleBackground(true);

//    hPanel * maincommPanel = gui->addPanel("Video", NULL, HGUI_ABSOLUTE_POSITION, gui->margin1, gui->margin1+gui->margin3+5, 883, 990, true);
//	maincommPanel->setVisibleBackground(true);
//
//	hPanel * mainlayoutPanel = gui->addPanel("Text", NULL, HGUI_ABSOLUTE_POSITION, gui->margin1, gui->margin1+gui->margin3+5, 883, 990, true);
//	mainlayoutPanel->setVisibleBackground(true);

    // create tabs for panels
    int mainTabBoxWidth = mainscenesPanel->getWidth();

	hTabBox *  mainTabBox = gui->addTabBox("mainTabBox", NULL, HGUI_ABSOLUTE_POSITION, gui->margin1, 5, mainTabBoxWidth);
	mainTabBox->addItems(3);

	mainTabBox->setItemLabel(1, "Hydra");
	mainTabBox->setItemPanel(1, mainscenesPanel);
//	mainTabBox->setItemLabel(2, "Video");
//	mainTabBox->setItemPanel(2, maincommPanel);
//	mainTabBox->setItemLabel(3, "Text");
//	mainTabBox->setItemPanel(3, mainlayoutPanel);

	mainTabBox->setItemPanel(1, mainscenesPanel);
	mainTabBox->selectItem(1);

	gui->setRootWidget(mainTabBox);

	// setup events
    hEvents * events = hEvents::getInstance();
	events->setup();

    events->set_verbose(false);
	events->addObject("GuiView", this);

    // create widgets for Application mainscenesPanel
    int halfWidth = (mainscenesPanel->getWidth() - gui->margin2 * 3)/2;

    // Scene Control Panel

    hPanel * scenesPanel = gui->addPanel("scenes", mainscenesPanel, HGUI_TOP_LEFT, gui->margin2, gui->margin1, halfWidth, 250, true);
	hLabel * scenesPanelLabel = gui->addLabel("", scenesPanel, HGUI_TOP_LEFT, 2, 0, "Scenes");

    hTextBox * videoPath = gui->addTextBox("videoPath", scenesPanel,  HGUI_NEXT_ROW, gui->margin2, gui->margin2, 300, boost::any_cast<string>(_appModel->getProperty("videoPath")));
    hLabel * labelText1 = gui->addLabel("", scenesPanel, HGUI_RIGHT, 2, 0, "video path");

    hTextBox * sceneName = gui->addTextBox("sceneName", scenesPanel,  HGUI_NEXT_ROW, gui->margin2, gui->margin2, 300, "");
    hLabel * sceneNameLabel = gui->addLabel("", scenesPanel, HGUI_RIGHT, 2, 0, "new scene name");
    sceneName->clearAfterReturn(true);

    events->addListener("updateVideoPath", this, &GuiView::updateVideoPath);
    videoPath->setMessage("updateVideoPath");

    events->addListener("addScene", this, &GuiView::addScene);
    sceneName->setMessage("addScene");

    hListBox * sceneList = gui->addListBox("sceneList", scenesPanel, HGUI_NEXT_ROW, gui->margin2+2, gui->margin2, 200);
    sceneList->addItems(6,"");
    events->addListener("selectScene", this, &GuiView::selectScene);
    updateSceneList();

    hButton * delButton = gui->addButton("del", scenesPanel, HGUI_ABSOLUTE_POSITION, 25, 240, 80, "delete");
    events->addListener("del", this, &GuiView::del);
    delButton->setMessage("del");

    hButton * delAllButton = gui->addButton("delAll", scenesPanel, HGUI_ABSOLUTE_POSITION, 25 + 90, 240, 80, "delete all");
    events->addListener("delAll", this, &GuiView::delAll);
    delAllButton->setMessage("delAll");

    hButton * saveButton = gui->addButton("save", scenesPanel, HGUI_ABSOLUTE_POSITION, 25 + 90 * 2, 240, 80, "save");
    events->addListener("save", this, &GuiView::save);
    saveButton->setMessage("save");

    hButton * rewindButton = gui->addButton("rewind", scenesPanel, HGUI_ABSOLUTE_POSITION, 25 + 90 * 3, 240, 80, "rewind");
    events->addListener("rewind", this, &GuiView::rewind);
    rewindButton->setMessage("rewind");

    // Communication Control Panel

	hPanel * commPanel = gui->addPanel("communication", mainscenesPanel, HGUI_RIGHT, gui->margin2, 0, halfWidth, 250, true);
	hLabel * commPanelLabel = gui->addLabel("", commPanel, HGUI_TOP_LEFT, 2, 0, "Communication");

//    hTextBox * textPath = gui->addTextBox("textPath", commPanel,  HGUI_NEXT_ROW, gui->margin2, gui->margin2, 300, boost::any_cast<string>(_appModel->getProperty("textPath")));
//    hLabel * textPathLabel = gui->addLabel("", commPanel, HGUI_RIGHT, 2, 0, "text path");

    // Scene Layout Panel

    hPanel * layoutPanel = gui->addPanel("layout", mainscenesPanel, HGUI_NEXT_ROW, gui->margin2, gui->margin2, mainscenesPanel->getWidth()-2*gui->margin2, 180, true);
	hLabel * layoutPanelLabel = gui->addLabel("", layoutPanel, HGUI_TOP_LEFT, 2, 0, "Video/Camera Layout");

    hListBox * patternList = gui->addListBox("patternList", layoutPanel, HGUI_NEXT_ROW, gui->margin2, 0, 120);
    patternList->addItems(7, "");

    map< string, vector<ofRectangle*> >& patterns = _appModel->getPatterns();
    map< string, vector<ofRectangle*> >::iterator it;

    int item = 1;
    for (it = patterns.begin(); it != patterns.end(); it++, item++) {
        patternList->setElementLabel(item, it->first);
    }

    hButtonBox * patternLayout = gui->addButtonBox("patternLayout", layoutPanel, HGUI_NEXT_COL, gui->margin2-100, 17, 32, 20);
    hLabel * patternLayoutLabel = gui->addLabel("patternLayoutLabel", layoutPanel, HGUI_RIGHT, -patternLayout->getWidth(), -16, "ONscreenLayout");
	patternLayout->addItems(3,2);
	patternLayout->displayIndexes(true);

    hButtonBox * patternLayoutOFF = gui->addButtonBox("patternLayoutOFF", layoutPanel, HGUI_NEXT_COL, gui->margin2-7-patternLayoutLabel->getWidth(), 80, 32, 20);
    hLabel * patternLayoutOFFLabel = gui->addLabel("patternLayoutOFFLabel", layoutPanel, HGUI_RIGHT, -20, -16, "OFFscreenLayout");
    patternLayoutOFF->addItems(3,2);
	patternLayoutOFF->displayIndexes(true);

    hListBox * cameraList = gui->addListBox("cameraList", layoutPanel, HGUI_NEXT_COL, gui->margin2 + patternLayout->getWidth() - 40, 17, 120);
    hLabel * cameraListLabel = gui->addLabel("cameraListLabel", layoutPanel, HGUI_RIGHT, -cameraList->getWidth(), -16, "camera");
    cameraList->addItems(5, "");

    for (int i = 0; i < 5; i++) {
        cameraList->setElementLabel(i+1, "camera_" + ofToString(i+1));
    }

    hListBox * videoList = gui->addListBox("videoList", layoutPanel, HGUI_NEXT_COL, gui->margin2, 17, 220);
    hLabel * videoListLabel = gui->addLabel("videoListLabel", layoutPanel, HGUI_RIGHT, -videoList->getWidth(), -16, "video");

    hCheckBox * offscreenCB = gui->addCheckBox("offScreen", layoutPanel, HGUI_NEXT_COL, gui->margin2+videoList->getWidth(), 17);
    hLabel * offscreenCBLabel = gui->addLabel("offscreenCBLabel", layoutPanel, HGUI_RIGHT, 2, 0, "offscreen");
    offscreenCB->setBoolVar(&bOffScreen);

    hCheckBox * degradeCB = gui->addCheckBox("degrade", layoutPanel, HGUI_NEXT_COL, -offscreenCB->getWidth() - 38, 17 + offscreenCB->getHeight() * 2);
    hLabel * degradeCBLabel = gui->addLabel("degradeCBLabel", layoutPanel, HGUI_RIGHT, 2, 0, "degrade");
    degradeCB->setBoolVar(&bDegraded);

    hSlider * volumeSlider = gui->addSlider("volumeSlider", layoutPanel, HGUI_NEXT_COL, -degradeCB->getWidth() - 34, 17 + degradeCB->getHeight() * 4, 100);
    hLabel * volumeSliderLabel = gui->addLabel("volumeSliderLabel", layoutPanel, HGUI_RIGHT, 2, 0, "volume");
    hLabel * volumeSliderValLabel = gui->addLabel("volumeSliderValLabel", layoutPanel, HGUI_RIGHT, 0, 0, "");
    volumeSlider->setLinkedLabel(volumeSliderValLabel, true);
    volumeSlider->setFloatVar(&fVolume);
    volumeSlider->setRange(0.0, 1.0, 2);
    volumeSlider->setValue(1.0);

    hSlider * panSlider = gui->addSlider("panSlider", layoutPanel, HGUI_NEXT_COL, -degradeCB->getWidth() - volumeSlider->getWidth() - 16, 17 + volumeSlider->getHeight() * 6, 100);
    hLabel * panSliderLabel = gui->addLabel("panSliderLabel", layoutPanel, HGUI_RIGHT, 2, 0, "pan");
    hLabel * panSliderValLabel = gui->addLabel("panSliderValLabel", layoutPanel, HGUI_RIGHT, 0, 0, "");
    panSlider->setLinkedLabel(panSliderValLabel, true);
    panSlider->setFloatVar(&fPan);
    panSlider->setRange(-1.0, 1.0, 2);
    panSlider->setValue(0.0);

    events->addListener("setVolume", this, &GuiView::setVolume);
    volumeSlider->setMessage("setVolume");

    events->addListener("setPan", this, &GuiView::setPan);
    panSlider->setMessage("setPan");

    events->addListener("setOffScreen", this, &GuiView::setOffScreen);
    offscreenCB->setMessage("setOffScreen");

    events->addListener("setDegraded", this, &GuiView::setDegraded);
    degradeCB->setMessage("setDegraded");

    events->addListener("changePattern", this, &GuiView::changePattern);
    patternList->setMessage("changePattern");

    events->addListener("selectPattern", this, &GuiView::selectPattern);
    events->addListener("selectPatternOFF", this, &GuiView::selectPattern);
    patternLayout->setMessage("selectPattern");
    patternLayoutOFF->setMessage("selectPatternOFF");

    events->addListener("selectCamera", this, &GuiView::selectCamera);
    cameraList->setMessage("selectCamera");

    events->addListener("selectVideo", this, &GuiView::selectVideo);
    videoList->setMessage("selectVideo");

    updateVideoList();

    patternList->setAllElementsDisabled(true);
    cameraList->setAllElementsDisabled(true);
    videoList->setAllElementsDisabled(true);
    patternLayout->setAllElementsDisabled(true);
    patternLayoutOFF->setAllElementsDisabled(true);

    // IOS Communication Panels

    for (int i = 0; i < MAX_SERVERS; i++) {

//        hPanel * serverPanel = gui->addPanel("server" + ofToString(i), mainscenesPanel, HGUI_NEXT_ROW, gui->margin2, gui->margin1, (i%2 == 0 ? 0 : halfWidth), 250, true);
//        hLabel * serverPanelLabel = gui->addLabel("", serverPanel, HGUI_TOP_LEFT, 2, 0, "Server_" + ofToString(i));
    }

    for (int i = 0; i < MAX_SERVERS; i++) {

//        string panelID = ofToString(i);
//
//        hPanel * textPanel = gui->addPanel("textPanel" + panelID, mainscenesPanel, HGUI_NEXT_ROW, gui->margin2, gui->margin2, mainscenesPanel->getWidth()-2*gui->margin2, 170, true);
//        hLabel * textPanelLabel = gui->addLabel("", textPanel, HGUI_TOP_LEFT, 2, 0, "Text (Port " + panelID + ")");
//
//        hTextBox * textPresetName = gui->addTextBox("textPresetName" + panelID, textPanel,  HGUI_NEXT_ROW, gui->margin2, gui->margin2, 246, "");
//        hLabel * textPresetNameLabel = gui->addLabel("", textPanel, HGUI_RIGHT, 2, 0, "new preset name");
//        textPresetName->clearAfterReturn(true);
//
//        events->addListener("addPreset_" + panelID, this, &GuiView::addPreset);
//        textPresetName->setMessage("addPreset_" + panelID);
//
//        hListBox * presetList = gui->addListBox("presetList" + panelID, textPanel, HGUI_NEXT_ROW, gui->margin2, gui->margin2, 120);
//        presetList->addItems(5, "");
//
//        events->addListener("selectPreset_" + panelID, this, &GuiView::selectPreset);
//
//        hListBox * textList = gui->addListBox("textList" + panelID, textPanel, HGUI_NEXT_COL, gui->margin2-100, 47, 120);
//        textList->addItems(5, "");
//
//        events->addListener("selectText_" + panelID, this, &GuiView::selectText);
//
//        hButton * recordButton = gui->addButton("record" + panelID, textPanel, HGUI_NEXT_COL, gui->margin2+140, 26, 80, "start record");
//        events->addListener("record_" + panelID, this, &GuiView::record);
//        recordButton->setMessage("record_" + panelID);
//
//        hButton * playButton = gui->addButton("play" + panelID, textPanel, HGUI_NEXT_COL, gui->margin2, 26, 80, "start play");
//        events->addListener("play_" + panelID, this, &GuiView::play);
//        playButton->setMessage("play_" + panelID);
//
//        hButton * clearButton = gui->addButton("clear" + panelID, textPanel, HGUI_NEXT_COL, gui->margin2, 26, 80, "clear");
//        events->addListener("clear_" + panelID, this, &GuiView::clear);
//        clearButton->setMessage("clear_" + panelID);
//
//        hSlider * textVelocity = gui->addSlider("textVelocity" + panelID, textPanel, HGUI_NEXT_ROW, 293, gui->margin2*3, 500);
//        hLabel * textVelocityLabel = gui->addLabel("", textPanel, HGUI_ABSOLUTE_POSITION, 0, 0, "");
//        textVelocity->setLinkedLabel(textVelocityLabel, true);
//        textVelocity->setRange(-250.0f, 250.0f);
//        textVelocity->setValue(0.0f);
//        textVelocity->setColor(0x333333);
//        //textVelocity->setFloatVar(&SOMETHING);
//        events->addListener("setTextVelocity_" + panelID, this, &GuiView::setTextVelocity);
//        textVelocity->setMessage("setTextVelocity_" + panelID);
//
//        hSlider * textPosition = gui->addSlider("textPosition" + panelID, textPanel, HGUI_NEXT_ROW, 293, gui->margin2, 500);
//        textPosition->setRange(0.0f, 1.0f);
//        textPosition->setColor(0x333333);
//        textPosition->setValue(0.0f);
//
//        textList->setAllElementsDisabled(true);
//        recordButton->setDisabled(true);
//        playButton->setDisabled(true);
//        clearButton->setDisabled(true);
//        textVelocity->setDisabled(true);
//        textPosition->setDisabled(true);
        //textPosition->setFloatVar(&SOMETHING);
    }

    // Alert Dialog

    int msgBoxX     = (ofGetWidth() / 3)  - (gui->checkBoxSize + 2) / 2;
	int msgBoxY     = (ofGetHeight() / 5) - (gui->checkBoxSize + 2) / 2;
	int msgBoxXSize =  ofGetWidth()  / 3;
	int msgBoxYSize =  ofGetWidth()  / 6;

    gui->setAlertFont("fonts/verdana.ttf", 14);

	hAlert * alertDialog = gui->addAlert("alertDialog", NULL, HGUI_ABSOLUTE_POSITION, msgBoxX-19, msgBoxY+18, msgBoxXSize, msgBoxYSize);

    // Gui Colors

    gui->setBackgroundColor	(0x333333);
	gui->setBorderColor		(0xCCCCCC);
	gui->setFillColor		(0x555555);
	gui->setAltFillColor	(0x557766);
	gui->setButtonShadowColor(0x999999);

	gui->setScrollHandleColor(0xDDDDDD);
	gui->setScrollButtonColor(0xDDDDDD);
	gui->setTextColor		(0xFFFFFF);
	gui->setTextColor2		(0x000000);
	gui->setAlertTextColor	(0xCCCCCC);

	gui->setDisableColor	(0x999999);
	gui->setEditTextColor	(0x113388);
	gui->setEditTextColor2	(0x113388);
	gui->setEditBackColor	(0xCCDDEE);
	gui->setCaretColor		(0x000000);

	gui->setLabelSelColor	(0xBBBBFF);
	gui->setItemSelColor	(0xBBBBBB);
	gui->setItemSelTextColor(0x333333);
	gui->setTabBoxSelColor	(0x4477CC);
	gui->setButtonBoxSelColor(0x44CC77);

	gui->setCheckBoxColor	(0x44CC77);
	gui->setSliderColor		(0x999999);
	gui->setTwoDSliderColor	(0x33BB66);
	gui->setCounterColor	(0x33BB66);

	gui->setDialogColor		(0xE5E5E5);
	gui->setMessageBoxColor	(0x77FFAA);
	gui->setAlertColor		(0xFF7777);

}

//--------------------------------------------------------------
void GuiView::update() {

    hGui * gui = hGui::getInstance();
    hEvents * events = hEvents::getInstance();

//    map< string, TextServer* >& servers = _appModel->getServers();
//    map< string, TextServer* >::iterator it;
//
//    int portIndex = 1;
//    for (it = servers.begin(); it != servers.end(); it++, portIndex++) {
//
//        TextServer* textServer = it->second;
//        string portNumber = ofToString(portIndex);
//        string panelID = ofToString(portIndex-1);
//        events->sendEvent("port" + portNumber + "CB.setSelected", textServer->getServer()->isConnected());
//        events->sendEvent("port" + portNumber + "TextLabel.setLabel", "port " + portNumber + ":" + ofToString(textServer->getServer()->getNumClients()));
//        hSlider * textPosition = (hSlider*)gui->getWidget("textPosition" + panelID);
//
//        hButton * recordButton = (hButton*)gui->getWidget("record" + panelID);
//        hButton * playButton = (hButton*)gui->getWidget("play" + panelID);
//        TextObject* preset = textServer->getPreset();
//        if (preset == NULL) continue;
//
//        if (preset->getIsPlaying()) {
//            textPosition->setValue(preset->getPosition());
//            playButton->setLabel("stop play");
//        } else {
//            playButton->setLabel("start play");
//        }
//
//        if (preset->getIsRecording()) {
//            recordButton->setLabel("stop record");
//        } else {
//            recordButton->setLabel("start record");
//        }
//    }

    begin();
    {
         if(boost::any_cast<bool>(_appModel->getProperty("showGui"))) gui->draw();
    }
    end();
}

void GuiView::updateVideoList() {
    LOG_VERBOSE("Update video list");

    ofDirectory& videoDirectory = _appModel->getVideoDirectory();
    int numVideos = videoDirectory.numFiles();

    if (numVideos == 0) numVideos = _appModel->listVideoFolder(boost::any_cast<string>(_appModel->getProperty("videoPath")));
    if (numVideos == 0) {

        LOG_WARNING("No videos in that folder!!");
        return;
    }

    hGui * gui = hGui::getInstance();

    hListBox * videoList = (hListBox*)gui->getWidget("videoList");
    videoList->clear();



    videoList->addItems(7, "");

    for (int i = 0; i < numVideos; i++) {
        if (i < 7) videoList->setElementLabel(i+1, videoDirectory.getName(i));
        if (i > 6) videoList->addData(videoDirectory.getName(i));
    }

    videoList->setMessage("selectVideo");

}

void GuiView::updateSceneList() {
    LOG_VERBOSE("Update scene list");

    hGui * gui = hGui::getInstance();
    hEvents * events = hEvents::getInstance();

    map<string, Scene*>& scenes = _appModel->getScenes();

    hListBox * sceneList = (hListBox*)gui->getWidget("sceneList");
    sceneList->clear();
    sceneList->addItems(6,"");

    if (scenes.size() > 0) {

        map<string, Scene*>::iterator it;
        int index = 0;
        for (it = scenes.begin(); it != scenes.end(); it++, index++) {
            if (index < 6) {
                sceneList->setElementLabel(index+1, it->first);
            } else {
                sceneList->addData(it->first);
            }
        }
    } else {
        LOG_VERBOSE("No scenes!!!");
    }

    sceneList->setMessage("selectScene");
    sceneList->displayAllIndexes(true);

    Scene* scene = _appModel->getCurrentScene();
    if (scene == NULL) return;

    string sceneName = scene->getName();
    sceneList->selectElement(sceneName);
    sceneList->scrollItems(sceneList->findElement(sceneName)->index-1);
    sceneList->setScrollBarPosition(sceneList->findElement(sceneName)->index);

}

void GuiView::updatePatternLayout() {

    Scene* scene = _appModel->getCurrentScene();
    if (scene == NULL) return;

    int type = _appModel->getPatternIndex(scene->getPatternName());

    if (type == -1) type = 0;

    hGui * gui = hGui::getInstance();
    hButtonBox * patternLayout = (hButtonBox*)gui->getWidget("patternLayout");
    hButtonBox * patternLayoutOFF = (hButtonBox*)gui->getWidget("patternLayoutOFF");
    hListBox * patternList = (hListBox*)gui->getWidget("patternList");
    patternList->setAllElementsDisabled(false);
    patternLayout->setAllElementsDisabled(false);
    patternLayoutOFF->setAllElementsDisabled(false);
    patternLayout->clear();

    patternList->selectElement(scene->getPatternName());

    switch (type) {
        case 0:
            // none
            break;
        case 1:
            // single
            patternLayout->addItems(1,1);
            break;
        case 2:
            //double
            patternLayout->addItems(2,1);
            break;
        case 3:
            //triple
            patternLayout->addItems(3,1);
            break;
        case 4:
            // quad
            patternLayout->addItems(2,2);
            break;
        case 5:
            // quad inset
            patternLayout->addItems(2,2);
            break;
        case 6:
            // sextet
            patternLayout->addItems(3,2);
            break;
    }

    patternLayout->setMessage("selectPattern");
    patternLayoutOFF->setMessage("selectPatternOFF");

}

//--------------------------------------------------------------
void GuiView::addScene(hEventArgs& args) {
    LOG_VERBOSE("Add new scene: " + args.strings[0]);

    hGui * gui = hGui::getInstance();
    hEvents * events = hEvents::getInstance();

    string newSceneName = args.strings[0];//boost::any_cast<string>(_appModel->getProperty("newSceneName"));

    if (newSceneName != "" && _appModel->getScene(newSceneName) == NULL) {
        LOG_VERBOSE("Begin editing: " + newSceneName);

        hListBox * patternList = (hListBox*)gui->getWidget("patternList");
        hListBox * patternLayoutOFF = (hListBox*)gui->getWidget("patternLayoutOFF");
        hButtonBox * patternLayout = (hButtonBox*)gui->getWidget("patternLayout");
        hListBox * videoList = (hListBox*)gui->getWidget("videoList");
        hListBox * cameraList = (hListBox*)gui->getWidget("cameraList");
        patternLayout->setAllElementsDisabled(true);
        patternLayoutOFF->setAllElementsDisabled(true);
        videoList->setAllElementsDisabled(true);
        cameraList->setAllElementsDisabled(true);
        patternList->setAllElementsDisabled(false);
        patternList->selectElement(1);

        Scene* scene = new Scene();
        scene->setName(newSceneName);
        _appModel->addScene(scene);
        _appModel->setCurrentScene(newSceneName);
        updateSceneList();

    } else {

        LOG_WARNING("Blank or non-unique scene name...");
        events->sendEvent("alertDialog.clear");
        events->sendEvent("alertDialog.display", "Enter a unique \nnew scene name!");
    }

}

//--------------------------------------------------------------
void GuiView::changePattern(hEventArgs& args) {
    LOG_VERBOSE("Change pattern layout");

    Scene* scene = _appModel->getCurrentScene();
    if (scene == NULL) {
        LOG_ERROR("Somehow there is no current scene!!!");
        return;
    }

    int oldtype = _appModel->getPatternIndex(scene->getPatternName());
    vector<ofRectangle*> pattern = _appModel->getPattern(args.strings[0]);

    if (oldtype != -1){

        vector<ofRectangle*> oldpattern = _appModel->getPattern(scene->getPatternName());

        if(oldpattern.size() > pattern.size()){
            // delete excess videos
            for (int i = pattern.size()-6; i < scene->getVideos().size()-6; i++) {
                delete scene->getVideos()[i];
                scene->getVideos()[i] = NULL;
            }
        }
    }

    scene->setPatternName(args.strings[0]);
    scene->getVideos().resize(pattern.size());

    for (int i = 0; i < pattern.size(); i++) {
        if(scene->getVideos()[i] == NULL) scene->getVideos()[i] = new VideoObject();
    }

    hGui * gui = hGui::getInstance();
    hListBox * videoList = (hListBox*)gui->getWidget("videoList");
    hListBox * cameraList = (hListBox*)gui->getWidget("cameraList");
    videoList->setAllElementsDisabled(true);
    cameraList->setAllElementsDisabled(true);
    videoList->unselectLastRadioElement();
    cameraList->unselectLastRadioElement();
    bOffScreen = false;
    bDegraded = false;
    fVolume = 1.0f;
    fPan = 0.0f;
    updatePatternLayout();

}

//--------------------------------------------------------------
void GuiView::selectPattern(hEventArgs& args) {
    LOG_VERBOSE("Select pattern: " + args.eventName);

    hGui * gui = hGui::getInstance();

    Scene* scene = _appModel->getCurrentScene();
    if (scene == NULL) {
        LOG_ERROR("Somehow there is no current scene!!!");
        return;
    }

    hListBox * videoList = (hListBox*)gui->getWidget("videoList");
    hListBox * cameraList = (hListBox*)gui->getWidget("cameraList");
//    hCheckBox * offscreenCB = (hCheckBox*)gui->getWidget("offscreenCB");
//    hCheckBox * degradedCB = (hCheckBox*)gui->getWidget("degradedCB");
//    hSlider * volumeSlider = (hSlider*)gui->getWidget("volumeSlider");
//    hSlider * panSlider = (hSlider*)gui->getWidget("panSlider");

    videoList->setAllElementsDisabled(false);
    cameraList->setAllElementsDisabled(false);

    int position;
    if (args.eventName == "selectPattern") position = args.values[0]-1;
    if (args.eventName == "selectPatternOFF") position = _appModel->getPattern(scene->getPatternName()).size() - args.values[0];
    scene->setLayoutPosition(position);

    string videoName = scene->getVideos()[position]->_videoName;

    if (videoName != "") {
        LOG_VERBOSE("Video/Camera path is already selected so display it...");

        if(videoList->findElement(videoName) == NULL && cameraList->findElement(videoName) == NULL){
            LOG_ERROR("Video/Camera path exists but asset can't be found");
            videoList->unselectLastRadioElement();
            cameraList->unselectLastRadioElement();
            bOffScreen = false;
            bDegraded = false;
            fVolume = 1.0f;
            fPan = 0.0f;
            return;
        }

        bOffScreen = scene->getVideos()[position]->_bOffscreen;
        bDegraded = scene->getVideos()[position]->_bDegraded;
        fVolume = scene->getVideos()[position]->_fVolume;
        fPan = scene->getVideos()[position]->_fPan;

        if (scene->getVideos()[position]->_inputType == GO_VIDEO_CAMERA) {
            videoList->unselectLastRadioElement();
            cameraList->selectElement(videoName);
        } else {
            cameraList->unselectLastRadioElement();
            videoList->selectElement(videoName);
            videoList->scrollItems(videoList->findElement(videoName)->index-1);
            videoList->setScrollBarPosition(videoList->findElement(videoName)->index);
        }
    } else {
        LOG_VERBOSE("No Video/Camera path selected");
        videoList->unselectLastRadioElement();
        cameraList->unselectLastRadioElement();
        bOffScreen = false;
        bDegraded = false;
        fVolume = 1.0f;
        fPan = 0.0f;
    }

}

//--------------------------------------------------------------
void GuiView::selectCamera(hEventArgs& args) {
    LOG_VERBOSE("Select camera");

    hGui * gui = hGui::getInstance();

    Scene* scene = _appModel->getCurrentScene();
    if (scene == NULL) {
        LOG_ERROR("Somehow there is no current scene!!!");
        return;
    }

    hListBox * videoList = (hListBox*)gui->getWidget("videoList");
    videoList->unselectLastRadioElement();

    scene->getVideos()[scene->getLayoutPosition()]->_videoPath = args.strings[0];
    scene->getVideos()[scene->getLayoutPosition()]->_videoName = args.strings[0];
    scene->getVideos()[scene->getLayoutPosition()]->_inputType = GO_VIDEO_CAMERA;
    scene->getVideos()[scene->getLayoutPosition()]->_bOffscreen = bOffScreen = false;
    scene->getVideos()[scene->getLayoutPosition()]->_bDegraded = bDegraded = false;
    scene->getVideos()[scene->getLayoutPosition()]->_fVolume = fVolume = 1.0f;
    scene->getVideos()[scene->getLayoutPosition()]->_fPan = fPan = 0.0f;
    scene->getVideos()[scene->getLayoutPosition()]->_bAssigned = false;

}

//--------------------------------------------------------------
void GuiView::setOffScreen(hEventArgs& args) {
    LOG_VERBOSE("Set offscreen video: " + ofToString(bOffScreen));
    Scene* scene = _appModel->getCurrentScene();
    if (scene == NULL) return;
    scene->getVideos()[scene->getLayoutPosition()]->_bOffscreen = bOffScreen;
}

//--------------------------------------------------------------
void GuiView::setDegraded(hEventArgs& args) {
    LOG_VERBOSE("Set degraded video: " + ofToString(bDegraded));
    Scene* scene = _appModel->getCurrentScene();
    if (scene == NULL) return;
    scene->getVideos()[scene->getLayoutPosition()]->_bDegraded = bDegraded;
}

//--------------------------------------------------------------
void GuiView::setVolume(hEventArgs& args) {
    LOG_VERBOSE("Set volume of video: " + ofToString(fVolume));
    Scene* scene = _appModel->getCurrentScene();
    if (scene == NULL) return;
    scene->getVideos()[scene->getLayoutPosition()]->_fVolume = fVolume;
}

//--------------------------------------------------------------
void GuiView::setPan(hEventArgs& args) {
    LOG_VERBOSE("Set pan of video: " + ofToString(fPan));
    Scene* scene = _appModel->getCurrentScene();
    if (scene == NULL) return;
    scene->getVideos()[scene->getLayoutPosition()]->_fPan = fPan;
}

//--------------------------------------------------------------
void GuiView::selectVideo(hEventArgs& args) {
    LOG_VERBOSE("Select video");

    hGui * gui = hGui::getInstance();

    Scene* scene = _appModel->getCurrentScene();
    if (scene == NULL) {
        LOG_ERROR("Somehow there is no current scene!!!");
        return;
    }

    hListBox * cameraList = (hListBox*)gui->getWidget("cameraList");
    cameraList->unselectLastRadioElement();

    cout << scene->getVideos()[scene->getLayoutPosition()]->_fVolume << " " << scene->getVideos()[scene->getLayoutPosition()]->_fPan << " " << scene->getVideos()[scene->getLayoutPosition()]->_bDegraded << " " << scene->getVideos()[scene->getLayoutPosition()]->_bOffscreen << endl;

    scene->getVideos()[scene->getLayoutPosition()]->_videoPath = _appModel->getVideoDirectory().getPath(args.values[0]-1);
    scene->getVideos()[scene->getLayoutPosition()]->_videoName = args.strings[0];
    scene->getVideos()[scene->getLayoutPosition()]->_inputType = GO_VIDEO_PLAYER;
    scene->getVideos()[scene->getLayoutPosition()]->_bOffscreen = bOffScreen = false;
    scene->getVideos()[scene->getLayoutPosition()]->_bDegraded = bDegraded = false;
    scene->getVideos()[scene->getLayoutPosition()]->_fVolume = fVolume = 1.0f;
    scene->getVideos()[scene->getLayoutPosition()]->_fPan = fPan = 0.0f;
    scene->getVideos()[scene->getLayoutPosition()]->_bAssigned = false;

    cout << fVolume << " " << fPan << " " << bDegraded << " " << bOffScreen << endl;

}

//--------------------------------------------------------------
void GuiView::updateVideoPath(hEventArgs& args) {
    LOG_VERBOSE("Update video path: " + args.strings[0]);
    _appModel->setProperty("videoPath", args.strings[0]);
    updateVideoList();
}

//--------------------------------------------------------------
void GuiView::updatePorts(hEventArgs& args) {
    LOG_VERBOSE("Update ports: " + args.eventName + " = " + args.strings[0]);
    _appModel->setProperty(args.eventName, args.strings[0]);
}

//--------------------------------------------------------------
void GuiView::save(hEventArgs& args) {
    LOG_VERBOSE("Save button");
    _appModel->saveScenes("scenes/scenes.bin");
}

//--------------------------------------------------------------
void GuiView::del(hEventArgs& args) {
    LOG_VERBOSE("Del button");
    Scene * scene = _appModel->getCurrentScene();
    _appModel->delScene(scene);
    updateSceneList();
}

//--------------------------------------------------------------
void GuiView::delAll(hEventArgs& args) {
    LOG_VERBOSE("Del all button");
    _appModel->delAllScenes();
    updateSceneList();
}

//--------------------------------------------------------------
void GuiView::rewind(hEventArgs& args) {
    LOG_VERBOSE("Rewind current scene");
    Scene* scene = _appModel->getCurrentScene();
    if (scene == NULL) return;

    _appModel->rewindScene(scene);
}

//--------------------------------------------------------------
void GuiView::selectScene(hEventArgs& args) {
    LOG_VERBOSE("Select scene");

    if (_appModel->setCurrentScene(args.strings[0])) {
        hGui * gui = hGui::getInstance();
        hListBox * videoList = (hListBox*)gui->getWidget("videoList");
        hListBox * cameraList = (hListBox*)gui->getWidget("cameraList");
        videoList->setAllElementsDisabled(true);
        cameraList->setAllElementsDisabled(true);
        videoList->unselectLastRadioElement();
        cameraList->unselectLastRadioElement();
        bOffScreen = false;
        bDegraded = false;
        fVolume = 1.0f;
        fPan = 0.0f;
        updatePatternLayout();
    }
}

//--------------------------------------------------------------
void GuiView::keyPressed(int key) {
    hGui * gui = hGui::getInstance();
    gui->keyPressed(key);
}

//--------------------------------------------------------------
void GuiView::mouseDragged(int x, int y, int button) {
    //LOG_VERBOSE("mouseDragged");
    hGui * gui = hGui::getInstance();
    gui->mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void GuiView::mousePressed(int x, int y, int button) {
    //LOG_VERBOSE("mousePressed");
    hGui * gui = hGui::getInstance();
    gui->mousePressed(x, y, button);
}

//--------------------------------------------------------------
void GuiView::mouseReleased(int x, int y, int button) {
    //LOG_VERBOSE("mouseReleased");
    hGui * gui = hGui::getInstance();
    gui->mouseReleased(x, y, button);
}
