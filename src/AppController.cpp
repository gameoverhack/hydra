/*
 *  AppController.cpp
 *  emptyExample
 *
 *  Created by gameover on 22/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#include "AppController.h"

//--------------------------------------------------------------
AppController::AppController() {

	LOG_NOTICE("Constructing AppController");

    ofSetLogLevel(OF_LOG_VERBOSE);
    //ofSetVerticalSync(true);

	registerStates();

	_dataController = new DataController();
	_dataController->loadProperties("config.xml");

	_appModel->setProperty("showGui", true);

//	_appModel->setProperty("showProps", true);
	_appModel->setProperty("showDebug", true);
//	_appModel->setProperty("showFullscreen", false);
//	_appModel->setProperty("grabberWidth", 720);
//	_appModel->setProperty("grabberHeight", 480);
    _appModel->setProperty("videoPath", (string)"D:/HYDRA_VIDEO_FINALS");
    _appModel->setProperty("textPath", (string)"/texts");

	_mouseController = new MouseController();
	_keyboardController = new KeyboardController();
	_midiController = new MidiController();
	_oscController = new OscController();
	_videoController = new VideoController();
	_IOSController = new IOSController();

    _appModel->loadScenes("scenes/scenes.bin");
    _appModel->listVideoFolder(boost::any_cast<string>(_appModel->getProperty("videoPath")));

    ofxDisplayList displays = ofxDisplayManager::get()->getDisplays();
    //(displays.size() > 0 ? displays[1]->x : 0)
    _guiView = new GuiView(0, 0, 900, 1024, ofxFensterManager::get()->getPrimaryWindow(), "hydra");
	_appView = new AppView(0, 0, 1920.0f, 1080.0f, NULL, "output");
    _debugView = new DebugView(1024, 0, 900, 1024, NULL, "debug");

    _guiView->setup();


	//_appModel->setScene(new Scene("test_scene", 2, 0, 1024.0f, 768.0f));
	//_appModel->setScene(new Scene("test2_scene", 2, 0, 1024.0f, 768.0f));

//	_functionModel->registerFunction("GuiController::toggleShow",
//									 MakeDelegate(_guiController, &GuiController::toggleShow));
//
//	_functionModel->registerFunction("AppModel::adjustIntProperty",
//												MakeDelegate(_appModel, &AppModel::adjustIntProperty));
//
//	_functionModel->registerFunction("KinectController::setNearThreshold",
//												MakeDelegate(_kinectController, &KinectController::setNearThreshold));
//
//	_functionModel->registerFunction("KinectController::setFarThreshold",
//												MakeDelegate(_kinectController, &KinectController::setFarThreshold));
//
	_functionModel->registerFunction("AppModel::toggleBooleanProperty",
												MakeDelegate(_appModel, &AppModel::toggleBooleanProperty));
//
//	_functionModel->registerFunction("AppController::toggleFullscreen",
//												MakeDelegate(this, &AppController::toggleFullscreen));
//
//	_functionModel->registerFunction("KinectController::toggleRegisterKinectViewport",
//												MakeDelegate(_kinectController, &KinectController::toggleRegisterKinectViewport));
//
//	_functionModel->registerFunction("AppController::nextScene",
//								MakeDelegate(this, &AppController::nextScene));
//
//	_functionModel->registerFunction("AppController::previousScene",
//								MakeDelegate(this, &AppController::previousScene));
//
//	_functionModel->registerFunction("KinectController::toggleUseApproximation",
//								MakeDelegate(_kinectController, &KinectController::toggleUseApproximation));
//
//	_functionModel->registerFunction("KinectController::setCalibrationImage",
//								MakeDelegate(_kinectController, &KinectController::setCalibrationImage));
//
//	_functionModel->registerFunction("AppController::test",
//									 MakeDelegate(this, &AppController::test));
//
//	_functionModel->registerFunction("AppController::addScene",
//									 MakeDelegate(this, &AppController::addScene));
//
//	_functionModel->registerFunction("AppController::deleteScene",
//									 MakeDelegate(this, &AppController::deleteScene));
//
//	_functionModel->registerFunction("AppController::addKinectLayer",
//									 MakeDelegate(this, &AppController::addKinectLayer));
//
//	_functionModel->registerFunction("AppController::deleteKinectLayer",
//									 MakeDelegate(this, &AppController::deleteKinectLayer));
//
//	_functionModel->registerFunction("AppController::addVideoLayer",
//									 MakeDelegate(this, &AppController::addVideoLayer));
//
//	_functionModel->registerFunction("AppController::deleteVideoLayer",
//									 MakeDelegate(this, &AppController::deleteVideoLayer));
//
//	_functionModel->registerFunction("AppController::gotoSceneName",
//									 MakeDelegate(this, &AppController::gotoSceneName));
//
//    _functionModel->registerFunction("AppController::resetCurrentViewport",
//									 MakeDelegate(this, &AppController::resetCurrentViewport));
//
//    _functionModel->registerFunction("AppController::copyCurrentViewport",
//									 MakeDelegate(this, &AppController::copyCurrentViewport));
//
//    _functionModel->registerFunction("AppController::pasteCurrentViewport",
//									 MakeDelegate(this, &AppController::pasteCurrentViewport));
//
//
//	_functionModel->registerFunction("AppController::setKinectBackground",
//									 MakeDelegate(this, &AppController::setKinectBackground));
//
//	ofSetFullscreen(boost::any_cast<bool>(_appModel->getProperty("showFullscreen")));

//    _keyModel->registerEvent('x', kKEY_DOWN, "reset current viewport", "AppController::resetCurrentViewport");
//    _keyModel->registerEvent('c', kKEY_DOWN, "copy current viewport", "AppController::copyCurrentViewport");
//    _keyModel->registerEvent('v', kKEY_DOWN, "paste current viewport", "AppController::pasteCurrentViewport");
//
	_keyModel->registerEvent('f', kKEY_DOWN, "toggle fullscreen/window", "AppView::toggleFullscreen");
	_keyModel->registerEvent('g', kKEY_DOWN, "toggle show/hide gui", "AppModel::toggleBooleanProperty", (string)"showGui");
	_keyModel->registerEvent('c', kKEY_DOWN, "change to random pattern", "AppView::changePattern");
	_keyModel->registerEvent('p', kKEY_DOWN, "show all properties in debug view", "AppModel::toggleBooleanProperty", (string)"showProps");
	_keyModel->registerEvent('d', kKEY_DOWN, "show/hide debug view", "AppModel::toggleBooleanProperty", (string)"showDebug");
	//_keyModel->registerEvent('s', kKEY_DOWN, "toggle dual screen draw", "AppModel::toggleBooleanProperty", (string)"showDualScreen");
	//_keyModel->registerEvent('y', kKEY_DOWN, "increment blurIterations", "AppModel::adjustIntProperty", (string)"blurIterations", 1);
//	//_keyModel->registerEvent('h', kKEY_DOWN, "decrement blurIterations", "AppModel::adjustIntProperty", (string)"blurIterations", -1);
//	_keyModel->registerEvent('i', kKEY_DOWN, "increment current kinectLayer nearThreshold", "KinectController::setNearThreshold", 10, true);
//	_keyModel->registerEvent('k', kKEY_DOWN, "decrement current kinectLayer nearThreshold", "KinectController::setNearThreshold", -10, true);
//	_keyModel->registerEvent('o', kKEY_DOWN, "increment current kinectLayer farThreshold", "KinectController::setFarThreshold", 10, true);
//	_keyModel->registerEvent('l', kKEY_DOWN, "decrement current kinectLayer farThreshold", "KinectController::setFarThreshold", -10, true);
	//_keyModel->registerEvent('t', kKEY_DOWN, "toggle use approximation for contours", "KinectController::toggleUseApproximation");
	//_keyModel->registerEvent('s', kKEY_DOWN, "toggle hide/show settings", "GuiController::toggleShow");

//	_keyModel->registerEvent('.', kKEY_DOWN, "next scene", "AppController::nextScene");
//	_keyModel->registerEvent(',', kKEY_DOWN, "previous scene", "AppController::previousScene");
//
//	_keyModel->registerEvent('1', kKEY_DOWN, "jump to scene0", "AppController::gotoSceneName", (string)"scene0");
//	_keyModel->registerEvent('2', kKEY_DOWN, "jump to scene1", "AppController::gotoSceneName", (string)"scene1");
//	_keyModel->registerEvent('3', kKEY_DOWN, "jump to scene2", "AppController::gotoSceneName", (string)"scene2");
//	_keyModel->registerEvent('4', kKEY_DOWN, "jump to scene3", "AppController::gotoSceneName", (string)"scene3");
//	_keyModel->registerEvent('5', kKEY_DOWN, "jump to scene4", "AppController::gotoSceneName", (string)"scene4");
//	_keyModel->registerEvent('6', kKEY_DOWN, "jump to scene5", "AppController::gotoSceneName", (string)"scene5");
//
//	_midiModel->registerEvent(2, 144, 0, 0, kMIDI_ANY, "full screen", "AppView::toggleFullscreen");
//	_midiModel->registerEvent(1, 144, 37, 0, kMIDI_ANY, "jump to scene1", "AppController::gotoSceneName", (string)"scene1");
//	_midiModel->registerEvent(1, 144, 38, 0, kMIDI_ANY, "jump to scene2", "AppController::gotoSceneName", (string)"scene2");
//	_midiModel->registerEvent(1, 144, 39, 0, kMIDI_ANY, "jump to scene3", "AppController::gotoSceneName", (string)"scene3");
//	_midiModel->registerEvent(1, 144, 40, 0, kMIDI_ANY, "jump to scene4", "AppController::gotoSceneName", (string)"scene4");
//	_midiModel->registerEvent(1, 144, 41, 0, kMIDI_ANY, "jump to scene5", "AppController::gotoSceneName", (string)"scene5");
//
//	_oscModel->registerEvent("/1/volume1", 0.0f, kOSC_PASS_PARAM_ONE, "slider test", "AppController::test", (string)"/1/volume1");
//	_oscModel->registerEvent("/1/volume2", 0.0f, kOSC_PASS_PARAM_ONE, "slider test", "AppController::test", (string)"/1/volume2");

//	_midiModel->registerEvent(1, 144, 37, 0, kMIDI_PASS_BYTE_ONE, "test1", "AppController::test1");
//	_midiModel->registerEvent(1, 144, 38, 0, kMIDI_PASS_BYTE_TWO, "test2", "AppController::test2", (string)"byteTwo");
//	_midiModel->registerEvent(1, 208, 0, 0, kMIDI_PASS_BYTE_BOTH, "test3", "AppController::test3");


	//_appModel->getCurrentScene()->addVideoLayer();
	//->getCurrentScene()->getVideoLayer(0)->loadMovie("/Users/gameover/Desktop/FolioBig/VimeoReady/MoralesVacircaGingoldEDIT.mp4");

	setState(kAPPCONTROLLER_RUNNING);

}

//--------------------------------------------------------------
AppController::~AppController() {


}

void AppController::exit() {
    LOG_NOTICE("Destroying AppController");

	_dataController->saveProperties("config.xml");
	_appModel->saveScenes("scenes/scenes.bin");

	delete _appView;
	delete _dataController;
	delete _keyboardController;
	delete _midiController;
	delete _videoController;
	delete _IOSController;
	delete _appModel;

}

// state registration
//--------------------------------------------------------------
void AppController::registerStates() {

	LOG_VERBOSE("Registering States");

	registerState(kAPPCONTROLLER_INITING, "kAPPCONTROLLER_INITING");
	registerState(kAPPCONTROLLER_LOADING, "kAPPCONTROLLER_LOADING");
	registerState(kAPPCONTROLLER_CALIBRATE, "kAPPCONTROLLER_CALIBRATE");
	registerState(kAPPCONTROLLER_RUNNING, "kAPPCONTROLLER_RUNNING");
	registerState(kAPPCONTROLLER_EXITING, "kAPPCONTROLLER_EXITING");

	_appModel->registerStatefulClass(typeid(*this).name());

	setState(kAPPCONTROLLER_INITING);

}

void AppController::update() {
    _midiController->update();
    _oscController->update();
    _videoController->update();
    _IOSController->update();
}


void AppController::draw() {

}
