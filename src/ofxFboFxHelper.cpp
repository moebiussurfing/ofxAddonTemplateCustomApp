#include "ofxFboFxHelper.h"

#pragma mark - OF

//--------------------------------------------------------------
ofxFboFxHelper::ofxFboFxHelper()
{
	//path for settings
	setPathGlobal("ofxFboFxHelper/");
	path_Params_Control = "params_Control.xml";

	setActive(true);//add key and mouse listeners
}

//--------------------------------------------------------------
void ofxFboFxHelper::setup()
{
	//log mode
	ofSetLogLevel("ofxFboFxHelper", OF_LOG_NOTICE);
	//ofSetLogLevel("ofxFboFxHelper", OF_LOG_SILENT);

	//--

	//window
	screenW = ofGetWidth();
	screenH = ofGetHeight();

	//--

	//params control (addon variables)
	Addon_Active.set("ADDON ACTIVE", true);
	Addon_Float.set("ADDON float", 0, 0, 1);
	//group
	params_Addon.setName("ADDON");
	params_Addon.add(Addon_Active);
	params_Addon.add(Addon_Float);

	//callback
	ofAddListener(params_Addon.parameterChangedE(), this, &ofxFboFxHelper::Changed_params_Addon);

	//-

	//addon control(internal)

	//params
	MODE_Active.set("ACTIVE", true);
	ENABLE_keys.set("KEYS", true);
	SHOW_Help.set("HELP", false);
	MODE_App.set("APP MODE", 0, 0, NUM_MODES_APP - 1);
	MODE_App_Name.set("", "");
	MODE_App_Name.setSerializable(false);
	ENABLE_Debug.set("DEBUG", true);
	ENABLE_AutoSave.set("AUTO SAVE", true);
	SHOW_Gui.set("GUI", true);
	Gui_Position.set("GUI POSITION",
		glm::vec2(screenW * 0.5, screenH * 0.5),
		glm::vec2(0, 0),
		glm::vec2(screenW, screenH)
	);

	//params control (internal)
	params_Control.setName("INTERNAL");
	params_Control.add(MODE_App);
	params_Control.add(MODE_App_Name);
	params_Control.add(MODE_Active);
	params_Control.add(ENABLE_keys);
	params_Control.add(SHOW_Help);
	params_Control.add(ENABLE_Debug);
	params_Control.add(ENABLE_AutoSave);
	params_Control.add(SHOW_Gui);
	params_Control.add(Gui_Position);

	//callback
	ofAddListener(params_Control.parameterChangedE(), this, &ofxFboFxHelper::Changed_params_Control);

	//-

	//all params
	params.setName("ALL PARAMS");
	params.add(params_Addon);
	params.add(params_Control);

	//callback
	ofAddListener(params.parameterChangedE(), this, &ofxFboFxHelper::Changed_params);

	//--

	//gui

	//theme
	string str = "fonts/overpass-mono-bold.otf";
	ofxGuiSetFont(path_GLOBAL + str, 9);
	ofxGuiSetDefaultHeight(20);
	ofxGuiSetBorderColor(32);
	ofxGuiSetFillColor(ofColor(48));
	ofxGuiSetTextColor(ofColor::white);
	ofxGuiSetHeaderColor(ofColor(24));
	//ofxGuiSetBackgroundColor(ofColor::black);

	//setup gui
	gui_Control.setup("ofxFboFxHelper");
	gui_Control.add(params);//add control (internal) and addon params

	//collapse groups
	auto &g0 = gui_Control.getGroup("ALL PARAMS");//1st level
	auto &g2 = g0.getGroup("ADDON");//2nd level
	auto &g3 = g0.getGroup("INTERNAL");//2nd level
	auto &g31 = g3.getGroup("GUI POSITION");//3nd level
	//g0.minimize();
	//g2.minimize();
	g3.minimize();
	g31.minimize();

	//-

	//settings
	DISABLE_Callbacks = true;
	loadParams(params_Control, path_GLOBAL + path_Params_Control);

	

	//--

	//startup

	ofLogNotice("ofxFboFxHelper") << "STARTUP INIT";

	DISABLE_Callbacks = false;
	MODE_Active = true;

	//set gui position after window setup/resizing
	windowResized(screenW, screenH);

	//-
}

