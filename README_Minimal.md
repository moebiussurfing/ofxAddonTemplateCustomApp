# ofxAddon
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Stage: beta](https://img.shields.io/badge/-alpha-red)

## Overview
**ofxFboMixerBlend** is an add-on template for openFrameworks to make other addons.

## Screenshot
![Screenshot](docs/Capture.PNG?raw=true "Screenshot")

## Features
- Can have some app modes  (2 by default). So it can have multiple behaviour modes. (Edit/Use).

## Usage
- Copy the folder '/ofxFboMixerBlend'.
- Rename the folder to the name of your addon

## Code Example
ofApp.h
```.cpp
#include "ofxSurfingUndoHelper.h"
ofParameterGroup params {"Params"};
```

ofApp.cpp
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
```

## Dependencies
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxScaleDragRect](https://github.com/moebiussurfing/ofxScaleDragRect)

The above add-ons already packed somewhere into **OF_ADDON/libs**.  
No need to add them manually with the **OF Project Generator**:  
* [ofxColorQuantizerHelper](https://github.com/moebiussurfing/ofxColorQuantizerHelper)

## Tested Systems
* **Windows 10** / **VS 2017** / **OF ~0.11**
* **macOS**. **High Sierra** / **Xcode9** & **Xcode10** / **OF ~0.11**

## Author
An addon by **@moebiusSurfing**  
*( ManuMolina ) 2019-2021*  

## License
[**MIT License**](https://github.com/LICENSE)