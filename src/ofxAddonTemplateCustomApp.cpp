#include "ofxAddonTemplateCustomApp.h"

//--------------------------------------------------------------
ofxAddonTemplateCustomApp::ofxAddonTemplateCustomApp()
{
	// path for settings
	setPathGlobal("ofxAddonTemplateCustomApp/");
	path_Params_AppSettings = "ofxAddonTemplateCustomApp_AppSettings.xml";
	path_Params_Control = "Params_Control.xml";

	bCallbacksEnable = true;
	setActive(true); // adds key and mouse listeners

	ofAddListener(ofEvents().update, this, &ofxAddonTemplateCustomApp::update);
	ofAddListener(ofEvents().draw, this, &ofxAddonTemplateCustomApp::draw);
	//ofAddListener(ofEvents().draw, this, &ofxAddonTemplateCustomApp::draw, OF_EVENT_ORDER_AFTER_APP);

	setup();
}

//--------------------------------------------------------------
ofxAddonTemplateCustomApp::~ofxAddonTemplateCustomApp()
{
	setActive(false); // removes keys and mouse listeners

	// removes params callbacks listeners
	ofRemoveListener(params.parameterChangedE(), this, &ofxAddonTemplateCustomApp::Changed_params);
	ofRemoveListener(params_Control.parameterChangedE(), this, &ofxAddonTemplateCustomApp::Changed_params_Control);
	ofRemoveListener(params_Addon.parameterChangedE(), this, &ofxAddonTemplateCustomApp::Changed_params_Addon);

	// removes update/draw callbacks
	ofRemoveListener(ofEvents().update, this, &ofxAddonTemplateCustomApp::update);
	ofRemoveListener(ofEvents().draw, this, &ofxAddonTemplateCustomApp::draw);
	//ofRemoveListener(ofEvents().draw, this, &ofxAddonTemplateCustomApp::draw, OF_EVENT_ORDER_AFTER_APP);

	exit();
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::setup()
{
	// log mode
	ofSetLogLevel("ofxAddonTemplateCustomApp", OF_LOG_NOTICE);

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
	ofAddListener(params_Addon.parameterChangedE(), this, &ofxAddonTemplateCustomApp::Changed_params_Addon);

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
	params_Control.add(positionGui);

	//-

	// params App settings
	params_AppSettings.setName("AppSettings");
	params_AppSettings.add(bGui);
	params_AppSettings.add(bHelp);
	params_AppSettings.add(bKeys);

	//-

	// exclude for settings
	bDebug.setSerializable(false);
	appModeName.setSerializable(false);

	//-

	// callback
	ofAddListener(params_Control.parameterChangedE(), this, &ofxAddonTemplateCustomApp::Changed_params_Control);

	//-

	// all params
	params.setName("All Params");
	params.add(params_AppSettings);
	params.add(params_Addon);
	params.add(params_Control);

	// callback
	ofAddListener(params.parameterChangedE(), this, &ofxAddonTemplateCustomApp::Changed_params);

	//--

	// gui
	setupGui();

	//--

	// startup
	startup();

	//--
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::setupGui()
{
	ofLogNotice(__FUNCTION__);

	// theme
	ofxSurfingHelpers::setThemeDarkMini_ofxGui();

	//-

	// setup gui
	{
		gui_Control.setup("ofxAddonTemplateCustomApp");
		gui_Control.add(params);//add control (internal) and addon params
		gui_Control.setPosition(positionGui.get().x, positionGui.get().y);
		//gui_Control.setPosition(ofGetWidth() - 210, 20);

		// collapse groups
		auto &g0 = gui_Control.getGroup(params.getName());//1st level
		auto &g2 = g0.getGroup(params_Addon.getName());//2nd level
		auto &g3 = g0.getGroup(params_Control.getName());//2nd level
		auto &g31 = g3.getGroup(positionGui.getName());//3nd level
		//g0.minimize();
		g2.minimize();
		g3.minimize();
		g31.minimize();
	}
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::startup()
{
	ofLogNotice(__FUNCTION__);

	bCallbacksEnable = true;

	//-

	// settings
	ofxSurfingHelpers::loadGroup(params_Control, path_GLOBAL + path_Params_Control);
	ofxSurfingHelpers::loadGroup(params_AppSettings, path_GLOBAL + path_Params_AppSettings);

	bActive = true;

	// set gui position after window setup/resizing
	windowResized(wWindow, hWindow);

	ofxSurfingHelpers::CheckFolder(path_GLOBAL);
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::update(ofEventArgs & args) {

	// autosave
	if (bAutoSave && ofGetElapsedTimeMillis() - timerLastAutosave > timeToAutosave)
	{
		bCallbacksEnable = false;
		// get gui position before save
		positionGui = glm::vec2(gui_Control.getPosition());
		ofxSurfingHelpers::saveGroup(params_Control, path_GLOBAL + path_Params_Control);
		bCallbacksEnable = true;

		timerLastAutosave = ofGetElapsedTimeMillis();
		ofLogVerbose(__FUNCTION__) << "AutoSaved DONE";
	}
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::draw(ofEventArgs & args)
{
	// edit mode
	if (appModeIndex == 1)
	{
	}

	// gui
	if (bGui) drawGui();
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::update() {
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::draw() {
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::drawGui() {
	gui_Control.draw();
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::windowResized(int w, int h)
{
	wWindow = w;
	hWindow = h;

	rectWindow = ofGetCurrentViewport();

	//-

	// user gui deopending on window dimensions
	//gui_Control.setPosition(wWindow * 0.5 - 200, hWindow - 200);
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::exit()
{
	// get gui position before save
	positionGui = glm::vec2(gui_Control.getPosition());

	ofxSurfingHelpers::saveGroup(params_Control, path_GLOBAL + path_Params_Control);
	ofxSurfingHelpers::saveGroup(params_AppSettings, path_GLOBAL + path_Params_AppSettings);
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::setActive(bool b)
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
void ofxAddonTemplateCustomApp::setGuiVisible(bool b)
{
	ofLogNotice(__FUNCTION__);
	bGui = b;
}

// keys
//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::keyPressed(ofKeyEventArgs &eventArgs)
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
void ofxAddonTemplateCustomApp::keyReleased(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;
	ofLogNotice(__FUNCTION__) << (char)key << " [" << key << "]";

	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::addKeysListeners()
{
	ofLogNotice(__FUNCTION__);
	ofAddListener(ofEvents().keyPressed, this, &ofxAddonTemplateCustomApp::keyPressed);
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::removeKeysListeners()
{
	ofLogNotice(__FUNCTION__);
	ofRemoveListener(ofEvents().keyPressed, this, &ofxAddonTemplateCustomApp::keyPressed);
}

// mouse
//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::mouseDragged(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	ofLogVerbose(__FUNCTION__) << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::mousePressed(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	ofLogVerbose(__FUNCTION__) << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::mouseReleased(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	ofLogVerbose(__FUNCTION__) << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::addMouseListeners()
{
	ofAddListener(ofEvents().mouseDragged, this, &ofxAddonTemplateCustomApp::mouseDragged);
	ofAddListener(ofEvents().mousePressed, this, &ofxAddonTemplateCustomApp::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxAddonTemplateCustomApp::mouseReleased);
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::removeMouseListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxAddonTemplateCustomApp::keyPressed);
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::Changed_params(ofAbstractParameter &e)
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
void ofxAddonTemplateCustomApp::Changed_params_Addon(ofAbstractParameter &e)
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
void ofxAddonTemplateCustomApp::Changed_params_Control(ofAbstractParameter &e)
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

	else if (name == positionGui.getName())
	{
		gui_Control.setPosition(positionGui.get().x, positionGui.get().y);
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
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::setKeyAppMode(int k)
{
	keyAppMode = k;
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::setLogLevel(ofLogLevel level)
{
	ofSetLogLevel(__FUNCTION__, level);
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::setPathGlobal(string s)//must call before setup. disabled by default
{
	path_GLOBAL = s;

	ofxSurfingHelpers::CheckFolder(path_GLOBAL);
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::dragEvent(ofDragInfo info) {
}
