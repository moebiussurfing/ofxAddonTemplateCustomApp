# ofxAddonTemplateCustomApp from MoebiusSurfing

## Overview
ofxAddonTemplateCustomApp is a template addon for openFrameworks to make other 'like app' addons.

It's a kind of "typical app behaviour" helper that I usually reuse to start from my custom addons.

You can use this as template and add your custom addon methods/classes inside.

So you can forgot to add the typical stuff to handle key/mouse listeneres and some other features.

## Screenshot
![Alt text](/screenshot.JPG?raw=true "MoebiusSurfing")

## Features
- can have some app modes  (2 by default). So it can have multiple behaviour modes. (Edit/Use).
- active mode: handles key and mouse listeners to (enable or avoid) feed to your addon.
- can set Active or inactive mode to the addon, to enable Edit the addon from your ofApp or just Use "like it is".
- can enable Help toggle to display info: shortcuts, usage... (Need to be implemented by you.)
- can enable a Debug Mode to debug/print stuff. (Need to be implemented by you.)
- all this settings are stored and recalled when app closes and next session opens.
- auto save timer to autosave control settings.
- it includes the typical snippets to get and filter ofParameterGroup callbacks.

## Usage
- copy the folder '/ofxAddonTemplateCustomApp'.
- rename the folder to the name of your addon
- open a good text editor. I use Sublime Text for this kind of edits.
- rename all 'ofxAddonTemplateCustomApp' appearances inside the text content in all the files to the name of your addon. ('ofxPhotoFilter')
- dp the same rename to required file names.
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
