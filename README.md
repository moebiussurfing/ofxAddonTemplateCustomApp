# ofxAddonTemplate from MoebiusSurfing

## Overview
ofxAddonTemplate is an addon template for openFrameworks to make other addons.

It's a kind of "typical app behaviour" helper that I usually reuse to start from my custom addons.

You can use this as template and add your addon methods/classes inside.

Then you can forgot to add the typical stuff to handle key/mouse listeneres and some other features.

## Features
- can have some app modes  (2 by default). So it can have multiple behaviour modes. (Edit/use).
- active mode: handles key and mouse listeners to (enable or avoid) feed to your addon.
- can set active or inactive mode to the addon, to enable adjust/control or just using "like is"
- can enable help toggle to display info: shortcuts, usage... Need to customize by you.
- can enable a debug mode to debug stuff. Need to customize by you.
- all this settings are stored and recalled when app closes and next session opens.
- auto save timer to aurtosave control settings.

## Screenshot
![Alt text](/screenshot.JPG?raw=true "MoebiusSurfing")

### TODO
- add some typical debug and help features.
- add some addon with key listeners management.

## Dependencies
- ofxGui

## Tested systems
- Windows 10 / VS2017 / OF 0.11.0

## Author
MoebiusSurfing. 
