
#pragma once

#include "ofMain.h"

#include "ofxGui.h"

#define NUM_MODES_APP 2

//namespace ofxAddonTemplate {
//class ofxAddonTemplate : public ofEasyCam {

class ofxAddonTemplate
{

public:

#pragma mark - OF

    ofxAddonTemplate();
    ~ofxAddonTemplate();

    void setup();
    void update();
    void draw();
    void exit();
    void windowResized(int w, int h);

#pragma mark - API

    void setActive(bool b);
    void setGuiVisible(bool b);
    void setPathGlobal(string s);//must call before setup. disabled by default

    void setLogLevel(ofLogLevel level);

    void setKey_MODE_App(int k);
    int key_MODE_App = OF_KEY_TAB;//default key to switch MODE_App

private:

    int screenW, screenH;

#pragma mark - PARAMS

    //params
    ofParameterGroup params;
    ofParameter<bool> MODE_Active;
    ofParameter<bool> ENABLE_keys;
    ofParameter<bool> ENABLE_Debug;
    ofParameter<bool> SHOW_Gui;
    ofParameter<glm::vec2> Gui_Position;
    ofParameter<bool> SHOW_Help;
    ofParameter<int> MODE_App;
    ofxPanel gui;

#pragma mark - CALLBACKS

    void Changed_params(ofAbstractParameter &e);

#pragma mark - OF LISTENERS

    void keyPressed(ofKeyEventArgs &eventArgs);
    void keyReleased(ofKeyEventArgs &eventArgs);
    void addKeysListeners();
    void removeKeysListeners();

    void mouseDragged(ofMouseEventArgs &eventArgs);
    void mousePressed(ofMouseEventArgs &eventArgs);
    void mouseReleased(ofMouseEventArgs &eventArgs);
    void addMouseListeners();
    void removeMouseListeners();

#pragma mark - FILE SETTINGS

    string path_GLOBAL = "ofxAddonTemplate/";//this is to folder all files to avoid mixing with other addons data
    string path_Params = "params.xml";

    void loadParams(ofParameterGroup &g, string path);
    void saveParams(ofParameterGroup &g, string path);

};

//}