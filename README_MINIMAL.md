## Overview
**ofxAddonTemplateCustomApp** is an addon template for **openFrameworks** to make other addons.

## Screenshot

![](Example/Capture.PNG)

## Features
- Can have some app modes  (2 by default). So it can have multiple behaviour modes. (Edit/Use).

## Requeriments
- Copy the folder `/ofxAddonTemplateCustomApp`.
- Rename the folder to the name of your addon.

## Usage

#### ofApp.h
```.cpp
#pragma once
#include "ofMain.h"

#include "ofxAddonTemplateCustomApp.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void draw();
	void keyPressed(int key);
	void exit();
};
```

#### ofApp.cpp
```.cpp
void ofApp::setup()
{
}

void ofApp::draw()
{
}

void ofApp::keyPressed(int key)
{
    if (key == 's')
    {
    }
}

void ofApp::exit()
{
}
```

## Dependencies
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxScaleDragRect](https://github.com/moebiussurfing/ofxScaleDragRect)

## Tested Systems
* **Windows 11** / **VS 2022** / **OF ~0.12**
* **macOS**. **High Sierra** / **Xcode9** & **Xcode10** / **OF ~0.12**

## License
**MIT License**