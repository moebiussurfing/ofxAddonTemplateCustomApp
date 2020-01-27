# ofxAddonTemplate from MoebiusSurfing

## Overview
ofxAddonTemplate is an addon template for openFrameworks to make other addons.

It's a kind of "typical app behaviour" helper that I usually reuse to start from my custom addons.

You can use this as template and add your addon methods/classes inside.

Then you can forgot to add the typical stuff to handle key/mouse listeneres and some other features.

## Screenshot
![Alt text](/screenshot.JPG?raw=true "MoebiusSurfing")

## Features
- can have some app modes  (2 by default). So it can have multiple behaviour modes. (Edit/use).
- active mode: handles key and mouse listeners to (enable or avoid) feed to your addon.
- can set active or inactive mode to the addon, to enable adjust/control or just using "like is"
- can enable help toggle to display info: shortcuts, usage... Need to customize by you.
- can enable a debug mode to debug stuff. Need to customize by you.
- all this settings are stored and recalled when app closes and next session opens.
- auto save timer to aurtosave control settings.

## Usage
- copy the folder '/ofxAddonTemplate'.
- rename the folder to the name of your addon
- open a good text editor. I use Sublime Text for this kind of edits.
- rename all 'ofxAddonTemplate' appearances inside all the files to tha name of your addon. ('ofxPhotoFilter')
- remember to save all files.
- now you can use the addon as usual. create a project with Project Generator. 
- try to build the included example to start from it.

### TODO
- rename all name and tittle
- add some typical debug and help features.
- add some addon with key listeners management.

## Dependencies
- ofxGui

## Tested systems
- Windows 10 / VS2017 / OF 0.11.0

## Author
MoebiusSurfing. 
