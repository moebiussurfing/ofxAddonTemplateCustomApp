ofxAddonTemplateCustomApp
=============================
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Stage: beta](https://img.shields.io/badge/-alpha-red)

# Overview
**ofxAddonTemplateCustomApp** is a **PERSONAL** template addon for **openFrameworks** to make other **addons** or **oF-Apps** for **MYSELF**.

## Screenshot
![image](docs/readme_images/Capture1.PNG?raw=true "image")

## Features

- Common ofParameters. 
- ofxGui and ImGui GUI with customized themes.
- Auto Store/Recall app session settings.
- Callbacks methods for litering.
- Key commands with modificators.
- ImGui initiation wiht Layout Presets Engine.
- Readme files, git ignore and doc folder for screenshots.
- Different app modes.
- Readme files, git ignore and some scripts.
- Key commands snippets.

## Usage

_[ EXAMPLE FOR OTHER ADDONS ]_  

### ofApp.h
```.cpp
#include "ofxAddonTemplateCustomApp.h"

ofxAddonTemplateCustomApp myAddon;
```
_[ EXAMPLE FOR OTHER ADDONS ]_  
### ofApp.cpp
```.cpp
ofApp::setup()
{
	ofxAddonTemplateCustomApp.setup();
}

ofApp::update()
{
	ofxAddonTemplateCustomApp.update();
}

ofApp::draw()
{
	ofxAddonTemplateCustomApp.draw();
	ofxAddonTemplateCustomApp.drawGui();
}
```

<details>
  <summary>Dependencies</summary>
  <p>

Clone these add-ons and include into the **OF Project Generator** to allow compile your projects or the examples:
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui)  
* [ofxImGui](https://github.com/Daandelange/ofxImGui/) [*Fork*]  
* ofxGui [*Core*]
_[ EXAMPLE FOR OTHER ADDONS ]_  
* [ofxScaleDragRect](https://github.com/moebiussurfing/ofxScaleDragRect)
* [ofxWindowApp](https://github.com/moebiussurfing/ofxWindowApp)  [ Only for the example ]
* ofxXmlSettings [*Core*]

Above add-ons already packed into **OF_ADDON/libs**. No need to add them manually with the **OF Project Generator**:  
* [ofxColorQuantizerHelper](https://github.com/moebiussurfing/ofxColorQuantizerHelper)

*Thanks a lot to all these ofxAddons coders. Look into each folder for authoring credits, original forks, and license info.*  
  </p>
</details>

<details>
  <summary>Tested Systems</summary>
  <p>

  - **Windows 10** / **VS 2017** / **OF ~0.11**
  - **macOS**. **High Sierra** / **Xcode9** & **Xcode10** / **OF ~0.11**
  </p>
</details>

<details>
  <summary>TODO / NOTES</summary>
  <p>

* Hit me up if you have any suggestions or feature requests.
* 
  </p>
</details>

## Author
An addon by **@moebiusSurfing**  
*( ManuMolina ) 2019-2021*  

### Contact / Follow
[Twitter](https://twitter.com/moebiussurfing/) | [Instagram](https://www.instagram.com/moebiussurfing/) | [YouTube](https://www.youtube.com/channel/UCzUw96_wjmNxyIoFXf84hQg)  

### License
**MIT License**
