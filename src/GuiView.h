/*
 *  GuiView.h
 *  emptyExample
 *
 *  Created by gameover on 22/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#ifndef _H_GUIVIEW
#define _H_GUIVIEW

#include "BaseView.h"
#include "BaseState.h"
#include "MidiModel.h"
#include "hGui_all.h"
#include "hObject.h"

enum {
	kGUIVIEW_INIT,
	kGUIVIEW_SCENE,
	kGUIVIEW_CALIBRATE,
	kGUIVIEW_LOAD
};

class GuiView : public BaseView, public BaseState, public hObject {

public:

#ifdef FENSTER
	GuiView(float x, float y, float width, float height, ofxFenster* window = NULL, string windowTitle = "Untitled");	//ctor
#else
    GuiView(float x, float y, float width, float height);
#endif
    ~GuiView();								//dtor

	void registerStates();

    void setup();
	void update();

#ifdef FENSTER
    void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
#else
    void keyPressed(ofKeyEventArgs & k);
    void keyReleased(ofKeyEventArgs & k){};
    void mouseMoved(ofMouseEventArgs & m){};
    void mouseDragged(ofMouseEventArgs & m);
    void mousePressed(ofMouseEventArgs & m);
    void mouseReleased(ofMouseEventArgs & m);
#endif

    void save(hEventArgs& args);
    void addScene(hEventArgs& args);
    void del(hEventArgs& args);
    void delAll(hEventArgs& args);
    void rewind(hEventArgs& args);

    void updateVideoPath(hEventArgs& args);
    void updateTextPath(hEventArgs& args);
    void updatePorts(hEventArgs& args);

    void changePattern(hEventArgs& args);
    void selectPattern(hEventArgs& args);
    void selectCamera(hEventArgs& args);
    void selectVideo(hEventArgs& args);
    void selectScene(hEventArgs& args);
    void selectText(hEventArgs& args);
    void setOffScreen(hEventArgs& args);
    void setDegraded(hEventArgs& args);
    void setVolume(hEventArgs& args);
    void setPan(hEventArgs& args);

    void updateVideoList();
    void updateSceneList();
    void updatePatternLayout();

    void autoMidiMap(int port, int byteTwo);

private:

    bool bFlip[3];
    bool bOffScreen;
    bool bDegraded;
    float fVolume;
    float fPan;

    void loadStringFonts();
    void parseText(string & text, int targetScreenWidth);

protected:



};

#endif
