/*
 *  GuiView.cpp
 *  emptyExample
 *
 *  Created by gameover on 22/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#include "GuiView.h"
#include "glut.h"

//--------------------------------------------------------------
#ifdef FENSTER
GuiView::GuiView(float x, float y, float width, float height, ofxFenster* window, string windowTitle) : BaseView(x, y, width, height, window, windowTitle) {
#else
GuiView::GuiView(float x, float y, float width, float height) : BaseView(x, y, width, height) {
    ofRegisterKeyEvents(this);
    ofRegisterMouseEvents(this);
    controlWidth = boost::any_cast<float>(_appModel->getProperty("controlWidth"));
#endif
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

    //_functionModel->registerFunction("GuiView::oscPlayScene", MakeDelegate(this, &GuiView::oscPlayScene));
    _functionModel->registerFunction("GuiView::oscIpadControl", MakeDelegate(this, &GuiView::oscIpadControl));
    _functionModel->registerFunction("GuiView::oscPrompterControl", MakeDelegate(this, &GuiView::oscPrompterControl));
    _functionModel->registerFunction("GuiView::oscKinectControl", MakeDelegate(this, &GuiView::oscKinectControl));
// change to all prompter addresses
    //_oscModel->registerEvent("/play", (string)"/play", kOSC_PASS_PARAM_ONE, "osc play scene", "GuiView::oscPlayScene");
    _oscModel->registerEvent("/ipad", (string)"/ipad", (string)"/ipad", kOSC_PASS_PARAM_BOTH, "osc ipad select", "GuiView::oscIpadControl");
    _oscModel->registerEvent("/prompter", (string)"/prompter", (string)"/prompter", kOSC_PASS_PARAM_BOTH, "osc show control", "GuiView::oscPrompterControl");
    _oscModel->registerEvent("/kinect", (string)"/kinect", (string)"/kinect", kOSC_PASS_PARAM_BOTH, "osc kinect control", "GuiView::oscKinectControl");

    //assert(false);
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

//--------------------------------------------------------------
void GuiView::oscKinectControl(string command, string value){
    LOG_VERBOSE("Kinect Control OSC TRIGGER: " + command + " " + value);

    if(command == "minDepth" && value != ""){
        cout << "Kinect OSC minDepth: " << value << endl;
        _appModel->sendAllKinectOsc("/app/minDepth", value);
    }
    if(command == "maxDepth" && value != ""){
        cout << "Kinect OSC maxDepth: " << value << endl;
        _appModel->sendAllKinectOsc("/app/maxDepth", value);
    }
    if(command == "blur" && value != ""){
        cout << "Kinect OSC blur: " << value << endl;
        _appModel->sendAllKinectOsc("/app/blur", value);
    }
    if(command == "smoothing" && value != ""){
        cout << "Kinect OSC smoothing: " << value << endl;
        _appModel->sendAllKinectOsc("/app/smoothing", value);
    }
    if(command == "threshold" && value != ""){
        cout << "Kinect OSC threshold: " << value << endl;
        _appModel->sendAllKinectOsc("/app/threshold", value);
    }
    if(command == "contour" && value != ""){
        cout << "Kinect OSC contour: " << value << endl;
        _appModel->sendAllKinectOsc("/app/contour", value);
    }
    if(command == "fade" && value != ""){
        cout << "Kinect OSC fade: " << value << endl;
        _appModel->sendAllKinectOsc("/app/fade", value);
    }
    if(command == "white"){
        cout << "Kinect OSC white: " << value << endl;
        _appModel->sendAllKinectOsc("/app/white", value);
    }
    if(command == "image"){
        cout << "Kinect OSC image: " << value << endl;
        _appModel->sendAllKinectOsc("/app/image", value);
    }
    if(command == "snapshot"){
        cout << "Kinect OSC snapshot: " << value << endl;
        _appModel->sendAllKinectOsc("/app/snapshot", value);
    }
    if(command == "play" && value != ""){
        cout << "Kinect OSC Play: " << value << endl;
        hEventArgs args;
        args.eventName = "selectKinectVideo";
        args.strings.push_back(value);
        selectKinectVideo(args);
    }
    if(command == "stop"){
        cout << "Kinect OSC Stop: " << value << endl;
        _appModel->sendAllKinectOsc("/app/stop", value);
    }
}

//--------------------------------------------------------------
void GuiView::oscPrompterControl(string command, string value){
    LOG_VERBOSE("Show Prompter OSC TRIGGER: " + command + " " + value);

    // rewind scene commands
    if(command == "rewind" && value == ""){
        cout << "Prompter OSC Rewind: " << value << endl;
        Scene* scene = _appModel->getCurrentScene();
        if (scene == NULL) return;
        _appModel->rewindScene(scene);
    }

    if(command == "rewind" && value != "all" && value != ""){
        cout << "Prompter OSC Rewind: " << value << endl;
        Scene* scene = _appModel->getScene(value);
        if (scene == NULL) return;
        _appModel->rewindScene(scene);
    }

    if(command == "rewind" && value == "all"){
        cout << "Prompter OSC Rewind: " << value << endl;
        _appModel->rewindAll();
    }

    // play scene command
    if(command == "play"){
        cout << "Prompter OSC Play: " << value << endl;
        hEventArgs args;
        args.eventName = "selectScene";
        Scene* scene = _appModel->getScene(value);
        if (scene == NULL) return;
        args.strings.push_back(scene->getName());
        selectScene(args);
        string sceneName = scene->getName();
        hGui * gui = hGui::getInstance();
        hListBox * sceneList = (hListBox*)gui->getWidget("sceneList");
        sceneList->selectElement(sceneName);
        sceneList->setScrollBarPosition(sceneList->findElement(sceneName)->index - 1);
    }
}

//--------------------------------------------------------------
void GuiView::oscIpadControl(string ipadID, string value){

    LOG_VERBOSE("Ipad OSC TRIGGER: " + ipadID + " " + value);

    int iIpadID;
    if(ipadID == "A"){
        iIpadID = -100; // garbage
    }else{
       iIpadID = ofToInt(ipadID) - 1;
    }

    map<int, IOSVideoPlayer*>& iosVideoPlayers = _appModel->getIOSVideoPlayers();
    map<int, IOSVideoPlayer*>::iterator it = iosVideoPlayers.find(100 + iIpadID);


    if(value == "mirror:true"){
        if(it != iosVideoPlayers.end()){
            _appModel->sendIOSOsc("/app/mirror", 100 + iIpadID, "1");
        }else if(ipadID == "A"){
            _appModel->sendAllIOSOsc("/app/mirror", "1");
        }
        return;
    }

    if(value == "mirror:false"){
        if(it != iosVideoPlayers.end()){
            _appModel->sendIOSOsc("/app/mirror", 100 + iIpadID, "0");
        }else if(ipadID == "A"){
            _appModel->sendAllIOSOsc("/app/mirror", "0");
        }
        return;
    }

    if(value == "stop"){
        if(it != iosVideoPlayers.end()){
            _appModel->sendIOSOsc("/app/stop", 100 + iIpadID, "");
        }else if(ipadID == "A"){
            _appModel->sendAllIOSOsc("/app/stop", "");
        }
        return;
    }

    hEventArgs args;
    args.eventName = "selectIOSVideo_" + ofToString(iIpadID);
    args.strings.push_back(value);
    selectIOSVideo(args);
}

//--------------------------------------------------------------
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

    // create tabs for panels
    int mainTabBoxWidth = mainscenesPanel->getWidth();

	hTabBox *  mainTabBox = gui->addTabBox("mainTabBox", NULL, HGUI_ABSOLUTE_POSITION, gui->margin1, 5, mainTabBoxWidth);
	mainTabBox->addItems(3);

	mainTabBox->setItemLabel(1, "Hydra");
	mainTabBox->setItemPanel(1, mainscenesPanel);

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

    hButton * rewindButton = gui->addButton("rewind", scenesPanel, HGUI_ABSOLUTE_POSITION, 25 + 90 * 3, 240, 60, "rewind");
    events->addListener("rewind", this, &GuiView::rewind);
    rewindButton->setMessage("rewind");

    hButton * rewindAllButton = gui->addButton("rewindAll", scenesPanel, HGUI_ABSOLUTE_POSITION, 25 + 90 * 3 + 70, 240, 60, "rewind all");
    events->addListener("rewindAll", this, &GuiView::rewindAll);
    rewindAllButton->setMessage("rewindAll");

    // Kinect Control Panel

	hPanel * kinectPanel = gui->addPanel("kinect", mainscenesPanel, HGUI_RIGHT, gui->margin2, 0, halfWidth, 250, true);
	hLabel * kinectPanelLabel = gui->addLabel("", kinectPanel, HGUI_TOP_LEFT, 2, 0, "Kinect");

    hListBox * kinectVideoList = gui->addListBox("kinectVideoList", kinectPanel, HGUI_NEXT_ROW, gui->margin2+2, gui->margin2, 200);

    kinectVideoList->addItems(7,"");
    events->addListener("selectKinectVideo", this, &GuiView::selectKinectVideo);
    kinectVideoList->setMessage("selectKinectVideo");

    hButton * fadeUp = gui->addButton("fadeUp", kinectPanel, HGUI_NEXT_ROW, gui->margin2+2, gui->margin2 - 90, 80, "fadeUp");
    events->addListener("fadeUp", this, &GuiView::fadeKinectVideo);
    fadeUp->setMessage("fadeUp");

    hButton * fadeDown = gui->addButton("fadeDown", kinectPanel, HGUI_NEXT_COL, gui->margin2+2, 208, 80, "fadeDown");
    events->addListener("fadeDown", this, &GuiView::fadeKinectVideo);
    fadeDown->setMessage("fadeDown");

    hButton * stopButton = gui->addButton("stopKinectVideo", kinectPanel, HGUI_NEXT_COL, gui->margin2+2, 208, 80, "stopKinectVideo");
    events->addListener("stopKinectVideo", this, &GuiView::stopKinectVideo);
    stopButton->setMessage("stopKinectVideo");

    hCheckBox * heartBeat = gui->addCheckBox("kinectHeartBeat", kinectPanel, HGUI_NEXT_COL, gui->margin2+2, 208);

    hLabel * kinectInfo = gui->addLabel("kinectDeviceInfo", kinectPanel, HGUI_NEXT_ROW, gui->margin2+2, gui->margin2 - 68, "");

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

        string panelID = ofToString(i);

        int xx, yy;
        int currentHeight = 468;

        if(i == 0 || i == 1){
            xx = 3 + 2 * gui->margin2 + i * (gui->margin2 + halfWidth);
            yy = gui->margin2 + currentHeight;
        }else{
            xx = 3 + 2 * gui->margin2 + (i-2) * (gui->margin2 + halfWidth);
            yy = 2 * gui->margin2 + currentHeight + 250;
        }

        hPanel * serverPanel = gui->addPanel("server" + panelID, mainscenesPanel, HGUI_ABSOLUTE_POSITION, xx, yy, halfWidth, 250, true);
        hLabel * serverPanelLabel = gui->addLabel("", serverPanel, HGUI_TOP_LEFT, 2, 0, "iPad_" + ofToString(i+1));

        hListBox * iosVideoList = gui->addListBox("iosVideoList" + panelID, serverPanel, HGUI_NEXT_ROW, gui->margin2+2, gui->margin2, 200);

        iosVideoList->addItems(7,"");
        events->addListener("selectIOSVideo_" + panelID, this, &GuiView::selectIOSVideo);
        iosVideoList->setMessage("selectIOSVideo_" + panelID);

        hButton * stopButton = gui->addButton("stopIOSVideo" + panelID, serverPanel, HGUI_NEXT_ROW, gui->margin2+2, gui->margin2 - 90, 80, "stopIOSVideo");
        events->addListener("stopIOSVideo_" + panelID, this, &GuiView::stopIOSVideo);
        stopButton->setMessage("stopIOSVideo_" + panelID);

        hButton * syncButton = gui->addButton("syncIOSDevice" + panelID, serverPanel, HGUI_NEXT_COL, gui->margin2+2, 208, 80, "syncIOSDevice");
        events->addListener("syncIOSDevice_" + panelID, this, &GuiView::syncIOSDevice);
        syncButton->setMessage("syncIOSDevice_" + panelID);

        hButton * delButton = gui->addButton("delIOSDevice" + panelID, serverPanel, HGUI_NEXT_COL, gui->margin2+2, 208, 80, "delIOSDevice");
        events->addListener("delIOSDevice_" + panelID, this, &GuiView::delIOSDevice);
        delButton->setMessage("delIOSDevice_" + panelID);

        hCheckBox * heartBeat = gui->addCheckBox("iosHeartBeat" + panelID, serverPanel, HGUI_NEXT_COL, gui->margin2+2, 208);

        hLabel * labelInfo = gui->addLabel("iosDeviceInfo" + panelID, serverPanel, HGUI_NEXT_ROW, gui->margin2+2, gui->margin2 - 68, "");

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

    for(int i = 0; i < MAX_SERVERS; i++){
        updateIOSVideoList(i);
    }

    updateKinectVideoList();

    begin();
    {
         gui->draw();
    }
    end();
}

//--------------------------------------------------------------
void GuiView::updateIOSVideoList(int iosID){

    //LOG_VERBOSE("Update iosVideo list " + ofToString(iosID));

    map<int, IOSVideoPlayer*>& iosVideoPlayers = _appModel->getIOSVideoPlayers();
    map<int, IOSVideoPlayer*>::iterator it = iosVideoPlayers.find(100 + iosID);

    if(it != iosVideoPlayers.end()){

        hGui * gui = hGui::getInstance();
        hListBox * iosVideoList = (hListBox*)gui->getWidget("iosVideoList" + ofToString(iosID));

        IOSVideoPlayer* iosVideoPlayer = it->second;

        hCheckBox * heartBeat = (hCheckBox*)gui->getWidget("iosHeartBeat" + ofToString(iosID));
        if(ofGetElapsedTimeMillis() - iosVideoPlayer->lastHeartBeat < 500){
            heartBeat->setSelected(true);
        }else{
            heartBeat->setSelected(false);
        }

        if(iosVideoPlayer->bNeedsDisplayUpdate){

            LOG_VERBOSE("Updating iosVideo list " + ofToString(iosID));

            iosVideoList->clear();
            iosVideoList->addItems(7, "");

            for (int i = 0; i < iosVideoPlayer->files.size(); i++) {
                if (i < 7) iosVideoList->setElementLabel(i+1, iosVideoPlayer->files[i]);
                if (i > 6) iosVideoList->addData(iosVideoPlayer->files[i]);
            }

            iosVideoList->setMessage("selectIOSVideo_" + ofToString(iosID));
            iosVideoList->displayAllIndexes(true);
            iosVideoPlayer->bNeedsDisplayUpdate = false;
        }

        hLabel * labelInfo = (hLabel*)gui->getWidget("iosDeviceInfo" + ofToString(iosID));

        string msg = _appModel->getIOSAppStateAsString(iosVideoPlayer->iosAppState) + "  ";

        if(iosVideoPlayer->currentFile != ""){
            string elementName = iosVideoPlayer->currentFile;
            if(iosVideoPlayer->iosAppState == IOS_APP_PLAY){
                elementName = elementName.substr(1);
                //cout << "name*" << iosVideoPlayer->files.size() << " " << iosVideoList->getNumData() << " " << endl;
                if(iosVideoPlayer->files.size() > 0){
                    iosVideoList->selectElement(elementName);
                    iosVideoList->setScrollBarPosition(iosVideoList->findElement(elementName)->index - 1);
                }else{
                    //cout << "NULL" << endl;
                }
            }
            msg += elementName + " " + ofToString(iosVideoPlayer->currentFrame) + " / " + ofToString(iosVideoPlayer->totalFrames);
        }else{
            iosVideoList->unselectLastRadioElement();
        }
        labelInfo->setLabel(msg);
    }
}

//--------------------------------------------------------------
void GuiView::delIOSDevice(hEventArgs& args){

    if(glutGetModifiers() == GLUT_ACTIVE_ALT){
        LOG_VERBOSE("Delete iosVideo device " + args.eventName);
        vector<string> iosParts = ofSplitString(args.eventName, "_");
        int iosID = ofToInt(iosParts[1]);

        map<int, IOSVideoPlayer*>& iosVideoPlayers = _appModel->getIOSVideoPlayers();
        map<int, IOSVideoPlayer*>::iterator it = iosVideoPlayers.find(100 + iosID);

        if(it != iosVideoPlayers.end()){
            _appModel->sendIOSOsc("/app/delete", 100 + iosID);
        }

    }else{
        LOG_WARNING("Hold down the Alt/Option key to delete ALL movies on this ipad!");
    }

}

//--------------------------------------------------------------
void GuiView::syncIOSDevice(hEventArgs& args){

    LOG_VERBOSE("Sync iosVideo device " + args.eventName);

    vector<string> iosParts = ofSplitString(args.eventName, "_");
    int iosID = ofToInt(iosParts[1]);

    map<int, IOSVideoPlayer*>& iosVideoPlayers = _appModel->getIOSVideoPlayers();
    map<int, IOSVideoPlayer*>::iterator it = iosVideoPlayers.find(100 + iosID);

    if(it != iosVideoPlayers.end()){
        IOSVideoPlayer* iosVideoPlayer = it->second;
        _appModel->sendIOSOsc("/app/sync", 100 + iosID);
        hGui * gui = hGui::getInstance();
        hListBox * iosVideoList = (hListBox*)gui->getWidget("iosVideoList" + ofToString(iosID));
        iosVideoList->clear();
        //updateIOSVideoList(iosID);
    }

}

//--------------------------------------------------------------
void GuiView::selectIOSVideo(hEventArgs& args){
    LOG_VERBOSE("Select iosVideo " + args.eventName);

    vector<string> iosParts = ofSplitString(args.eventName, "_");
    int iosID = ofToInt(iosParts[1]);

    if(iosID == -100 && args.strings[0] != ""){
        _appModel->sendAllIOSOsc("/app/play", args.strings[0]);
        return;
    }

    map<int, IOSVideoPlayer*>& iosVideoPlayers = _appModel->getIOSVideoPlayers();
    map<int, IOSVideoPlayer*>::iterator it = iosVideoPlayers.find(100 + iosID);

    if(it != iosVideoPlayers.end() && args.strings[0] != ""){
        _appModel->sendIOSOsc("/app/play", 100 + iosID, args.strings[0]);
    }

}

//--------------------------------------------------------------
void GuiView::stopIOSVideo(hEventArgs& args){
    LOG_VERBOSE("Stop IOS Video: " + args.eventName);

    vector<string> iosParts = ofSplitString(args.eventName, "_");
    int iosID = ofToInt(iosParts[1]);

    map<int, IOSVideoPlayer*>& iosVideoPlayers = _appModel->getIOSVideoPlayers();
    map<int, IOSVideoPlayer*>::iterator it = iosVideoPlayers.find(100 + iosID);

    if(it != iosVideoPlayers.end()){
        _appModel->sendIOSOsc("/app/stop", 100 + iosID, "");
    }
}

//--------------------------------------------------------------
void GuiView::updateKinectVideoList(){
    KinectVideoPlayer& kinectVideoPlayer = _appModel->getKinectVideoPlayer();

    if(kinectVideoPlayer.isSetup){

        hGui * gui = hGui::getInstance();
        hListBox * kinectVideoList = (hListBox*)gui->getWidget("kinectVideoList");

        hCheckBox * heartBeat = (hCheckBox*)gui->getWidget("kinectHeartBeat");
        if(ofGetElapsedTimeMillis() - kinectVideoPlayer.lastHeartBeat < 500){
            heartBeat->setSelected(true);
        }else{
            heartBeat->setSelected(false);
        }

        if(kinectVideoPlayer.bNeedsDisplayUpdate){

            LOG_VERBOSE("Updating kinectVideo list");

            kinectVideoList->clear();
            kinectVideoList->addItems(7, "");

            for (int i = 0; i < kinectVideoPlayer.files.size(); i++) {
                if (i < 7) kinectVideoList->setElementLabel(i+1, kinectVideoPlayer.files[i]);
                if (i > 6) kinectVideoList->addData(kinectVideoPlayer.files[i]);
            }

            kinectVideoList->setMessage("selectKinectVideo");
            kinectVideoList->displayAllIndexes(true);
            kinectVideoPlayer.bNeedsDisplayUpdate = false;
        }

        hLabel * labelInfo = (hLabel*)gui->getWidget("kinectDeviceInfo");

        string msg = _appModel->getKinectAppStateAsString(kinectVideoPlayer.kinectAppState) + "  ";

        if(kinectVideoPlayer.currentFile != ""){
            if(kinectVideoPlayer.kinectAppState == KINECT_APP_PLAY){
                string elementName = kinectVideoPlayer.currentFile;
                if(kinectVideoPlayer.files.size() > 0){
                    kinectVideoList->selectElement(elementName);
                    kinectVideoList->setScrollBarPosition(kinectVideoList->findElement(elementName)->index - 1);
                }else{
                    //cout << "NULL" << endl;
                }
            }

            msg += kinectVideoPlayer.currentFile + " " + ofToString(kinectVideoPlayer.currentFrame) + " / " + ofToString(kinectVideoPlayer.totalFrames);
        }else{
            kinectVideoList->unselectLastRadioElement();
        }

        labelInfo->setLabel(msg);
    }

}
//--------------------------------------------------------------
void GuiView::fadeKinectVideo(hEventArgs& args){
    LOG_VERBOSE("Fade kinect video " + args.eventName);
    if(args.eventName == "fadeUp"){
        _appModel->sendAllKinectOsc("/app/fade", "1.0:3000");
    }
    if(args.eventName == "fadeDown"){
        _appModel->sendAllKinectOsc("/app/fade", "0.0:3000");
    }
}

//--------------------------------------------------------------
void GuiView::selectKinectVideo(hEventArgs& args){
    LOG_VERBOSE("Select kinectVideo " + args.eventName);
    _appModel->sendAllKinectOsc("/app/play", args.strings[0]);
}

//--------------------------------------------------------------
void GuiView::stopKinectVideo(hEventArgs& args){
    LOG_VERBOSE("Stop Kinect Video: " + args.eventName);
    _appModel->sendAllKinectOsc("/app/stop", "");
}

//--------------------------------------------------------------
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

    vector<ofRectangle*> oldPattern = _appModel->getPattern(scene->getPatternName());
    vector<ofRectangle*> pattern = _appModel->getPattern(args.strings[0]);
    vector<VideoObject*>& videos = scene->getVideos();

    if(pattern.size() < oldPattern.size() && pattern.size() > 0){

        vector<VideoObject*> oldVideos;
        oldVideos.resize(videos.size());
        for(int i = 0; i < videos.size(); i++){
            oldVideos[i] = videos[i];
        }

        videos.clear();
        videos.resize(pattern.size());

        for(int i = 0; i < 6; i++){
            videos[videos.size() - 1 - i] = oldVideos[oldVideos.size() - 1 - i];
        }

        for(int i = 0; i < videos.size() - 6; i++){
            if(oldVideos[i] != NULL){
                videos[i] = oldVideos[i];
            }else{
                videos[i] = new VideoObject();
            }
        }

    oldVideos.clear();

    }else{
        if(pattern.size() == 0){
            videos.clear();
        }else{
            videos.resize(pattern.size());
            for(int i = 0; i < videos.size(); i++){
                if(videos[i] == NULL) videos[i] = new VideoObject;
            }
        }
    }
    scene->setPatternName(args.strings[0]);

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
            videoList->setScrollBarPosition(videoList->findElement(videoName)->index - 1);
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
void GuiView::rewindAll(hEventArgs& args) {
    LOG_VERBOSE("Rewind ALL scenes");
    _appModel->rewindAll();
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
#ifdef FENSTER
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
#else
//--------------------------------------------------------------
void GuiView::keyPressed(ofKeyEventArgs & k) {
    //LOG_VERBOSE("keyPressed");
    hGui * gui = hGui::getInstance();
    gui->keyPressed(k.key);
}

//--------------------------------------------------------------
void GuiView::keyReleased(ofKeyEventArgs & k) {
    //LOG_VERBOSE("keyReleased");
}

//--------------------------------------------------------------
void GuiView::mouseMoved(ofMouseEventArgs & m) {
    //LOG_VERBOSE("mouseMoved");
    if(m.x > controlWidth){
        ofHideCursor();
    }else{
        ofShowCursor();
    }

}

//--------------------------------------------------------------
void GuiView::mouseDragged(ofMouseEventArgs & m) {
    //LOG_VERBOSE("mouseDragged");
    hGui * gui = hGui::getInstance();
    gui->mouseDragged(m.x, m.y, m.button);
}

//--------------------------------------------------------------
void GuiView::mousePressed(ofMouseEventArgs & m) {
    //LOG_VERBOSE("mousePressed");
    hGui * gui = hGui::getInstance();
    gui->mousePressed(m.x, m.y, m.button);
}

//--------------------------------------------------------------
void GuiView::mouseReleased(ofMouseEventArgs & m) {
    //LOG_VERBOSE("mouseReleased");
    hGui * gui = hGui::getInstance();
    gui->mouseReleased(m.x, m.y, m.button);
}
#endif
