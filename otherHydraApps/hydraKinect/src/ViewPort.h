/*
 *  ViewPort.h
 *  Bunyip
 *
 *  Created by gameover on 28/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#ifndef _H_VIEWPORT
#define _H_VIEWPORT

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxOSXKeyModifiers.h"

enum {
    kASPECT_FREE,
    kASPECT_MAINTAIN,
    kASPECT_LOCKXY
};

class ViewPort {
	
public:
	
	ViewPort(float width, float height) : _width(width), _height(height) {
        _corners = new ofPoint[4];reset(width, height);
        _path = "viewport.xml";
        _oldCorners = new ofPoint[4];
    };
	
	ViewPort(float x, float y, float width, float height, ofPoint * corners)
	: _x(x), _y(y), _width(width), _height(height), _corners(corners) {
        _path = "viewport.xml";
        _oldCorners = new ofPoint[4];
    };
	
	~ViewPort() {delete [] _corners;};
	
	void reset(float width, float height) {
		
		_x				= 0.0f;
		_y				= 0.0f;
		_width			= width;
		_height			= height;
		_corners[0].x	= 0.0f;
		_corners[0].y	= 0.0f;
		_corners[1].x	= 1.0f;
		_corners[1].y	= 0.0f;
		_corners[2].x	= 1.0f;
		_corners[2].y	= 1.0f;
		_corners[3].x	= 0.0f;
		_corners[3].y	= 1.0f;
        saveViewport();
		
	};
	
	void drawViewport() {
		glPushMatrix();
		
		ofNoFill();
		ofSetColor(0, 255, 0);
		ofRect(0, 0, _width, _height);
		ofSetColor(255, 255, 255, 255);
		
		ofNoFill();
		ofSetColor(255, 0, 0);
		ofBeginShape();
		for(int corner = 0; corner < 4; corner++){
			ofVertex(_x + _corners[corner].x * _width, 
					 _y + _corners[corner].y * _height);
		}
		ofEndShape(true);
		ofSetColor(255, 255, 255, 255);
		glPopMatrix();
	}
	
	void drawViewport(float x, float y) {
		
		_editX		= x;
		_editY		= y;
		_editScaleX = 1.0f;
		_editScaleY	= 1.0f;
		
		glPushMatrix();
		glTranslatef(_editX, _editY, 0.0f);
		drawViewport();
		glPopMatrix();
	}
	
	void drawViewport(float x, float y, float width, float height) {
		
		_editX		= x;
		_editY		= y;
		_editScaleX = width/_width;
		_editScaleY	= height/_height;
		
		glPushMatrix();
		glTranslatef(_editX, _editY, 0.0f);
		glScalef(_editScaleX, _editScaleY, 0.0f);
		drawViewport();
		glPopMatrix();
	}
	
    inline void warp(){
        warp(_width, _height);
    }
    
    inline void warp(int width, int height){
        
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
            cvdst[i].x = _corners[i].x * (float)width;
            cvdst[i].y = _corners[i].y * (float)height;
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
    
    void saveViewport(){
        settings.loadFile(ofToDataPath(_path));
        settings.removeTag("viewport");
        settings.addTag("viewport");
        settings.addAttribute("viewport", "x", getX(), 0);
        settings.addAttribute("viewport", "y", getY(), 0);
        settings.addAttribute("viewport", "w", getWidth(), 0);
        settings.addAttribute("viewport", "h", getHeight(), 0);
        settings.addAttribute("viewport", "c0x", getCorners()[0].x, 0);
        settings.addAttribute("viewport", "c0y", getCorners()[0].y, 0);
        settings.addAttribute("viewport", "c1x", getCorners()[1].x, 0);
        settings.addAttribute("viewport", "c1y", getCorners()[1].y, 0);
        settings.addAttribute("viewport", "c2x", getCorners()[2].x, 0);
        settings.addAttribute("viewport", "c2y", getCorners()[2].y, 0);
        settings.addAttribute("viewport", "c3x", getCorners()[3].x, 0);
        settings.addAttribute("viewport", "c3y", getCorners()[3].y, 0);
        settings.saveFile(ofToDataPath(_path));
    }
    
    void loadViewport(){
        settings.loadFile(ofToDataPath(_path));
        setX(settings.getAttribute("viewport", "x", 0.0f));
        setY(settings.getAttribute("viewport", "y", 0.0f));
        setWidth(settings.getAttribute("viewport", "w", 1024.0f));
        setHeight(settings.getAttribute("viewport", "h", 768.0f));
        getCorners()[0].x = settings.getAttribute("viewport", "c0x", 0.0f);
        getCorners()[0].y = settings.getAttribute("viewport", "c0y", 0.0f);
        getCorners()[1].x = settings.getAttribute("viewport", "c1x", 1.0f);
        getCorners()[1].y = settings.getAttribute("viewport", "c1y", 0.0f);
        getCorners()[2].x = settings.getAttribute("viewport", "c2x", 1.0f);
        getCorners()[2].y = settings.getAttribute("viewport", "c2y", 1.0f);
        getCorners()[3].x = settings.getAttribute("viewport", "c3x", 0.0f);
        getCorners()[3].y = settings.getAttribute("viewport", "c3y", 1.0f);
    }
    
    void mouseDragged(int x, int y, int button){

        ofMouseEventArgs e;
        e.x = x;
        e.y = y;
        e.button = button;
        
        if(_currentViewPortCorner != -1) {
            float scaleX = ((float)e.x - getEditX() - (getX()) * getEditScaleX())/(getWidth() * getEditScaleX());
            float scaleY = ((float)e.y - getEditY() - (getY()) * getEditScaleY())/(getHeight() * getEditScaleY());
            
            float deltaX;
            float deltaY;
            
            if (_aspectType != kASPECT_MAINTAIN) {
                deltaX = scaleX - getCorners()[_currentViewPortCorner].x;
                deltaY = scaleY - getCorners()[_currentViewPortCorner].y;
                getCorners()[_currentViewPortCorner].x = scaleX;
                getCorners()[_currentViewPortCorner].y = scaleY;
            } else {
                if(ABS(e.x - _mouseAspectX) > ABS(e.y - _mouseAspectY)){
                    deltaX = (e.x - _mouseOriginX)/(getWidth() * getEditScaleX());
                    deltaY = 0.0f;
                    _corners[0].y = _oldCorners[0].y;
                    _corners[1].y = _oldCorners[1].y;
                    _corners[2].y = _oldCorners[2].y;
                    _corners[3].y = _oldCorners[3].y;
                    getCorners()[_currentViewPortCorner].x += deltaX;
                }else{
                    deltaX = 0.0f;
                    deltaY = (e.y - _mouseOriginY)/(getHeight() * getEditScaleY());
                    _corners[0].x = _oldCorners[0].x;
                    _corners[1].x = _oldCorners[1].x;
                    _corners[2].x = _oldCorners[2].x;
                    _corners[3].x = _oldCorners[3].x;
                    getCorners()[_currentViewPortCorner].y += deltaY;
                }
                
            }
            
            if (_aspectType == kASPECT_LOCKXY || _aspectType == kASPECT_MAINTAIN) {
                if (_currentViewPortCorner == 0) {
                    getCorners()[1].x -= deltaX;
                    getCorners()[1].y += deltaY;
                    getCorners()[2].x -= deltaX;
                    getCorners()[2].y -= deltaY;
                    getCorners()[3].x += deltaX;
                    getCorners()[3].y -= deltaY;
                }
                if (_currentViewPortCorner == 1) {
                    getCorners()[0].x -= deltaX;
                    getCorners()[0].y += deltaY;
                    getCorners()[2].x += deltaX;
                    getCorners()[2].y -= deltaY;
                    getCorners()[3].x -= deltaX;
                    getCorners()[3].y -= deltaY;
                }
                if (_currentViewPortCorner == 2) {
                    getCorners()[0].x -= deltaX;
                    getCorners()[0].y -= deltaY;
                    getCorners()[1].x += deltaX;
                    getCorners()[1].y -= deltaY;
                    getCorners()[3].x -= deltaX;
                    getCorners()[3].y += deltaY;
                }
                if (_currentViewPortCorner == 3) {
                    getCorners()[0].x += deltaX;
                    getCorners()[0].y -= deltaY;
                    getCorners()[1].x -= deltaX;
                    getCorners()[1].y -= deltaY;
                    getCorners()[2].x -= deltaX;
                    getCorners()[2].y += deltaY;
                }
            }
            _mouseOriginX = e.x;
            _mouseOriginY = e.y;
        }
        
        if (_currentViewPortCorner == -1) {
            if ((float)e.x > getEditX() + (getX()) * getEditScaleX() &&
                (float)e.x < getEditX() + (getX() + getWidth()) * getEditScaleX() &&
                (float)e.y > getEditY() + (getY()) * getEditScaleY() &&
                (float)e.y < getEditY() + (getY() + getHeight()) * getEditScaleY()) {
                setX(getX() + (e.x - _mouseOriginX)/getEditScaleX());
                setY(getY() + (e.y - _mouseOriginY)/getEditScaleY());
                _mouseOriginX = e.x;
                _mouseOriginY = e.y;
            }
        }
    }
    
    void mousePressed(int x, int y, int button){
        
        //mouseX = x-ofGetWidth()/2;;
        
        ofMouseEventArgs e;
        e.x = x;
        e.y = y;
        e.button = button;
        
        _aspectType = kASPECT_FREE;
        
        if(keyModifiers.getAppleCommandModifier()){
            _aspectType = kASPECT_MAINTAIN;
            _mouseAspectX = e.x;
            _mouseAspectY = e.y;
            memcpy(_oldCorners, _corners, sizeof(ofPoint[4]));
        }
        if(keyModifiers.getAppleShiftModifier()) _aspectType = kASPECT_LOCKXY;
        
        float smallestDist = 0.2;
        _currentViewPortCorner = -1;
        
        for(int corner = 0; corner < 4; corner++){
            
            float distx = (getCorners()[corner].x - ((float)e.x - getEditX() - (getX()) * getEditScaleX())/(getWidth() * getEditScaleX()));
            float disty = (getCorners()[corner].y - ((float)e.y - getEditY() - (getY()) * getEditScaleY())/(getHeight() * getEditScaleY()));
            
            float dist  = sqrt( distx * distx + disty * disty);
            
            if(dist < smallestDist && dist < 0.1){
                _currentViewPortCorner = corner;
                _mouseOriginX = e.x;
                _mouseOriginY = e.y;
                smallestDist = dist;
                return;
            }
        }
        
        
        if (_currentViewPortCorner == -1) {
            
            if ((float)e.x > getEditX() + (getX()) * getEditScaleX() &&
                (float)e.x < getEditX() + (getX() + getWidth()) * getEditScaleX() &&
                (float)e.y > getEditY() + (getY()) * getEditScaleY() &&
                (float)e.y < getEditY() + (getY() + getHeight()) * getEditScaleY()) {
                
                _mouseOriginX = e.x;
                _mouseOriginY = e.y;
                return;
            }
            
        }
    }
    
    //--------------------------------------------------------------
    void mouseReleased(int x, int y, int button){
        _mouseAspectX = _mouseAspectY = -1;
        _currentViewPortCorner = -1;
        saveViewport();
    }
    
	void		setX(float x) {_x = x;};
	void		setY(float y) {_y = y;};
	void		setWidth(float width) {_width = width;};
	void		setHeight(float height) {_height = height;};
	
	float		getX() {return _x;};
	float		getY() {return _y;};
	float		getWidth() {return _width;};
	float		getHeight() {return _height;};
	ofPoint*	getCorners() {return _corners;};
	
	float		getEditX() {return _editX;};
	float		getEditY() {return _editY;};
	float		getEditScaleX() {return _editScaleX;};
	float		getEditScaleY() {return _editScaleY;};		
	
private:
	
    ofxXmlSettings settings;
    ofxOSXKeyModifiers keyModifiers;
    
    string      _path;
    
	float		_x;
	float		_y;
	float		_width;
	float		_height;
	ofPoint*	_corners;
    ofPoint*    _oldCorners;
	
	float		_editX;
	float		_editY;
	float		_editScaleX;
	float		_editScaleY;
    
	int         _currentViewPortCorner;
    int         _mouseOriginX;
    int         _mouseOriginY;
    int         _mouseAspectX;
    int         _mouseAspectY;
    int         _aspectType;
};

#endif