//--------------------------------------------------------------
void ofxFboFxHelper::update()
{
	//--

//autosave
//ENABLE_AutoSave = false;
	if (ENABLE_AutoSave && ofGetElapsedTimeMillis() - timerLast_Autosave > timeToAutosave)
	{
		DISABLE_Callbacks = true;
		//get gui position before save
		Gui_Position = glm::vec2(gui_Control.getPosition());
		saveParams(params_Control, path_GLOBAL + path_Params_Control);
		DISABLE_Callbacks = false;

		timerLast_Autosave = ofGetElapsedTimeMillis();
		//ofLogNotice("ofApp") << "Autosaved DONE";
	}
}

//--------------------------------------------------------------
void ofxFboFxHelper::draw()
{
	if (SHOW_Gui)
	{
		//edit mode
		if (MODE_App == 1)
		{
			//controlPoints.draw();
		}

		gui_Control.draw();
	}
}

//--------------------------------------------------------------
void ofxFboFxHelper::exit()
{

}

//--------------------------------------------------------------
ofxFboFxHelper::~ofxFboFxHelper()
{
	setActive(false);//remove keys and mouse listeners

	//remove params callbacks listeners
	ofRemoveListener(params.parameterChangedE(), this, &ofxFboFxHelper::Changed_params);
	ofRemoveListener(params_Control.parameterChangedE(), this, &ofxFboFxHelper::Changed_params_Control);
	ofRemoveListener(params_Addon.parameterChangedE(), this, &ofxFboFxHelper::Changed_params_Addon);

	//get gui position before save
	Gui_Position = glm::vec2(gui_Control.getPosition());

	saveParams(params_Control, path_GLOBAL + path_Params_Control);

	//exit();
}

//--------------------------------------------------------------
void ofxFboFxHelper::setLogLevel(ofLogLevel level)
{
	ofSetLogLevel("ofxFboFxHelper", level);
}


#pragma mark - OF LISTENERS

//--------------------------------------------------------------
void ofxFboFxHelper::windowResized(int w, int h)
{
	screenW = w;
	screenH = h;

	//-

	//user gui deopending on window dimensions
	//gui_Control.setPosition(screenW * 0.5 - 200, screenH - 200);
}

//keys
//--------------------------------------------------------------
void ofxFboFxHelper::keyPressed(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;
	ofLogNotice("ofxFboFxHelper") << "keyPressed: " << (char)key << " [" << key << "]";

	//modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	bool debug = false;
	if (debug)
	{
		ofLogNotice("ofxFboFxHelper") << "mod_COMMAND: " << (mod_COMMAND ? "ON" : "OFF");
		ofLogNotice("ofxFboFxHelper") << "mod_CONTROL: " << (mod_CONTROL ? "ON" : "OFF");
		ofLogNotice("ofxFboFxHelper") << "mod_ALT: " << (mod_ALT ? "ON" : "OFF");
		ofLogNotice("ofxFboFxHelper") << "mod_SHIFT: " << (mod_SHIFT ? "ON" : "OFF");
	}

	//-

	//disabler for all keys. (independent from MODE_Active)
	if (ENABLE_keys)
	{
		//custom
		if (key == ' ')
		{
		}
		else if (key == ' ')
		{
		}

		//custom with modifiers
		if (key == OF_KEY_UP && mod_ALT)
		{
			ofLogNotice("ofxFboFxHelper") << "";
		}
		else if (key == OF_KEY_UP)
		{
			ofLogNotice("ofxFboFxHelper") << "";
		}

		//general
		if (key == key_MODE_App)
		{
			int i = MODE_App;
			i++;
			MODE_App = i % NUM_MODES_APP;


		}
		else if (key == 'g')
		{
			SHOW_Gui = !SHOW_Gui;
		}
		else if (key == 'h')
		{
			SHOW_Help = !SHOW_Help;
		}
		else if (key == 'd')
		{
			ENABLE_Debug = !ENABLE_Debug;
		}
	}

	//--

	//key enabler
	if (key == 'k')
	{
		ENABLE_keys = !ENABLE_keys;
		ofLogNotice("ofxFboFxHelper") << "KEYS: " << (ENABLE_keys ? "ON" : "OFF");

		if (!ENABLE_keys)
		{
			ofLogNotice("ofxFboFxHelper") << "ALL KEYS DISABLED. PRESS 'k' TO ENABLE GAIN!";
		}
		else
		{
			ofLogNotice("ofxFboFxHelper") << "KEYS ENABLED BACK";
		}
	}
}

//--------------------------------------------------------------
void ofxFboFxHelper::keyReleased(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;
	ofLogNotice("ofxFboFxHelper") << "keyPressed: " << (char)key << " [" << key << "]";

	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);
}

//--------------------------------------------------------------
void ofxFboFxHelper::addKeysListeners()
{
	ofAddListener(ofEvents().keyPressed, this, &ofxFboFxHelper::keyPressed);
}

