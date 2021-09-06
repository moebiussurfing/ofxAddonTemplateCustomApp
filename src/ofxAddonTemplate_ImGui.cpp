#include "ofxAddonTemplate_ImGui.h"

//--------------------------------------------------------------
ofxAddonTemplate_ImGui::ofxAddonTemplate_ImGui()
{
	// path for settings
	setPathGlobal("ofxAddonTemplate_ImGui/");
	path_Params_AppSettings = "ofxAddonTemplate_ImGui_AppSettings.xml";
	path_Params_Control = "Params_Control.xml";

	bCallbacksEnable = true;
	setActive(true); // adds key and mouse listeners

	ofAddListener(ofEvents().update, this, &ofxAddonTemplate_ImGui::update);
	ofAddListener(ofEvents().draw, this, &ofxAddonTemplate_ImGui::draw);
	//ofAddListener(ofEvents().draw, this, &ofxAddonTemplate_ImGui::draw, OF_EVENT_ORDER_AFTER_APP);

	setup();
}

//--------------------------------------------------------------
ofxAddonTemplate_ImGui::~ofxAddonTemplate_ImGui()
{
	setActive(false); // removes keys and mouse listeners

	// removes params callbacks listeners
	ofRemoveListener(params.parameterChangedE(), this, &ofxAddonTemplate_ImGui::Changed_params);
	ofRemoveListener(params_Control.parameterChangedE(), this, &ofxAddonTemplate_ImGui::Changed_params_Control);
	ofRemoveListener(params_Addon.parameterChangedE(), this, &ofxAddonTemplate_ImGui::Changed_params_Addon);

	// removes update/draw callbacks
	ofRemoveListener(ofEvents().update, this, &ofxAddonTemplate_ImGui::update);
	ofRemoveListener(ofEvents().draw, this, &ofxAddonTemplate_ImGui::draw);
	//ofRemoveListener(ofEvents().draw, this, &ofxAddonTemplate_ImGui::draw, OF_EVENT_ORDER_AFTER_APP);

	exit();
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setup()
{
	// log mode
	ofSetLogLevel("ofxAddonTemplate_ImGui", OF_LOG_NOTICE);

	//--

	// window
	wWindow = ofGetWidth();
	hWindow = ofGetHeight();
	rectWindow = ofGetCurrentViewport();

	//--

	// addon params
	Addon_Active.set("Addon Active", true);
	Addon_Float.set("Addon Float", 0, 0, 1);

	// group
	params_Addon.setName("Params Addon");
	params_Addon.add(Addon_Active);
	params_Addon.add(Addon_Float);

	// callback
	ofAddListener(params_Addon.parameterChangedE(), this, &ofxAddonTemplate_ImGui::Changed_params_Addon);

	//-

	// addon control (internal)

	// params

	bActive.set("Active", true);
	bKeys.set("Keys", true);
	bHelp.set("Help", false);

	appModeIndex.set("App Mode", 0, 0, OFX_APP_MODES_AMOUNT - 1);
	appModeName.set("", "");

	bDebug.set("Debug", true);
	bAutoSave.set("AutoSave", true);

	bGui.set("Gui", true);
	bGui_User.set("Gui User", true);

	bGui_Advanced.set("Gui Advanced", true);
	positionGui.set("Gui Position",
		glm::vec2(wWindow * 0.5, hWindow * 0.5),
		glm::vec2(0, 0),
		glm::vec2(wWindow, hWindow)
	);

	// params control (internal)
	params_Control.setName("Internal");
	params_Control.add(appModeIndex);
	params_Control.add(appModeName);
	params_Control.add(bActive);
	params_Control.add(bKeys);
	params_Control.add(bHelp);
	params_Control.add(bDebug);
	params_Control.add(bAutoSave);
	params_Control.add(bGui);
	params_Control.add(bGui_User);
	params_Control.add(bGui_Advanced);
	params_Control.add(positionGui);

	//-

	// params App settings
	params_AppSettings.setName("AppSettings");
	params_AppSettings.add(bGui);
	params_AppSettings.add(bGui_User);
	params_AppSettings.add(bGui_Advanced);
	params_AppSettings.add(bHelp);
	params_AppSettings.add(bKeys);

	//-

	// exclude for settings
	bDebug.setSerializable(false);
	appModeName.setSerializable(false);

	//-

	// callback
	ofAddListener(params_Control.parameterChangedE(), this, &ofxAddonTemplate_ImGui::Changed_params_Control);

	//-

	// all params
	params.setName("All Params");
	params.add(params_AppSettings);
	params.add(params_Addon);
	params.add(params_Control);

	// callback
	ofAddListener(params.parameterChangedE(), this, &ofxAddonTemplate_ImGui::Changed_params);

	//--

	// gui
	setupGui();

	//--

	// startup
	startup();

	//--
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setupGui()
{
	ofLogNotice(__FUNCTION__);

	//-

	// setup ofxGui
	{
		// theme
		ofxSurfingHelpers::setThemeDarkMini_ofxGui();

		//-

		gui_Advanced.setup("ofxAddonTemplate_ImGui");
		gui_Advanced.add(params);//add control (internal) and addon params
		gui_Advanced.setPosition(positionGui.get().x, positionGui.get().y);
		//gui_Advanced.setPosition(ofGetWidth() - 210, 20);

		// collapse groups
		auto &g0 = gui_Advanced.getGroup(params.getName());//1st level
		auto &g2 = g0.getGroup(params_Addon.getName());//2nd level
		auto &g3 = g0.getGroup(params_Control.getName());//2nd level
		auto &g31 = g3.getGroup(positionGui.getName());//3nd level
		//g0.minimize();
		g2.minimize();
		g3.minimize();
		g31.minimize();
	}

	//--

#ifdef USE_IM_GUI
	// setup ImGui
	setupImGui();
#endif
}

//--

// ImGui
#ifdef USE_IM_GUI
//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setupImGui()
{
	ofLogNotice(__FUNCTION__);

	//-

	//guiManager.setImGuiViewPort(true); // -> For floating windows

	guiManager.setSettingsPathLabel("ofxAddonTemplate_ImGui");
	// -> Customize the path for settings
	// Useful for multiple instances not sharing settings.

	//-

#ifdef USE_LAYOUT_ENGINE // -> For enabling docking and layouts engine.
	guiManager.setup(IM_GUI_MODE_INSTANTIATED_DOCKING);
#endif

#ifndef USE_LAYOUT_ENGINE // -> For simple mode without Engine Layout
	guiManager.setup(IM_GUI_MODE_INSTANTIATED);
#endif

	//-

#ifdef USE_LAYOUT_ENGINE
	//if (0)
	{
		// -> Pre populate windows.
		// This enables the Layout Engine
		guiManager.addWindow("Audio Window", true);
		guiManager.addWindow("Main Window", true);
		guiManager.addWindow("Another"); // but without advanced mode
		guiManager.startup();
	}
#endif

	//-

	setupImGuiStyles();
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setupImGuiStyles()
{
	ofLogNotice(__FUNCTION__);

	// styles
	{
		//guiManager.clearStyles();

		//guiManager.AddStyle(bRotateGlobal, OFX_IM_TOGGLE_SMALL);
		//guiManager.AddStyle(bGui_Rotators, OFX_IM_TOGGLE_SMALL);
		//guiManager.AddStyle(bResetAllRotators, OFX_IM_BUTTON_SMALL);

		//guiManager.AddStyle(animatorFloat.bGui, OFX_IM_TOGGLE_SMALL);
		//guiManager.AddStyle(animatorNoise.bGui, OFX_IM_TOGGLE_SMALL);
		//guiManager.AddStyle(bGui_EasyCam, OFX_IM_TOGGLE_SMALL);

		//guiManager.AddStyle(bModeEdit, OFX_IM_TOGGLE_BIG);
		//guiManager.AddStyle(bModeLive, OFX_IM_TOGGLE_BIG);
		//guiManager.AddStyle(bSave, OFX_IM_TOGGLE_BIG);
	}
}
//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::drawImGui_User() { // user gui
	//return;

	ImGui::Begin("A RAW Window out of the Layout Engine");
	ImGui::Text("Hello");
	ImGui::TextWrapped("A RAW Window out of the Layout Engine");
	ImGui::End();

	//-

#ifdef USE_LAYOUT_ENGINE
	//if (0)
	{
		if (guiManager.beginWindowSpecial(0)) // -> This is our helpers to render windows
		{
			ImGui::Text("Hello, left!");
			ImGui::Text("Hello, left!");
			ImGui::Text("Hello, left!");
			ImGui::Text("Hello, left!");
			ImGui::Text("Hello, left!");

			guiManager.endWindowSpecial(); // -> Must be called inside the if / beginWindow brackets!
		}

		if (guiManager.beginWindowSpecial(1))
		{
			ImGui::Text("Hello, down!");
			ImGui::Text("Hello, down!");
			ImGui::Text("Hello, down!");
			ImGui::Text("Hello, down!");
			ImGui::Text("Hello, down!");

			guiManager.endWindowSpecial();
		}

		if (guiManager.beginWindowSpecial(2))
		{
			ImGui::Text("Hello");

			guiManager.endWindowSpecial();
		}
	}
#endif

	//-

#ifndef USE_LAYOUT_ENGINE
	//if (0)
	{
		if (ImGui::Begin("RAW Window 0"))
		{
			ImGui::Text("Hello, left!");
			ImGui::Text("Hello, left!");
			ImGui::Text("Hello, left!");
			ImGui::Text("Hello, left!");
			ImGui::Text("Hello, left!");
			ImGui::End();
		}

		if (ImGui::Begin("RAW Window 1"))
		{
			ImGui::Text("Hello, down!");
			ImGui::Text("Hello, down!");
			ImGui::Text("Hello, down!");
			ImGui::Text("Hello, down!");
			ImGui::Text("Hello, down!");
			ImGui::End();
		}

		if (ImGui::Begin("RAW Window 2"))
		{
			ImGui::Text("Hello");
			ImGui::End();
		}
	}
#endif
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::drawImGui() {
	if (!bGui_User) return;

	//-

	guiManager.begin();
	{
#ifdef USE_LAYOUT_ENGINE
		guiManager.beginDocking();
		{
		}
		guiManager.endDocking();
#endif

		//-

		if (bGui_User) drawImGui_User();
	}
	guiManager.end();
}
#endif

//----

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::startup()
{
	ofLogNotice(__FUNCTION__);

	bCallbacksEnable = true;
	
	loadSettings();

	//-

	bActive = true;

	// set gui position after window setup/resizing
	windowResized(wWindow, hWindow);
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::update(ofEventArgs & args) {

	// autosave
	if (bAutoSave && ofGetElapsedTimeMillis() - timerLastAutosave > timeToAutosave)
	{
		bCallbacksEnable = false;
		saveSettings();
		bCallbacksEnable = true;

		ofLogVerbose(__FUNCTION__) << "AutoSaved DONE";
		timerLastAutosave = ofGetElapsedTimeMillis();
	}
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::draw(ofEventArgs & args)
{
	if (!bGui) return;

	// edit mode
	if (appModeIndex == 1)
	{
	}

	// gui
	if (bGui_Advanced) drawGui();

#ifdef USE_IM_GUI
	// ImGui
	drawImGui();
#endif
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::update() {
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::draw() {
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::drawGui() {
	gui_Advanced.draw();
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::windowResized(int w, int h)
{
	wWindow = w;
	hWindow = h;

	rectWindow = ofGetCurrentViewport();

	//-

	// user gui deopending on window dimensions
	//gui_Advanced.setPosition(wWindow * 0.5 - 200, hWindow - 200);
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::exit()
{
	saveSettings();
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setActive(bool b)
{
	ofLogNotice(__FUNCTION__) << b;

	// Disables all keys and mouse interaction listeners from the addon

	bActive = b;
	if (!b)
	{
		removeKeysListeners();
		removeMouseListeners();
	}
	else
	{
		addKeysListeners();
		addMouseListeners();
	}
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setGuiVisible(bool b)
{
	ofLogNotice(__FUNCTION__);
	bGui = b;
}

// keys
//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::keyPressed(ofKeyEventArgs &eventArgs)
{
	// disabler for all keys. (independent from bActive)
	if (!bKeys) return;

	const int &key = eventArgs.key;
	ofLogNotice(__FUNCTION__) << (char)key << " [" << key << "]";

	// modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	bool debug = false;
	if (debug)
	{
		ofLogNotice(__FUNCTION__) << "mod_COMMAND: " << (mod_COMMAND ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_CONTROL: " << (mod_CONTROL ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_ALT: " << (mod_ALT ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_SHIFT: " << (mod_SHIFT ? "ON" : "OFF");
	}

	//-

	if (0) {}

	//// custom
	//else if (key == ' ')
	//{
	//}

	//// custom with modifiers
	//if (key == OF_KEY_UP && mod_ALT)
	//{
	//	ofLogNotice(__FUNCTION__) << "";
	//}
	//else if (key == OF_KEY_UP)
	//{
	//	ofLogNotice(__FUNCTION__) << "";
	//}

	// general
	//if (key == keyAppMode)
	//{
	//	int i = appModeIndex;
	//	i++;
	//	appModeIndex = i % OFX_APP_MODES_AMOUNT;
	//}

	else if (key == 'g')
	{
		bGui = !bGui;
	}

	else if (key == 'G')
	{
		bGui_Advanced = !bGui_Advanced;
	}
	else if (key == 'h')
	{
		bHelp = !bHelp;
	}
	else if (key == 'd')
	{
		bDebug = !bDebug;
	}

	//--

	// key enabler
	if (key == 'k')
	{
		bKeys = !bKeys;
		ofLogNotice(__FUNCTION__) << "KEYS: " << (bKeys ? "ON" : "OFF");

		if (!bKeys)
		{
			ofLogNotice(__FUNCTION__) << "ALL KEYS DISABLED. PRESS 'k' TO ENABLE GAIN!";
		}
		else
		{
			ofLogNotice(__FUNCTION__) << "KEYS ENABLED BACK";
		}
	}
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::keyReleased(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;
	ofLogNotice(__FUNCTION__) << (char)key << " [" << key << "]";

	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::addKeysListeners()
{
	ofLogNotice(__FUNCTION__);
	ofAddListener(ofEvents().keyPressed, this, &ofxAddonTemplate_ImGui::keyPressed);
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::removeKeysListeners()
{
	ofLogNotice(__FUNCTION__);
	ofRemoveListener(ofEvents().keyPressed, this, &ofxAddonTemplate_ImGui::keyPressed);
}

// mouse
//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::mouseDragged(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	ofLogVerbose(__FUNCTION__) << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::mousePressed(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	ofLogVerbose(__FUNCTION__) << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::mouseReleased(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	ofLogVerbose(__FUNCTION__) << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::addMouseListeners()
{
	ofAddListener(ofEvents().mouseDragged, this, &ofxAddonTemplate_ImGui::mouseDragged);
	ofAddListener(ofEvents().mousePressed, this, &ofxAddonTemplate_ImGui::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxAddonTemplate_ImGui::mouseReleased);
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::removeMouseListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxAddonTemplate_ImGui::keyPressed);
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::Changed_params(ofAbstractParameter &e)
{
	if (!bCallbacksEnable) return;

	string name = e.getName();
	ofLogNotice(__FUNCTION__) << name << " : " << e;

	if (0) {}

	//// filter params
	//else if (name == bClear.getName() && bClear.get())
	//{
	//	bClear = false;
	//}
	//else if (name == bRun.getName() && bRun.get())
	//{
	//	bRun = false;
	//}
	//else if (name == bInitialize.getName() && bInitialize.get())
	//{
	//	bInitialize = false;
	//	fileList = "";
	//}
}

// addon engine params
//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::Changed_params_Addon(ofAbstractParameter &e)
{
	if (!bCallbacksEnable) return;

	string name = e.getName();

	// exclude debugs
	if (name != "exclude"
		&& name != "exclude")
	{
		ofLogNotice(__FUNCTION__) << name << " : " << e;
	}

	// params
	if (name == "")
	{
	}
}

// addon control (internal) params
//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::Changed_params_Control(ofAbstractParameter &e)
{
	if (!bCallbacksEnable) return;

	string name = e.getName();

	// exclude debugs
	if (name != "exclude"
		&& name != "exclude")
	{
		ofLogNotice(__FUNCTION__) << name << " : " << e;
	}

	if (0) {}

	else if (name == "")
	{
	}

	else if (name == positionGui.getName())
	{
		gui_Advanced.setPosition(positionGui.get().x, positionGui.get().y);
	}
	else if (name == bActive.getName())
	{
		setActive(bActive);
	}
	else if (name == bGui.getName())
	{
	}
	else if (name == bHelp.getName())
	{
	}
	else if (name == bDebug.getName())
	{
	}

	// app modes
	//else if (name == "APP MODE")
	//{
	//	switch (appModeIndex)
	//	{
	//	case 0:
	//		appModeName = "RUN";
	//		//setActive(false);
	//		break;
	//	case 1:
	//		appModeName = "EDIT";
	//		//setActive(true);
	//		break;
	//	default:
	//		appModeName = "UNKNOWN";
	//		break;
	//	}
	//}
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setKeyAppMode(int k)
{
	keyAppMode = k;
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setLogLevel(ofLogLevel level)
{
	ofSetLogLevel(__FUNCTION__, level);
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::dragEvent(ofDragInfo info) {
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setPathGlobal(string s)//must call before setup. disabled by default
{
	path_GLOBAL = s;

	ofxSurfingHelpers::CheckFolder(path_GLOBAL);
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::saveSettings() {
	ofLogVerbose(__FUNCTION__);

	// get gui position before save
	positionGui = glm::vec2(gui_Advanced.getPosition());

	ofxSurfingHelpers::saveGroup(params_Control, path_GLOBAL + path_Params_Control);
	ofxSurfingHelpers::saveGroup(params_AppSettings, path_GLOBAL + path_Params_AppSettings);
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::loadSettings() {
	ofLogVerbose(__FUNCTION__);

	ofxSurfingHelpers::CheckFolder(path_GLOBAL);

	// settings
	ofxSurfingHelpers::loadGroup(params_Control, path_GLOBAL + path_Params_Control);
	ofxSurfingHelpers::loadGroup(params_AppSettings, path_GLOBAL + path_Params_AppSettings);
}
