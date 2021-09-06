
#pragma once

#include "ofMain.h"

/*

TODO:

*/


#include "ofxGui.h"
#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ofxGui.h"

class ofxAddonTemplateCustomApp
{

private:
	// we can handle many app modes to change behaviour
	enum AppMode
	{
		OF_APP_MODE_UNDEFINED = 0,
		OF_APP_MODE_INACTIVE,
		OF_APP_MODE_EDIT,
		OF_APP_MODE_LIVE,
		OFX_APP_MODES_AMOUNT
	};
	AppMode appMode = OF_APP_MODE_UNDEFINED;


	//--

public:

	// app session
	ofParameterGroup params_AppSettings;

	// to get callbacks
	ofParameterGroup params_Control;

	// addon variables
	ofParameterGroup params_Addon;;
	ofParameter<bool> Addon_Active;
	ofParameter<float> Addon_Float;

	// all params grouped
	ofParameterGroup params;

	//----

public:
	ofxAddonTemplateCustomApp();
	~ofxAddonTemplateCustomApp();

public:
	void setup();
	void update();
	void draw();
	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);
	void exit();

	void startup();
	void setupGui();
	void drawGui();

public:
	void windowResized(int w, int h);

	//-

public:
	void setActive(bool b);
	void setGuiVisible(bool b);
	void setPathGlobal(string s); // must call before setup. disabled by default
	void setLogLevel(ofLogLevel level);
	void setAutoSave(bool b)
	{
		bAutoSave = b;
	}

	//-

public:
	void setKeyAppMode(int k);

private:
	int keyAppMode = OF_KEY_TAB; // default key to switch appModeIndex

	//-

private:
	// window
	int wWindow, hWindow;
	ofRectangle rectWindow;

	// autosave
	ofParameter<bool> bAutoSave;
	uint64_t timerLastAutosave = 0;
	int timeToAutosave = 5000;

	// updating some params before save will trigs also the group callbacks
	// so we disable this callbacks just in case params updatings are required
	// in this case we will need to update gui position param
	bool bCallbacksEnable = false;

	//-

	// control params
	ofParameter<bool> bActive;
	ofParameter<bool> bKeys;
	ofParameter<bool> bDebug;
	ofParameter<bool> bGui;
	ofParameter<bool> bHelp;
	ofParameter<int> appModeIndex;
	ofParameter<string> appModeName;
	ofParameter<glm::vec2> positionGui;
	ofxPanel gui_Control;

	// callbacks
	void Changed_params_Control(ofAbstractParameter &e);
	void Changed_params(ofAbstractParameter &e);
	void Changed_params_Addon(ofAbstractParameter &e);

	// keys
	void keyPressed(ofKeyEventArgs &eventArgs);
	void keyReleased(ofKeyEventArgs &eventArgs);
	void addKeysListeners();
	void removeKeysListeners();

	// mouse
	void mouseDragged(ofMouseEventArgs &eventArgs);
	void mousePressed(ofMouseEventArgs &eventArgs);
	void mouseReleased(ofMouseEventArgs &eventArgs);
	void addMouseListeners();
	void removeMouseListeners();

	// settings
	string path_GLOBAL = "ofxAddonTemplateCustomApp/"; // this is to folder all files to avoid mixing with other addons data
	string path_Params_Control = "params_Control.xml";
	string path_Params_AppSettings = "params_AppSettings.xml";

	//-

public:
	void dragEvent(ofDragInfo dragInfo);
};
