/*
 *  SceneView.cpp
 *  Bunyip
 *
 *  Created by gameover on 26/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#include "SceneView.h"
//--------------------------------------------------------------
SceneView::SceneView(float x, float y, float width, float height, ofxFenster* window, string windowTitle) : BaseView(x, y, width, height, window, windowTitle) {

	LOG_NOTICE("Constructing SceneView");

}

/*--------------------------------------------------------------
 SceneView::~SceneView() {

 LOG_NOTICE("Destroying SceneView");

 }*/

//--------------------------------------------------------------
void SceneView::update() {

	begin();
	{

    }
	end();

}
