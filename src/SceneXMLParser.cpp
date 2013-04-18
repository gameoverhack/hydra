/*
 *  SceneXMLParser.cpp
 *  Bunyip
 *
 *  Created by gameover on 26/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#include "SceneXMLParser.h"

SceneXMLParser::SceneXMLParser(string xmlFile) : IXMLParser(xmlFile){
	load();
	parseXML();
}

void SceneXMLParser::parseXML(){

//	if(!_xml.pushTag("scenes", 0)){
//		LOG_ERROR("No scene nodes in config");
//		abort();
//	} else {
//		LOG_VERBOSE("Parsing scenes");
//	}
//
//	for(int node = 0; node < _xml.getNumTags("scene"); node++) {
//
//		string sceneName	= _xml.getAttribute("scene", "name", "error", node);
//		int numKinectLayers = _xml.getAttribute("scene", "numKinectLayers", -1, node);
//		int numVideoLayers	= _xml.getAttribute("scene", "numVideoLayers", -1, node);
//		float sceneWidth	= _xml.getAttribute("scene", "width", -1.0f, node);
//		float sceneHeight	= _xml.getAttribute("scene", "height", -1.0f, node);
//
//		if (sceneName == "error" || numKinectLayers == -1 || numVideoLayers == -1 || sceneWidth == -1.0f || sceneHeight == -1.0f) {
//			LOG_ERROR("Scene attributes are missing or broken!");
//			abort();
//		}
//
//		KinectView* kinectLayers[MAX_KINECT_LAYERS];
//		bool hasKinectLayers = false;
//
//		_xml.pushTag("scene", node);
//
//		for (int layer = 0; layer < _xml.getNumTags("kinectLayer"); layer++) {
//
//			hasKinectLayers = true;
//
//			float layerWidth		= _xml.getAttribute("kinectLayer", "width", -1.0f, layer);
//			float layerHeight		= _xml.getAttribute("kinectLayer", "height", -1.0f, layer);
//			int layerNearThresh		= _xml.getAttribute("kinectLayer", "nearThreshold", 0, layer);
//			int layerFarThresh		= _xml.getAttribute("kinectLayer", "farThreshold", 10000, layer);
//			int layerMinBlobs		= _xml.getAttribute("kinectLayer", "minBlobs", 1000, layer);
//			int layerMaxBlobs		= _xml.getAttribute("kinectLayer", "maxBlobs", 200000, layer);
//			float layerSmooth		= _xml.getAttribute("kinectLayer", "smoothThresh", 0.2, layer);
//
//			if (layerWidth == -1.0f || layerHeight == -1.0f) {
//				LOG_ERROR("KinectLayer attributes are missing or broken!");
//				abort();
//			}
//
//			_xml.pushTag("kinectLayer", layer);
//
//			int kct_effectType			= _xml.getAttribute("effectSettings", "effectType", kEFFECT_NONE, 0);
//			bool kct_doBlur				= _xml.getAttribute("effectSettings", "doBlur", 0, 0);
//			bool kct_doFlipX			= _xml.getAttribute("effectSettings", "doFlipX", false, 0);
//			bool kct_doFlipY			= _xml.getAttribute("effectSettings", "doFlipY", false, 0);
//			bool kct_doGrayscale		= _xml.getAttribute("effectSettings", "doGrayscale", false, 0);
//			bool kct_doInvert			= _xml.getAttribute("effectSettings", "doInvert", false, 0);
//			bool kct_doThreshold		= _xml.getAttribute("effectSettings", "doThreshold", false, 0);
//			bool kct_doHue				= _xml.getAttribute("effectSettings", "doHue", false, 0);
//			bool kct_doColourize		= _xml.getAttribute("effectSettings", "doColourize", false, 0);
//			bool kct_doSaturation		= _xml.getAttribute("effectSettings", "doSaturation", false, 0);
//			bool kct_doContrast			= _xml.getAttribute("effectSettings", "doContrast", false, 0);
//			bool kct_doBrightness		= _xml.getAttribute("effectSettings", "doBrightness", false, 0);
//			int	kct_blurLevel			= _xml.getAttribute("effectSettings", "blurLevel", 0, 0);
//			float kct_thresholdLevel	= _xml.getAttribute("effectSettings", "thresholdLevel", 0.0f, 0);
//			float kct_contrastLevel		= _xml.getAttribute("effectSettings", "contrastLevel", 0.0f, 0);
//			float kct_saturationLevel	= _xml.getAttribute("effectSettings", "saturationLevel", 0.0f, 0);
//			float kct_brightnessLevel	= _xml.getAttribute("effectSettings", "brightnessLevel", 0.0f, 0);
//			bool kct_doDrawFluid		= _xml.getAttribute("effectSettings", "doFluid", false, 0);
//
//			float kct_hue[4]; float kct_col[4];
//
//			kct_hue[0]					= _xml.getAttribute("effectSettings", "hueR", 1.0f, 0);
//			kct_hue[1]					= _xml.getAttribute("effectSettings", "hueG", 1.0f, 0);
//			kct_hue[2]					= _xml.getAttribute("effectSettings", "hueB", 1.0f, 0);
//			kct_hue[3]					= _xml.getAttribute("effectSettings", "hueA", 1.0f, 0);
//			kct_col[0]					= _xml.getAttribute("effectSettings", "colR", 1.0f, 0);
//			kct_col[1]					= _xml.getAttribute("effectSettings", "colG", 1.0f, 0);
//			kct_col[2]					= _xml.getAttribute("effectSettings", "colB", 1.0f, 0);
//			kct_col[3]					= _xml.getAttribute("effectSettings", "colA", 1.0f, 0);
//
//            bool kct_doSyphon           = _xml.getAttribute("effectSettings", "doSyphon", false, 0);
//
//			float viewportX			= _xml.getAttribute("viewPort", "x", -1.0f, 0);
//			float viewportY			= _xml.getAttribute("viewPort", "y", -1.0f, 0);
//			float viewportWidth		= _xml.getAttribute("viewPort", "width", -1.0f, 0);
//			float viewportHeight	= _xml.getAttribute("viewPort", "height", -1.0f, 0);
//
//			ofPoint* viewportCorners = new ofPoint[4];
//
//			_xml.pushTag("viewPort", 0);
//
//			for (int corner = 0; corner < 4; corner++) {
//				viewportCorners[corner].x = _xml.getAttribute("corner", "x", 0.0f, corner);
//				viewportCorners[corner].y = _xml.getAttribute("corner", "y", 0.0f, corner);
//			}
//
//			if (viewportX == -1.0f || viewportY == -1.0f || viewportWidth == -1.0f || viewportHeight == -1.0f) {
//				LOG_ERROR("ViewPort attributes are missing or broken!");
//				abort();
//			}
//
//			ViewPort * viewPort = new ViewPort(viewportX, viewportY, viewportWidth, viewportHeight, viewportCorners);
//			kinectLayers[layer] = new KinectView(layerWidth, layerHeight, layer, layerSmooth, layerMinBlobs, layerMaxBlobs, viewPort, layerNearThresh, layerFarThresh);
//			kinectLayers[layer]->setAllEffects(kct_effectType, kct_doBlur, kct_doFlipX, kct_doFlipY, kct_doGrayscale, kct_doInvert, kct_doThreshold, kct_doHue, kct_doColourize,        kct_doSaturation, kct_doContrast, kct_doBrightness, kct_blurLevel, kct_thresholdLevel, kct_contrastLevel, kct_saturationLevel, kct_brightnessLevel, kct_hue, kct_col);
//            kinectLayers[layer]->setDoSyphon(kct_doSyphon);
//            kinectLayers[layer]->setDoDrawFluid(kct_doDrawFluid);
//
//			_xml.popTag();
//			_xml.popTag();
//
//		}
//
//		VideoView* videoLayers[MAX_VIDEO_LAYERS];
//		bool hasVideoLayers = false;
//
//		for (int layer = 0; layer < _xml.getNumTags("videoLayer"); layer++) {
//
//			hasVideoLayers = true;
//
//			string videoPath	= _xml.getAttribute("videoLayer", "videoPath", "error", layer);
//			float videoWidth	= _xml.getAttribute("videoLayer", "width", -1.0f, 0);
//			float videoHeight	= _xml.getAttribute("videoLayer", "height", -1.0f, 0);
//
//            _xml.pushTag("videoLayer", layer);
//
//			int vid_effectType			= _xml.getAttribute("effectSettings", "effectType", kEFFECT_NONE, 0);
//			bool vid_doBlur				= _xml.getAttribute("effectSettings", "doBlur", 0, 0);
//			bool vid_doFlipX			= _xml.getAttribute("effectSettings", "doFlipX", false, 0);
//			bool vid_doFlipY			= _xml.getAttribute("effectSettings", "doFlipY", false, 0);
//			bool vid_doGrayscale		= _xml.getAttribute("effectSettings", "doGrayscale", false, 0);
//			bool vid_doInvert			= _xml.getAttribute("effectSettings", "doInvert", false, 0);
//			bool vid_doThreshold		= _xml.getAttribute("effectSettings", "doThreshold", false, 0);
//			bool vid_doHue				= _xml.getAttribute("effectSettings", "doHue", false, 0);
//			bool vid_doColourize		= _xml.getAttribute("effectSettings", "doColourize", false, 0);
//			bool vid_doSaturation		= _xml.getAttribute("effectSettings", "doSaturation", false, 0);
//			bool vid_doContrast			= _xml.getAttribute("effectSettings", "doContrast", false, 0);
//			bool vid_doBrightness		= _xml.getAttribute("effectSettings", "doBrightness", false, 0);
//			int	vid_blurLevel			= _xml.getAttribute("effectSettings", "blurLevel", 0, 0);
//			float vid_thresholdLevel	= _xml.getAttribute("effectSettings", "thresholdLevel", 0.0f, 0);
//			float vid_contrastLevel		= _xml.getAttribute("effectSettings", "contrastLevel", 0.0f, 0);
//			float vid_saturationLevel	= _xml.getAttribute("effectSettings", "saturationLevel", 0.0f, 0);
//			float vid_brightnessLevel	= _xml.getAttribute("effectSettings", "brightnessLevel", 0.0f, 0);
//
//			float vid_hue[4]; float vid_col[4];
//
//			vid_hue[0]					= _xml.getAttribute("effectSettings", "hueR", 1.0f, 0);
//			vid_hue[1]					= _xml.getAttribute("effectSettings", "hueG", 1.0f, 0);
//			vid_hue[2]					= _xml.getAttribute("effectSettings", "hueB", 1.0f, 0);
//			vid_hue[3]					= _xml.getAttribute("effectSettings", "hueA", 1.0f, 0);
//			vid_col[0]					= _xml.getAttribute("effectSettings", "colR", 1.0f, 0);
//			vid_col[1]					= _xml.getAttribute("effectSettings", "colG", 1.0f, 0);
//			vid_col[2]					= _xml.getAttribute("effectSettings", "colB", 1.0f, 0);
//			vid_col[3]					= _xml.getAttribute("effectSettings", "colA", 1.0f, 0);
//
//            bool vid_doSyphon           = _xml.getAttribute("effectSettings", "doSyphon", false, 0);
//
//            _xml.popTag();
//
//			videoLayers[layer]	= new VideoView(videoWidth, videoHeight, layer, videoPath);
//			videoLayers[layer]->setAllEffects(vid_effectType, vid_doBlur, vid_doFlipX, vid_doFlipY, vid_doGrayscale, vid_doInvert, vid_doThreshold, vid_doHue, vid_doColourize,        vid_doSaturation, vid_doContrast, vid_doBrightness, vid_blurLevel, vid_thresholdLevel, vid_contrastLevel, vid_saturationLevel, vid_brightnessLevel, vid_hue, vid_col);
//			videoLayers[layer]->setDoSyphon(vid_doSyphon);
//
//            videoLayers[layer]->loadMovie();
//			goThreadedVideo* video = videoLayers[layer]->getVideo();
//
//			if (video == NULL) continue;
//
//			while (video->isLoading()) {
//				video->psuedoUpdate();
//				video->psuedoDraw();
//			}
//
//			video->setPaused(true);
//			video->setFrame(0);
//
//		}
//
//		Scene* scene = new Scene(sceneName,
//								 numKinectLayers,
//								 numVideoLayers,
//								 sceneWidth,
//								 sceneHeight,
//								 (hasKinectLayers ? kinectLayers : NULL),
//								 (hasVideoLayers ? videoLayers : NULL));
//
//		_appModel->setScene(scene);
//
//		_xml.popTag();
//
//	}

}
