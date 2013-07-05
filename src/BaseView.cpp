/*
 *  BaseView.cpp
 *  Jungle
 *
 *  Created by gameover on 29/06/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#include "BaseView.h"
#ifdef FENSTER
BaseView::BaseView(float x, float y, float width, float height, ofxFenster* window, string windowTitle) {
    LOG_NOTICE("Constructing BaseView");

    _viewX = 0;
	_viewY = 0;

    if (window != NULL) {
        LOG_NOTICE("Assigning already created window to this view");
        _window = window;
        _window->setWindowPosition(x, y); // doesn't appear to sork on windows
    } else {
        LOG_NOTICE("Creating window for this view");
        _window = ofxFensterManager::get()->createFenster(x,y,width,height);
    }

    _window->addListener(this);
    _window->setBackgroundColor(0,0,0);
    _window->setWindowTitle(windowTitle);
    _window->setBorder(false);
    _window->setActive();
#else
BaseView::BaseView(float x, float y, float width, float height){
    LOG_NOTICE("Constructing BaseView");
    _viewX = x;
	_viewY = y;

#endif

	// save parameters
	_viewWidth = width;
	_viewHeight = height;

	// allocate the fbo texture
	_viewFBO[0].allocate(_viewWidth, _viewHeight, GL_BGRA);

	// allocate the fbo texture
	_viewFBO[1].allocate(_viewWidth, _viewHeight, GL_BGRA);

	_activeTextureIndex = 0;

	_viewPort = NULL;

}

BaseView::~BaseView() {

	LOG_NOTICE("Destroying BaseView");
	delete _viewPort;

}

// Returns the views fbo
ofFbo* BaseView::getViewFBO(){
	return &_viewFBO[0];
}

ofTexture* BaseView::getViewFBOTexture() {
	return &_viewFBO[0].getTextureReference();
}

float BaseView::getX(){
    return _viewX;
}

float BaseView::getY(){
    return _viewY;
}

float BaseView::getWidth() {
    return _viewWidth;
}
float BaseView::getHeight() {
    return _viewHeight;
}

void BaseView::warp(int width, int height, ofPoint * corners) {

	GLfloat myMatrix[16];

	for(int i = 0; i < 16; i++){
		if(i % 5 != 0) myMatrix[i] = 0.0;
		else myMatrix[i] = 1.0;
	}

	CvPoint2D32f cvsrc[4];
	CvPoint2D32f cvdst[4];

	cvsrc[0].x = 0;
	cvsrc[0].y = 0;
	cvsrc[1].x = width;
	cvsrc[1].y = 0;
	cvsrc[2].x = width;;
	cvsrc[2].y = height;
	cvsrc[3].x = 0;
	cvsrc[3].y = height;

	for(int i = 0; i < 4; i++){
		cvdst[i].x = corners[i].x * (float)width;
		cvdst[i].y = corners[i].y * (float)height;
	}

	CvMat * translate = cvCreateMat(3,3,CV_32FC1);

	CvMat* src_mat = cvCreateMat( 4, 2, CV_32FC1 );
	CvMat* dst_mat = cvCreateMat( 4, 2, CV_32FC1 );

	cvSetData( src_mat, cvsrc, sizeof(CvPoint2D32f));
	cvSetData( dst_mat, cvdst, sizeof(CvPoint2D32f));

	cvFindHomography(src_mat, dst_mat, translate);

	float *matrix = translate->data.fl;

	myMatrix[0]		= matrix[0];
	myMatrix[4]		= matrix[1];
	myMatrix[12]	= matrix[2];

	myMatrix[1]		= matrix[3];
	myMatrix[5]		= matrix[4];
	myMatrix[13]	= matrix[5];

	myMatrix[3]		= matrix[6];
	myMatrix[7]		= matrix[7];
	myMatrix[15]	= matrix[8];

	glMultMatrixf(myMatrix);

	cvReleaseMat(&translate);
	cvReleaseMat(&src_mat);
	cvReleaseMat(&dst_mat);

}

void BaseView::begin() {
#ifdef FENSTER
    _window->setActive();
#endif
	_viewFBO[0].begin();
	//glPushMatrix();

	glClearColor(0.0, 0.0, 0.0, 0.0); // transparent clear colour
    glClear(GL_COLOR_BUFFER_BIT);
    //ofSetColor(255, 255, 255, 255);
//    if (_viewPort != NULL) {
//        glTranslatef(_viewPort->getX(), _viewPort->getY(), 0.0f);
//        warp(_viewPort->getWidth(), _viewPort->getHeight(), _viewPort->getCorners());
//    }

}

void BaseView::end() {

	//ofSetColor(255, 255, 255, 255);
	//glPopMatrix();
	_viewFBO[0].end();

}

void BaseView::draw() {
	glPushMatrix();
    _viewFBO[0].draw(0, 0);
	glPopMatrix();
}

void BaseView::draw(float x, float y) {
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	draw();
	glPopMatrix();
}

void BaseView::draw(float x, float y, float width, float height) {
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	glScalef(width/_viewWidth, height/_viewHeight, 0.0f);
	draw();
	glPopMatrix();
}