//--------------------------------------------------------------
void ofxFboFxHelper::removeKeysListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxFboFxHelper::keyPressed);
}

//mouse
//--------------------------------------------------------------
void ofxFboFxHelper::mouseDragged(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//ofLogNotice("ofxFboFxHelper") << "mouseDragged " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxFboFxHelper::mousePressed(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//ofLogNotice("ofxFboFxHelper") << "mousePressed " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxFboFxHelper::mouseReleased(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//ofLogNotice("ofxFboFxHelper") << "mouseReleased " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxFboFxHelper::addMouseListeners()
{
	ofAddListener(ofEvents().mouseDragged, this, &ofxFboFxHelper::mouseDragged);
	ofAddListener(ofEvents().mousePressed, this, &ofxFboFxHelper::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxFboFxHelper::mouseReleased);
}

//--------------------------------------------------------------
void ofxFboFxHelper::removeMouseListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxFboFxHelper::keyPressed);
}


#pragma mark - API

//--------------------------------------------------------------
void ofxFboFxHelper::setActive(bool b)
{
	//* disables all keys and mouse interaction listeners from the addon

	MODE_Active = b;
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
void ofxFboFxHelper::setGuiVisible(bool b)
{
	SHOW_Gui = b;
}


#pragma mark - CALLBACKS

//all params
//--------------------------------------------------------------
void ofxFboFxHelper::Changed_params(ofAbstractParameter &e)
{
	//if (!DISABLE_Callbacks)
	//{
	//	string name = e.getName();

	//	//exclude debugs
	//	if (name != "exclude"
	//		&& name != "exclude")
	//	{
	//		ofLogNotice("ofxFboFxHelper") << "Changed_params: " << name << " : " << e;

	//	}

	//	//params
	//	if (name == "")
	//	{
	//	}
	//}
}

//addon engine params
//--------------------------------------------------------------
void ofxFboFxHelper::Changed_params_Addon(ofAbstractParameter &e)
{
	if (!DISABLE_Callbacks)
	{
		string name = e.getName();

		//exclude debugs
		if (name != "exclude"
			&& name != "exclude")
		{
			ofLogNotice("ofxFboFxHelper") << "Changed_params_Addon: " << name << " : " << e;

		}

		//params
		if (name == "")
		{
		}
	}
}

//addon control (internal) params
//--------------------------------------------------------------
void ofxFboFxHelper::Changed_params_Control(ofAbstractParameter &e)
{
	if (!DISABLE_Callbacks)
	{
		string name = e.getName();

		//exclude debugs
		if (name != "exclude"
			&& name != "exclude")
		{
			ofLogNotice("ofxFboFxHelper") << "Changed_params_Control: " << name << " : " << e;

		}

		//control params
		if (name == "")
		{
		}
		else if (name == "APP MODE")
		{
			switch (MODE_App)
			{
			case 0:
				MODE_App_Name = "RUN";
				//setActive(false);
				break;
			case 1:
				MODE_App_Name = "EDIT";
				//setActive(true);
				break;
			default:
				MODE_App_Name = "UNKNOWN";
				break;
			}
		}

		//filter params
		if (name == "GUI POSITION")
		{
			gui_Control.setPosition(Gui_Position.get().x, Gui_Position.get().y);
		}
		else if (name == "ACTIVE")
		{
			setActive(MODE_Active);
		}
		else if (name == "GUI")
		{
		}
		else if (name == "HELP")
		{
		}
		else if (name == "APP MODE")
		{
		}
		else if (name == "DEBUG")
		{
		}
	}

}

//--------------------------------------------------------------
void ofxFboFxHelper::setKey_MODE_App(int k)
{
	key_MODE_App = k;
}


#pragma mark - FILE SETTINGS

//--------------------------------------------------------------
void ofxFboFxHelper::setPathGlobal(string s)//must call before setup. disabled by default
{
	path_GLOBAL = s;
}

//--------------------------------------------------------------
void ofxFboFxHelper::loadParams(ofParameterGroup &g, string path)
{
	ofLogNotice("ofxFboFxHelper") << "loadParams: " << path;
	ofXml settings;
	settings.load(path);
	ofDeserialize(settings, g);
}

//--------------------------------------------------------------
void ofxFboFxHelper::saveParams(ofParameterGroup &g, string path)
{
	ofLogNotice("ofxFboFxHelper") << "saveParams: " << path;
	ofXml settings;
	ofSerialize(settings, g);
	settings.save(path);
}

