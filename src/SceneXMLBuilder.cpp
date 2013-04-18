/*
 *  SceneXMLBuilder.cpp
 *  Bunyip
 *
 *  Created by gameover on 26/08/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#include "SceneXMLBuilder.h"

SceneXMLBuilder::SceneXMLBuilder(string xmlFile) : IXMLBuilder(xmlFile){
	buildXML();
	save();
}

void SceneXMLBuilder::buildXML(){

	// get list of all properties
//	map<string, Scene*> scenes = _appModel->getScenes();
//
//	// set xml and root
//	_xml.addTag("scenes");
//	_xml.pushTag("scenes");
//
//	// create nodes for each property
//	map<string, Scene*>::iterator iter = scenes.begin();
//	while(iter != scenes.end()) {
//
//		Scene* scene = iter->second;
//
//		int node = _xml.addTag("scene");
//		_xml.addAttribute("scene", "name", scene->getName(), node);
//		_xml.addAttribute("scene", "numKinectLayers", scene->getNumberOfKinectLayers(), node);
//		_xml.addAttribute("scene", "numVideoLayers", scene->getNumberOfVideoLayers(), node);
//		_xml.addAttribute("scene", "width", scene->getWidth(), node);
//		_xml.addAttribute("scene", "height", scene->getHeight(), node);
//
//		_xml.pushTag("scene", node);
//
//		for (int layer = 0; layer < scene->getNumberOfKinectLayers(); layer++) {
//
//			KinectView* kinectLayer		= scene->getKinectLayer(layer);
//			ViewPort* viewPort			= kinectLayer->getViewPort();
//
//			_xml.addTag("kinectLayer");
//
//			_xml.addAttribute("kinectLayer", "width", kinectLayer->getWidth(), layer);
//			_xml.addAttribute("kinectLayer", "height", kinectLayer->getHeight(), layer);
//			_xml.addAttribute("kinectLayer", "nearThreshold", *kinectLayer->getNearThreshold(), layer);
//			_xml.addAttribute("kinectLayer", "farThreshold", *kinectLayer->getFarThreshold(), layer);
//			_xml.addAttribute("kinectLayer", "minBlobs", *kinectLayer->getMinSizeContourBlobs(), layer);
//			_xml.addAttribute("kinectLayer", "maxBlobs", *kinectLayer->getMaxSizeContourBlobs(), layer);
//			_xml.addAttribute("kinectLayer", "smoothThresh", *kinectLayer->getSmoothContourThreshold(), layer);
//
//			_xml.pushTag("kinectLayer", layer);
//
//			_xml.addTag("effectSettings");
//
//			_xml.addAttribute("effectSettings", "effectType", *kinectLayer->getEffectType(), 0);
//			_xml.addAttribute("effectSettings", "doBlur", *kinectLayer->getDoBlur(), 0);
//			_xml.addAttribute("effectSettings", "doFlipX", *kinectLayer->getDoFlipX(), 0);
//			_xml.addAttribute("effectSettings", "doFlipY", *kinectLayer->getDoFlipY(), 0);
//			_xml.addAttribute("effectSettings", "doGrayscale", *kinectLayer->getDoGreyscale(), 0);
//			_xml.addAttribute("effectSettings", "doInvert", *kinectLayer->getDoInvert(), 0);
//			_xml.addAttribute("effectSettings", "doThreshold", *kinectLayer->getDoThreshold(), 0);
//			_xml.addAttribute("effectSettings", "doHue", *kinectLayer->getDoHue(), 0);
//			_xml.addAttribute("effectSettings", "doColourize", *kinectLayer->getDoColourize(), 0);
//			_xml.addAttribute("effectSettings", "doSaturation", *kinectLayer->getDoSaturation(), 0);
//			_xml.addAttribute("effectSettings", "doContrast", *kinectLayer->getDoContrast(), 0);
//			_xml.addAttribute("effectSettings", "doBrightness", *kinectLayer->getDoBrightness(), 0);
//			_xml.addAttribute("effectSettings", "blurLevel", *kinectLayer->getBlurLevel(), 0);
//			_xml.addAttribute("effectSettings", "thresholdLevel", *kinectLayer->getThresholdLevel(), 0);
//			_xml.addAttribute("effectSettings", "contrastLevel", *kinectLayer->getContrastLevel(), 0);
//			_xml.addAttribute("effectSettings", "saturationLevel", *kinectLayer->getSaturationLevel(), 0);
//			_xml.addAttribute("effectSettings", "brightnessLevel", *kinectLayer->getBrightnessLevel(), 0);
//			_xml.addAttribute("effectSettings", "hueR", kinectLayer->getHueColour()[0], 0);
//			_xml.addAttribute("effectSettings", "hueG", kinectLayer->getHueColour()[1], 0);
//			_xml.addAttribute("effectSettings", "hueB", kinectLayer->getHueColour()[2], 0);
//			_xml.addAttribute("effectSettings", "hueA", kinectLayer->getHueColour()[3], 0);
//			_xml.addAttribute("effectSettings", "colR", kinectLayer->getColourizeColour()[0], 0);
//			_xml.addAttribute("effectSettings", "colG", kinectLayer->getColourizeColour()[1], 0);
//			_xml.addAttribute("effectSettings", "colB", kinectLayer->getColourizeColour()[2], 0);
//			_xml.addAttribute("effectSettings", "colA", kinectLayer->getColourizeColour()[3], 0);
//			_xml.addAttribute("effectSettings", "doSyphon", *kinectLayer->getDoSyphon(), 0);
//            _xml.addAttribute("effectSettings", "doFluid", *kinectLayer->getDoDrawFluid(), 0);
//
//			_xml.addTag("viewPort");
//
//			_xml.addAttribute("viewPort", "x", viewPort->getX(), 0);
//			_xml.addAttribute("viewPort", "y", viewPort->getY(), 0);
//			_xml.addAttribute("viewPort", "width", viewPort->getWidth(), 0);
//			_xml.addAttribute("viewPort", "height", viewPort->getHeight(), 0);
//
//			_xml.pushTag("viewPort", 0);
//
//			for (int corner = 0; corner < 4; corner++) {
//
//				_xml.addTag("corner");
//
//				(float)_xml.addAttribute("corner", "x", viewPort->getCorners()[corner].x, corner);
//				(float)_xml.addAttribute("corner", "y", viewPort->getCorners()[corner].y, corner);
//
//			}
//
//			_xml.popTag();
//			_xml.popTag();
//
//		}
//
//		for (int layer = 0; layer < scene->getNumberOfVideoLayers(); layer++) {
//
//			VideoView* videoLayer		= scene->getVideoLayer(layer);
//			//ViewPort* viewPort			= kinectLayer->getViewPort();
//
//			_xml.addTag("videoLayer");
//
//			_xml.addAttribute("videoLayer", "videoPath", *videoLayer->getVideoPath(), layer);
//			_xml.addAttribute("videoLayer", "width", videoLayer->getWidth(), layer);
//			_xml.addAttribute("videoLayer", "height", videoLayer->getHeight(), layer);
//
//            _xml.pushTag("videoLayer");
//
//            _xml.addTag("effectSettings");
//
//			_xml.addAttribute("effectSettings", "effectType", *videoLayer->getEffectType(), 0);
//			_xml.addAttribute("effectSettings", "doBlur", *videoLayer->getDoBlur(), 0);
//			_xml.addAttribute("effectSettings", "doFlipX", *videoLayer->getDoFlipX(), 0);
//			_xml.addAttribute("effectSettings", "doFlipY", *videoLayer->getDoFlipY(), 0);
//			_xml.addAttribute("effectSettings", "doGrayscale", *videoLayer->getDoGreyscale(), 0);
//			_xml.addAttribute("effectSettings", "doInvert", *videoLayer->getDoInvert(), 0);
//			_xml.addAttribute("effectSettings", "doThreshold", *videoLayer->getDoThreshold(), 0);
//			_xml.addAttribute("effectSettings", "doHue", *videoLayer->getDoHue(), 0);
//			_xml.addAttribute("effectSettings", "doColourize", *videoLayer->getDoColourize(), 0);
//			_xml.addAttribute("effectSettings", "doSaturation", *videoLayer->getDoSaturation(), 0);
//			_xml.addAttribute("effectSettings", "doContrast", *videoLayer->getDoContrast(), 0);
//			_xml.addAttribute("effectSettings", "doBrightness", *videoLayer->getDoBrightness(), 0);
//			_xml.addAttribute("effectSettings", "blurLevel", *videoLayer->getBlurLevel(), 0);
//			_xml.addAttribute("effectSettings", "thresholdLevel", *videoLayer->getThresholdLevel(), 0);
//			_xml.addAttribute("effectSettings", "contrastLevel", *videoLayer->getContrastLevel(), 0);
//			_xml.addAttribute("effectSettings", "saturationLevel", *videoLayer->getSaturationLevel(), 0);
//			_xml.addAttribute("effectSettings", "brightnessLevel", *videoLayer->getBrightnessLevel(), 0);
//			_xml.addAttribute("effectSettings", "hueR", videoLayer->getHueColour()[0], 0);
//			_xml.addAttribute("effectSettings", "hueG", videoLayer->getHueColour()[1], 0);
//			_xml.addAttribute("effectSettings", "hueB", videoLayer->getHueColour()[2], 0);
//			_xml.addAttribute("effectSettings", "hueA", videoLayer->getHueColour()[3], 0);
//			_xml.addAttribute("effectSettings", "colR", videoLayer->getColourizeColour()[0], 0);
//			_xml.addAttribute("effectSettings", "colG", videoLayer->getColourizeColour()[1], 0);
//			_xml.addAttribute("effectSettings", "colB", videoLayer->getColourizeColour()[2], 0);
//			_xml.addAttribute("effectSettings", "colA", videoLayer->getColourizeColour()[3], 0);
//            _xml.addAttribute("effectSettings", "doSyphon", *videoLayer->getDoSyphon(), 0);
//
//		}
//
//		_xml.popTag();
//
//		iter++;
//
//	}
//
//	_xml.popTag();
}
