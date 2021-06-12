
#pragma once

#include "ofMain.h"

//we can handle many app modes to change behaviour
#define NUM_MODES_APP 2

//dependencies
#include "ofxGui.h"
#include "ofxSurfingHelpers.h"

class ofxAddonTemplateCustomApp
{

public:

#pragma mark - ADDON ENGINE

	//all params grouped
	ofParameterGroup params;

    //addon variables
    ofParameterGroup params_Addon;;
	ofParameter<bool> Addon_Active;
	ofParameter<float> Addon_Float;

    //addon methods
    
    //----

#pragma mark - OF

    ofxAddonTemplateCustomApp();
    ~ofxAddonTemplateCustomApp();

    void setup();
    
    void update();
    void draw();
    void update(ofEventArgs & args);
    void draw(ofEventArgs & args);

    void exit();
    void startup();

	//public
	void windowResized(int w, int h);

#pragma mark - API

    void setActive(bool b);
    void setGuiVisible(bool b);
    void setPathGlobal(string s);//must call before setup. disabled by default
    void setLogLevel(ofLogLevel level);
	void setAutoSave(bool b)
	{
		ENABLE_AutoSave = b;
	}

    void setKey_MODE_App(int k);

	//-

private:

    int key_MODE_App = OF_KEY_TAB;//default key to switch MODE_App
    int screenW, screenH;

    //autosave
    ofParameter<bool> ENABLE_AutoSave;
    uint64_t timerLast_Autosave = 0;
    int timeToAutosave = 5000;

    //updating some params before save will trigs also the group callbacks
    //so we disable this callbacks just in case params updatings are required
    //in this case we will need to update gui position param
    bool DISABLE_Callbacks = false;

    //-

	void Changed_params_Addon(ofAbstractParameter &e);

	//-

#pragma mark - ADDON TEMPLATE STUFF

#pragma mark - CONTROL PARAMS

    //control params
    ofParameterGroup params_Control;
    ofParameter<bool> MODE_Active;
    ofParameter<bool> ENABLE_keys;
    ofParameter<bool> ENABLE_Debug;
    ofParameter<bool> SHOW_Gui;
    ofParameter<glm::vec2> Gui_Position;
    ofParameter<bool> SHOW_Help;
    ofParameter<int> MODE_App;
	ofParameter<string> MODE_App_Name;
	ofxPanel gui_Control;

#pragma mark - CALLBACKS

	void Changed_params_Control(ofAbstractParameter &e);
	void Changed_params(ofAbstractParameter &e);

#pragma mark - OF LISTENERS
	
	//keys
    void keyPressed(ofKeyEventArgs &eventArgs);
    void keyReleased(ofKeyEventArgs &eventArgs);
    void addKeysListeners();
    void removeKeysListeners();

	//mouse
    void mouseDragged(ofMouseEventArgs &eventArgs);
    void mousePressed(ofMouseEventArgs &eventArgs);
    void mouseReleased(ofMouseEventArgs &eventArgs);
    void addMouseListeners();
    void removeMouseListeners();

#pragma mark - FILE SETTINGS

    string path_GLOBAL = "ofxAddonTemplateCustomApp/";//this is to folder all files to avoid mixing with other addons data
    string path_Params_Control = "params_Control.xml";
};

//}