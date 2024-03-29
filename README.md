ofxAddonTemplateCustomApp
=============================
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Stage: beta](https://img.shields.io/badge/-alpha-red)

# Overview
**ofxAddonTemplateCustomApp** is a **PERSONAL** TEMPLATE addon for **openFrameworks** to make other **addons** or **oF-Apps** for **MYSELF**.

## Included classes:
- **myClassBasic** (.h/.cpp)  
A very simple template class with common oF methods: _setup/update/draw/_ ...etc.  

- **ofxAddonTemplateCustomApp** (.h/.cpp)  
A bigger template with more functions.  

- **ofxAddonTemplate_ImGui** (.h/.cpp)  
Added ImGui stuff but requires my [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui) addon.  
This file is also more up-to-date than **ofxAddonTemplateCustomApp**.  

## Screenshot
![image](docs/readme_images/Capture1.PNG?raw=true "image")

## Features
- Common ofParameters and groups.
  - ofxGui and ImGui GUI with customized themes.
  - ImGui initiation with a Layout Presets Engine.
    - Auto Store/Recall app session settings.
    - Callbacks methods for filtering ofParams.
- Callbacks for auto call update/draw.
- Key commands with modificators and snippets.
  - Readme files, Git ignore, some scripts and /doc/readme-images/ folders for screenshots.
    - Different app modes.
- Draggable floating help box.

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

<details>
  <summary>Dependencies</summary>
  <p>

Clone these add-ons and include into the **OF Project Generator** to allow compile your projects or the examples:
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui) / Only for the example.  
* [ofxImGui](https://github.com/Daandelange/ofxImGui/) / Fork  
* ofxGui / OF core  

_[ EXAMPLE FOR OTHER ADDONS ]_  

The below add-ons are already packed into **OF_ADDON/libs/**.  
No need to add them manually with the **OF PROJECT GENERATOR**:  
* [ofxAddonTemplateCustomApp](https://github.com/moebiussurfing/ofxAddonTemplateCustomApp)  

*Thanks a to all these ofxAddons coders. Look into each folder for authoring credits, original forks, and license info.*  
 </p>
</details>

<details>
  <summary>Tested Systems</summary>
  <p>

* **Windows 11** / **VS 2022** / **OF ~0.12**
* **macOS**. **High Sierra** / **Xcode9** & **Xcode10** / **OF ~0.12**
  </p>
</details>

<details>
  <summary>TODO / NOTES</summary>
  <p>

* Hit me up if you have any suggestions or feature requests.
 
  </p>
</details>

## Author
An addon by **@moebiusSurfing**  
*( ManuMolina ) 2019-2023*  

### Contact / Follow
<p>
<a href="mailto:moebiussurfing@gmail.com" target="_blank">Email</a> |
<a href="https://twitter.com/moebiusSurfing/" rel="nofollow">Twitter</a> | 
<a href="https://www.youtube.com/moebiusSurfing" rel="nofollow">YouTube</a> | 
<a href="https://www.instagram.com/moebiusSurfing/" rel="nofollow">Instagram</a> | 
<a href="https://github.com/moebiussurfing" target="_blank">GitHub</a> 
</p>

### License
MIT License