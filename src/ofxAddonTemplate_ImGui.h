
#pragma once

#include "ofMain.h"

/*

NOTE:
Notice that this class will be more up-to-date than ofxAddonTemplateCustomApp.h !

TODO:

*/


//--

#include "ofxGui.h"
#include "ofxSurfing_ofxGui.h"
#include "ofxSurfingHelpers.h"
#include "TextBoxWidget.h"

//--

#define USE_IM_GUI // -> Comment to disable all ImGui stuff
#define USE_LAYOUT_ENGINE // -> Uncomment to include Docking mode and the Surfing Layout Engine.
#ifdef USE_IM_GUI
#include "ofxSurfingImGui.h"
#endif

//--

class ofxAddonTemplate_ImGui
{
	//----

public:
	ofxAddonTemplate_ImGui();
	~ofxAddonTemplate_ImGui();

public:
	void setup();
	void update();
	void draw();
	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);
	void exit();

	//----
	
	void startup(); // -> Called when setup done. Usually to load settings.
	void setupParameters();
	void setupGui();
	void drawGui();

	// ImGui
#ifdef USE_IM_GUI
	void setupImGui();
	void setupImGuiStyles();
	void drawImGui();
	void drawImGui_User();
#endif

	// Help
private:
	void setupHelp();
	void drawHelp();
	//// font to label clicker boxes
	//ofTrueTypeFont myFont;
	//std::string myTTF;// gui font for all gui theme
	//int sizeTTF;
	TextBoxWidget helpTextBoxWidget;
	std::string strHelpInfo = "";
	bool bThemeDarkOrLight = true;

	//--

#ifdef USE_IM_GUI
public:
	ofxSurfing_ImGui_Manager guiManager;
#endif

	//--

	// Settings
	void saveSettings();
	void loadSettings();
	void setPathGlobal(string s); // Must call before setup.
	//--------------------------------------------------------------
	void setAutoSave(bool b)
	{
		bAutoSave = b;
	}

	//-

public:
	void setActive(bool b);
	void setGuiVisible(bool b);
	void setGuiVisibleToggle();
	void setLogLevel(ofLogLevel level);
	void dragEvent(ofDragInfo dragInfo);

	//-

public:
	void windowResized(int w, int h);

	// window
private:
	int wWindow, hWindow;
	ofRectangle rectWindow;

	// autosave
	ofParameter<bool> bAutoSave;
	uint64_t timerLastAutosave = 0;
	int timeToAutosave = 5000;

	// updating some params before save will trigs also the group callbacks
	// so we disable this callbacks just in case params updatings are required
	// in this case we will need to update gui position param
	bool bCallbacksEnable;

	//--

	// Group params

private:
//public:
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

	//-

	// Params
	ofParameter<bool> bActive;
	ofParameter<bool> bGui; // all gui global
	ofParameter<bool> bGui_User; // ImGui
	ofParameter<bool> bGui_Advanced; // ofxGui
	ofParameter<bool> bHelp;
	ofParameter<bool> bKeys;
	ofParameter<bool> bDebug;
	ofParameter<glm::vec2> positionGuiAdvanced;

	// gui
	ofxPanel gui_Advanced; // -> We use ofxGui to debug/advanced settings.

	// App modes
	// we can handle many app modes to change behaviour
private:
	enum AppMode
	{
		OF_APP_MODE_UNDEFINED = 0,
		//OF_APP_MODE_INACTIVE,
		OF_APP_MODE_EDIT,
		OF_APP_MODE_LIVE,
		OFX_APP_MODES_AMOUNT
	};
	AppMode appMode = OF_APP_MODE_UNDEFINED;
	//std::string appModeNames[OFX_APP_MODES_AMOUNT] = { "Undefined", /* "Inactive",*/ "Edit", "Live" };
	vector<std::string> appModeNames;
	ofParameter<int> appModeIndex;
	ofParameter<string> appModeName;
public:
	void setKeyAppMode(int k);
private:
	int keyAppMode = OF_KEY_TAB; // default key to switch appModeIndex

	//-

	// callbacks
private:
	void Changed_params_Control(ofAbstractParameter &e);
	void Changed_params(ofAbstractParameter &e);
	void Changed_params_Addon(ofAbstractParameter &e);

	// keys
private:
	void keyPressed(ofKeyEventArgs &eventArgs);
	void keyReleased(ofKeyEventArgs &eventArgs);
	void addKeysListeners();
	void removeKeysListeners();

	// mouse
private:
	void mouseDragged(ofMouseEventArgs &eventArgs);
	void mousePressed(ofMouseEventArgs &eventArgs);
	void mouseReleased(ofMouseEventArgs &eventArgs);
	void addMouseListeners();
	void removeMouseListeners();

	// settings
private:
	string path_Global = "ofxAddonTemplate_ImGui/"; // this is to folder all files to avoid mixing with other addons data
	string path_Params_Control = "params_Control.xml";
	string path_Params_AppSettings = "params_AppSettings.xml";
};
