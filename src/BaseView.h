/*
 *  BaseView.h
 *  Jungle
 *
 *  Created by gameover on 29/06/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#ifndef _H_BASEVIEW
#define _H_BASEVIEW

#include "ViewPort.h"
#include "Logger.h"

#include "ofxFensterManager.h"
#include "ofxOpenCv.h"

class BaseView : public ofxFensterListener {

public:

	BaseView(float x, float y, float width, float height, ofxFenster* window = NULL, string windowTitle = "Untitled");	//ctor
	virtual ~BaseView();						//dtor

	virtual void	update() = 0; // update view fbo to draw image

	virtual void	draw(); // draw out the view fbo
	virtual void	draw(float x, float y);
	virtual void	draw(float x, float y, float width, float height);

	void			begin();
	void			end();

	// Returns the views fbo
	ofFbo*			getViewFBO();
    ofTexture*      getViewFBOTexture();

    float			getWidth();
    float			getHeight();

	ViewPort*		getViewPort() {return _viewPort;};

	void			warp(int width, int height, ofPoint * corners);

    ofxFenster*     getWindowRef() {return _window;};

protected:

	ViewPort*		_viewPort;

	float			_viewWidth, _viewHeight;

	ofFbo			_viewFBO[2]; // final output FBO

	int             _activeTextureIndex;

	ofxFenster*     _window;

};

#endif
