ofxAddonTemplateCustomApp
=============================

# Overview
**ofxAddonTemplateCustomApp** is an **openFrameworks** addon to

## Screenshot
![image](/readme_images/Capture.PNG?raw=true "Capture1.PNG")

## Features
- 

## Usage
 
### ofApp.h
```.cpp
#include "ofxAddonTemplateCustomApp.h"
ofxAddonTemplateCustomApp myAddon;
```

### ofApp.cpp
```.cpp
ofApp::setup(){
	ofxAddonTemplateCustomApp.setup();
}

ofApp::update(){
	ofxAddonTemplateCustomApp.update();
}

ofApp::draw(){
	ofxAddonTemplateCustomApp.draw();
	ofxAddonTemplateCustomApp.drawGui();
}
```

## Dependencies
- 

## Tested systems
- **Windows10** / **VS2017** / **OF 0.11**

### TODO/IDEAS
* 

### Notes
*

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2020.*

## License
*MIT License.*