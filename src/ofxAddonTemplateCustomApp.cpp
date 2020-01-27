#include "ofxAddonTemplateCustomApp.h"

#pragma mark - OF

//--------------------------------------------------------------
ofxAddonTemplateCustomApp::ofxAddonTemplateCustomApp()
{
	//path for settings
	setPathGlobal("ofxAddonTemplateCustomApp/");
	path_Params_Control = "params_Control.xml";

	setActive(true);//add key and mouse listeners
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::setup()
{
	//log mode
	ofSetLogLevel("ofxAddonTemplateCustomApp", OF_LOG_NOTICE);
	//ofSetLogLevel("ofxAddonTemplateCustomApp", OF_LOG_SILENT);

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
	ofAddListener(params_Addon.parameterChangedE(), this, &ofxAddonTemplateCustomApp::Changed_params_Addon);

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
	params_Control.add(SHOW_Gui);
	params_Control.add(Gui_Position);

	//callback
	ofAddListener(params_Control.parameterChangedE(), this, &ofxAddonTemplateCustomApp::Changed_params_Control);

	//-

	//all params
	params.setName("ALL PARAMS");
	params.add(params_Addon);
	params.add(params_Control);

	//callback
	ofAddListener(params.parameterChangedE(), this, &ofxAddonTemplateCustomApp::Changed_params);

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
	gui_Control.setup("ofxAddonTemplateCustomApp");
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

	ofLogNotice("ofxAddonTemplateCustomApp") << "STARTUP INIT";

	DISABLE_Callbacks = false;
	MODE_Active = true;

	//set gui position after window setup/resizing
	windowResized(screenW, screenH);

	//-
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::update()
{
	//--

//autosave
//bAutosave = false;
	if (bAutosave && ofGetElapsedTimeMillis() - timerLast_Autosave > timeToAutosave)
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
void ofxAddonTemplateCustomApp::draw()
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
void ofxAddonTemplateCustomApp::exit()
{

}

//--------------------------------------------------------------
ofxAddonTemplateCustomApp::~ofxAddonTemplateCustomApp()
{
	setActive(false);//remove keys and mouse listeners

	//remove params callbacks listeners
	ofRemoveListener(params.parameterChangedE(), this, &ofxAddonTemplateCustomApp::Changed_params);
	ofRemoveListener(params_Control.parameterChangedE(), this, &ofxAddonTemplateCustomApp::Changed_params_Control);
	ofRemoveListener(params_Addon.parameterChangedE(), this, &ofxAddonTemplateCustomApp::Changed_params_Addon);

	//get gui position before save
	Gui_Position = glm::vec2(gui_Control.getPosition());

	saveParams(params_Control, path_GLOBAL + path_Params_Control);

	//exit();
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::setLogLevel(ofLogLevel level)
{
	ofSetLogLevel("ofxAddonTemplateCustomApp", level);
}


#pragma mark - OF LISTENERS

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::windowResized(int w, int h)
{
	screenW = w;
	screenH = h;

	//-

	//user gui deopending on window dimensions
	//gui_Control.setPosition(screenW * 0.5 - 200, screenH - 200);
}

//keys
//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::keyPressed(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;
	ofLogNotice("ofxAddonTemplateCustomApp") << "keyPressed: " << (char)key << " [" << key << "]";

	//modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	bool debug = false;
	if (debug)
	{
		ofLogNotice("ofxAddonTemplateCustomApp") << "mod_COMMAND: " << (mod_COMMAND ? "ON" : "OFF");
		ofLogNotice("ofxAddonTemplateCustomApp") << "mod_CONTROL: " << (mod_CONTROL ? "ON" : "OFF");
		ofLogNotice("ofxAddonTemplateCustomApp") << "mod_ALT: " << (mod_ALT ? "ON" : "OFF");
		ofLogNotice("ofxAddonTemplateCustomApp") << "mod_SHIFT: " << (mod_SHIFT ? "ON" : "OFF");
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
			ofLogNotice("ofxAddonTemplateCustomApp") << "";
		}
		else if (key == OF_KEY_UP)
		{
			ofLogNotice("ofxAddonTemplateCustomApp") << "";
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
		ofLogNotice("ofxAddonTemplateCustomApp") << "KEYS: " << (ENABLE_keys ? "ON" : "OFF");

		if (!ENABLE_keys)
		{
			ofLogNotice("ofxAddonTemplateCustomApp") << "ALL KEYS DISABLED. PRESS 'k' TO ENABLE GAIN!";
		}
		else
		{
			ofLogNotice("ofxAddonTemplateCustomApp") << "KEYS ENABLED BACK";
		}
	}
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::keyReleased(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;
	ofLogNotice("ofxAddonTemplateCustomApp") << "keyPressed: " << (char)key << " [" << key << "]";

	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::addKeysListeners()
{
	ofAddListener(ofEvents().keyPressed, this, &ofxAddonTemplateCustomApp::keyPressed);
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::removeKeysListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxAddonTemplateCustomApp::keyPressed);
}

//mouse
//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::mouseDragged(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//ofLogNotice("ofxAddonTemplateCustomApp") << "mouseDragged " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::mousePressed(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//ofLogNotice("ofxAddonTemplateCustomApp") << "mousePressed " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::mouseReleased(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//ofLogNotice("ofxAddonTemplateCustomApp") << "mouseReleased " << x << ", " << y << ", " << button;
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


#pragma mark - API

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::setActive(bool b)
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
void ofxAddonTemplateCustomApp::setGuiVisible(bool b)
{
	SHOW_Gui = b;
}


#pragma mark - CALLBACKS

//all params
//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::Changed_params(ofAbstractParameter &e)
{
	//if (!DISABLE_Callbacks)
	//{
	//	string name = e.getName();

	//	//exclude debugs
	//	if (name != "exclude"
	//		&& name != "exclude")
	//	{
	//		ofLogNotice("ofxAddonTemplateCustomApp") << "Changed_params: " << name << " : " << e;

	//	}

	//	//params
	//	if (name == "")
	//	{
	//	}
	//}
}

//addon engine params
//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::Changed_params_Addon(ofAbstractParameter &e)
{
	if (!DISABLE_Callbacks)
	{
		string name = e.getName();

		//exclude debugs
		if (name != "exclude"
			&& name != "exclude")
		{
			ofLogNotice("ofxAddonTemplateCustomApp") << "Changed_params_Addon: " << name << " : " << e;

		}

		//params
		if (name == "")
		{
		}
	}
}

//addon control (internal) params
//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::Changed_params_Control(ofAbstractParameter &e)
{
	if (!DISABLE_Callbacks)
	{
		string name = e.getName();

		//exclude debugs
		if (name != "exclude"
			&& name != "exclude")
		{
			ofLogNotice("ofxAddonTemplateCustomApp") << "Changed_params_Control: " << name << " : " << e;

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
void ofxAddonTemplateCustomApp::setKey_MODE_App(int k)
{
	key_MODE_App = k;
}


#pragma mark - FILE SETTINGS

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::setPathGlobal(string s)//must call before setup. disabled by default
{
	path_GLOBAL = s;
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::loadParams(ofParameterGroup &g, string path)
{
	ofLogNotice("ofxAddonTemplateCustomApp") << "loadParams: " << path;
	ofXml settings;
	settings.load(path);
	ofDeserialize(settings, g);
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::saveParams(ofParameterGroup &g, string path)
{
	ofLogNotice("ofxAddonTemplateCustomApp") << "saveParams: " << path;
	ofXml settings;
	ofSerialize(settings, g);
	settings.save(path);
}

