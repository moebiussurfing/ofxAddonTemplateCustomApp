#include "ofxAddonTemplate_ImGui.h"

//--------------------------------------------------------------
ofxAddonTemplate_ImGui::ofxAddonTemplate_ImGui()
{
	// path for settings
	setPathGlobal("ofxAddonTemplate_ImGui/");
	path_Params_AppSettings = "ofxAddonTemplate_ImGui_AppSettings.xml";
	path_Params_Control = "Params_Control.xml";

	bCallbacksEnable = false; // disable until ends setup
	setActive(true); // adds key and mouse listeners

	ofAddListener(ofEvents().update, this, &ofxAddonTemplate_ImGui::update);
	ofAddListener(ofEvents().draw, this, &ofxAddonTemplate_ImGui::draw);
	//ofAddListener(ofEvents().draw, this, &ofxAddonTemplate_ImGui::draw, OF_EVENT_ORDER_AFTER_APP);

	//-

	// Auto called on creation
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

	//-

	// Auto called on destruction
	exit();
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setup()
{
	// Log level
	setLogLevel(OF_LOG_NOTICE);
	//setLogLevel(OF_LOG_VERBOSE);

	//--

	// window
	wWindow = ofGetWidth();
	hWindow = ofGetHeight();
	rectWindow = ofGetCurrentViewport();
	
	//--
	
	setupParameters();

	//--

	// gui
	setupGui();

	//--

	// startup
	startup();
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setupParameters()
{
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

	// addon control

	// params

	bActive.set("Active", true);
	bKeys.set("Keys", true);
	bHelp.set("Help", false);
	bDebug.set("Debug", true);
	bAutoSave.set("AutoSave", false);

	// app modes
	appModeNames.clear();
	//appModeNames.push_back("Undefined");
	//appModeNames.push_back("Inactive");
	appModeNames.push_back("Edit");
	appModeNames.push_back("Live");
	appModeIndex.set("App Mode", 0, 0, appModeNames.size() - 1);
	//appModeIndex.set("App Mode", 0, 0, OFX_APP_MODES_AMOUNT - 1);
	appModeName.set("App Mode Name", "");

	bGui.set("Gui", true);
	bGui_User.set("Gui User", true);
	bGui_Advanced.set("Gui Advanced", true);
	positionGuiAdvanced.set("Gui Position",
		glm::vec2(wWindow * 0.5, hWindow * 0.5),
		glm::vec2(0, 0),
		glm::vec2(wWindow, hWindow)
	);

	// params control
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
	params_Control.add(positionGuiAdvanced);

	//-

	// params App settings (to save/load app state)
	params_AppSettings.setName("AppSettings");
	params_AppSettings.add(bGui);
	params_AppSettings.add(bGui_User);
	params_AppSettings.add(bGui_Advanced);
	params_AppSettings.add(bHelp);
	params_AppSettings.add(bKeys);

	//-

	// exclude for settings
	bDebug.setSerializable(false);
	bHelp.setSerializable(false);
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
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::startup()
{
	ofLogNotice(__FUNCTION__);
	bCallbacksEnable = true; // -> Just now we enable the callbacks!. That's to avoid some possible crashes.

	loadSettings();

	//-

	bActive = true;

	// Refresh window size to update some layouts or fbo sizes ...etc
	// set gui position after window setup/resizing
	windowResized(wWindow, hWindow);
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setupHelp()
{
	strHelpInfo = "ofxAddonTemplate_ImGui\n\n";
	strHelpInfo += "h : Help Info\n";
	strHelpInfo += "g : Gui\n";
	strHelpInfo += "G : Gui Advanced\n";
	strHelpInfo += "d : Debug\n";
	strHelpInfo += "k : Enable Keys\n";
	//strHelpInfo += "g : Show Gui\n";
	//strHelpInfo += "\n";

	// help box
	helpTextBoxWidget.setPath(path_Global);
	helpTextBoxWidget.setup();
	helpTextBoxWidget.setTheme(bThemeDarkOrLight);
}

//-

// Gui

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
		gui_Advanced.setPosition(positionGuiAdvanced.get().x, positionGuiAdvanced.get().y);
		//gui_Advanced.setPosition(ofGetWidth() - 210, 20);

		// collapse groups
		auto &g0 = gui_Advanced.getGroup(params.getName());//1st level
		auto &g2 = g0.getGroup(params_Addon.getName());//2nd level
		auto &g3 = g0.getGroup(params_Control.getName());//2nd level
		auto &g31 = g3.getGroup(positionGuiAdvanced.getName());//3nd level
		//g0.minimize();
		g2.minimize();
		g3.minimize();
		g31.minimize();
	}

	//--

	// setup ImGui
#ifdef USE_IM_GUI
	setupImGui();
#endif

	//--

	// Help info
	setupHelp();
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

	guiManager.setName("ofxAddonTemplate_ImGui");
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
		guiManager.addWindowSpecial("Audio Window", true);
		guiManager.addWindowSpecial("Main Window", true);
		guiManager.addWindowSpecial("Another"); // but without advanced mode
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

	ImGui::Begin("A RAW ImGui Window out of the Layout Engine");
	ImGui::Text("HelloRaw");
	// Select AppMode
	{
		int i = appModeIndex.get();
		float _w100 = ofxImGuiSurfing::getWidgetsWidth(1);

		//ImGui::SetNextItemWidth(_w100 - 20);
		//if (ofxImGui::VectorCombo(" ", &i, appModeNames))

		ImGui::SetNextItemWidth(_w100 - 100);
		if (ofxImGui::VectorCombo("AppMode", &i, appModeNames))

		{
			ofLogNotice(__FUNCTION__) << "Combo Index: " << ofToString(i);
			appModeIndex.set(i);
		}
	}
	ImGui::TextWrapped("HelloRaw, HelloRaw, HelloRaw, HelloRaw, HelloRaw, HelloRaw");
	ImGui::End();

	//-

#ifdef USE_LAYOUT_ENGINE
	//if (0)
	{
		if (guiManager.beginWindowSpecial(0)) // -> This is our helpers to render windows
		{
			ImGui::Text("Hello0, left!");
			ImGui::Text("Hello0, left!");

			guiManager.endWindowSpecial(); // -> Must be called inside the if / beginWindow brackets!
		}

		if (guiManager.beginWindowSpecial(1))
		{
			ImGui::Text("Hello1, down!");
			ImGui::Text("Hello1, down!");

			guiManager.endWindowSpecial();
		}

		if (guiManager.beginWindowSpecial(2))
		{
			ImGui::Text("Hello2");
			ImGui::Text("Hello2");

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

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::drawHelp()
{
	helpTextBoxWidget.setText(strHelpInfo);
	helpTextBoxWidget.draw();
}

//----

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
	// Different app behaviors
	// edit mode
	if (appModeIndex == OF_APP_MODE_EDIT)
	{
	}

	// gui
	if (!bGui) return;
	drawGui();
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::update() {
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::draw() {
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::drawGui() {

	// gui advanced
	if (bGui_Advanced) gui_Advanced.draw();

	// ImGui
#ifdef USE_IM_GUI
	if (bGui_User) drawImGui();
#endif

	// Help info box text: 
	if (bHelp) drawHelp();
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

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setGuiVisibleToggle()
{
	ofLogNotice(__FUNCTION__);
	bGui = !bGui;
}

// keys
//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::keyPressed(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;

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

	//--

	// disabler for all keys. (independent from bActive)
	if (!bKeys) return;

	ofLogNotice(__FUNCTION__) << (char)key << " [" << key << "]";

	// modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	static bool debug = false;
	if (debug)
	{
		ofLogNotice(__FUNCTION__) << "mod_COMMAND: " << (mod_COMMAND ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_CONTROL: " << (mod_CONTROL ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_ALT: " << (mod_ALT ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_SHIFT: " << (mod_SHIFT ? "ON" : "OFF");
	}

	//-

	//// custom
	//if (0) {}
	//else if (key == ' ')
	//{
	//}

	// custom with modifiers
	if (!mod_COMMAND && !mod_CONTROL && !mod_ALT)
	{
		if (0) {}

		else if (key == OF_KEY_UP)
		{
			ofLogNotice(__FUNCTION__) << "Up";
		}
		else if (key == OF_KEY_UP)
		{
			ofLogNotice(__FUNCTION__) << "Down";
		}
	}

	//-

	if (0) {}

	// app mode (Tab by default)
	else if (key == keyAppMode)
	{
		// cycled
		int i = appModeIndex.get();
		i++;
		appModeIndex.set(i % (appModeIndex.getMax() + 1));
	}

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

//-

// Callbacks

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::Changed_params(ofAbstractParameter &e)
{
	if (!bCallbacksEnable) return;

	string name = e.getName();
	ofLogNotice(__FUNCTION__) << name << " : " << e;

	if (0) {}
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

	// filter params
	if (0) {}

	else if (name == "")
	{
	}
	//// filter params
	//else if (name == Addon_Float.getName())
	//{
	//	Addon_Float = Addon_Float;
	//}
	//else if (name == Addon_Active.getName()))
	//{
	//	Addon_Active = Addon_Active;
	//}
	//else if (name == bInitialize.getName() && bInitialize.get())
	//{
	//	bInitialize = false;
	//}
}

// addon control params
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
		if (bDebug) {
			setLogLevel(OF_LOG_VERBOSE);
		}
		else {
			setLogLevel(OF_LOG_NOTICE);
		}
	}
	else if (name == positionGuiAdvanced.getName())
	{
		gui_Advanced.setPosition(positionGuiAdvanced.get().x, positionGuiAdvanced.get().y);
	}

	else if (name == appModeIndex.getName())
	{
		appModeIndex.setWithoutEventNotifications(ofClamp(appModeIndex.get(), appModeIndex.getMin(), appModeIndex.getMax()));
		appModeName = appModeNames[appModeIndex.get()];
		appMode = AppMode(appModeIndex.get());

		ofLogNotice(__FUNCTION__) << "AppMode: " << appModeIndex.get() << ":" << appModeName.get();

		// app modes
		//else if (name == "APP MODE")
		//{
		//	switch (appModeIndex)
		//	{
		//	case 0:
		//		//setActive(false);
		//		break;
		//	case 1:
		//		//setActive(true);
		//		break;
		//	default:
		//		break;
		//	}
		//}
	}
}

//--

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setKeyAppMode(int k)
{
	keyAppMode = k;
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setLogLevel(ofLogLevel logLevel)
{
	ofLogWarning(__FUNCTION__) << ofLogLevel(logLevel);
	ofSetLogLevel("ofxAddonTemplate_ImGui", logLevel);
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::dragEvent(ofDragInfo info) {
}

//-

// Settings

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::setPathGlobal(string s)//must call before setup. disabled by default
{
	path_Global = s;

	ofxSurfingHelpers::CheckFolder(path_Global);
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::saveSettings() {
	ofLogVerbose(__FUNCTION__);

	// get gui position before save
	positionGuiAdvanced = glm::vec2(gui_Advanced.getPosition());

	ofxSurfingHelpers::saveGroup(params_Control, path_Global + path_Params_Control);
	ofxSurfingHelpers::saveGroup(params_AppSettings, path_Global + path_Params_AppSettings);
}

//--------------------------------------------------------------
void ofxAddonTemplate_ImGui::loadSettings() {
	ofLogVerbose(__FUNCTION__);

	ofxSurfingHelpers::CheckFolder(path_Global);

	// settings
	ofxSurfingHelpers::loadGroup(params_Control, path_Global + path_Params_Control);
	ofxSurfingHelpers::loadGroup(params_AppSettings, path_Global + path_Params_AppSettings);
}
