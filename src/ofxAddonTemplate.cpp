#include "ofxAddonTemplate.h"

#pragma mark - OF

//--------------------------------------------------------------
ofxAddonTemplate::ofxAddonTemplate()
{
    //path for settings
    setPathGlobal("ofxAddonTemplate/");
    path_Params = "params.xml";

    setActive(true);//add listeners
}

//--------------------------------------------------------------
ofxAddonTemplate::~ofxAddonTemplate()
{
    setActive(false);//remove listeners

    ofRemoveListener(params.parameterChangedE(), this, &ofxAddonTemplate::Changed_params);

    //get gui position before save
    Gui_Position = glm::vec2(gui.getPosition());

    saveParams(params, path_GLOBAL + path_Params);

    exit();
}

//--------------------------------------------------------------
void ofxAddonTemplate::setup()
{
    //log mode
    ofSetLogLevel("ofxAddonTemplate", OF_LOG_NOTICE);
    //ofSetLogLevel("ofxAddonTemplate", OF_LOG_SILENT);

    //--

    //window
    screenW = ofGetWidth();
    screenH = ofGetHeight();

    //--

    //params
    MODE_Active.set("ACTIVE", true);
    ENABLE_keys.set("KEYS", true);
    SHOW_Help.set("HELP", false);
    MODE_App.set("APP MODE", 0, 0, NUM_MODES_APP - 1);
    ENABLE_Debug.set("DEBUG", true);
    SHOW_Gui.set("GUI", true);
    Gui_Position.set("GUI POSITION",
        glm::vec2(screenW * 0.5, screenH * 0.5),
        glm::vec2(0, 0),
        glm::vec2(screenW, screenH)
    );

    //params group
    params.setName("ofxAddonTemplate");
    params.add(MODE_Active);
    params.add(ENABLE_keys);
    params.add(SHOW_Help);
    params.add(MODE_App);
    params.add(ENABLE_Debug);
    params.add(SHOW_Gui);
    params.add(Gui_Position);

    //callback
    ofAddListener(params.parameterChangedE(), this, &ofxAddonTemplate::Changed_params);

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
    gui.setup("ofxAddonTemplate");
    gui.add(params);

    //-

    //settings
    loadParams(params, path_GLOBAL + path_Params);

    //update after load
    gui.setPosition(Gui_Position.get().x, Gui_Position.get().y);

    //--

    //startup

    ofLogNotice("ofxAddonTemplate") << "STARTUP INIT";

    MODE_Active = true;

    //set gui position after window setup/resizing
    windowResized(screenW, screenH);
}

//--------------------------------------------------------------
void ofxAddonTemplate::update()
{
}

//--------------------------------------------------------------
void ofxAddonTemplate::draw()
{
    if (SHOW_Gui)
    {
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofxAddonTemplate::exit()
{

}

//--------------------------------------------------------------
void ofxAddonTemplate::setLogLevel(ofLogLevel level)
{
    ofSetLogLevel("ofxAddonTemplate", level);
}


#pragma mark - OF LISTENERS

//--------------------------------------------------------------
void ofxAddonTemplate::windowResized(int w, int h)
{
    screenW = w;
    screenH = h;

    //-

    //user gui deopending on window dimensions
    //gui.setPosition(screenW * 0.5 - 200, screenH - 200);
}

//keys
//--------------------------------------------------------------
void ofxAddonTemplate::keyPressed(ofKeyEventArgs &eventArgs)
{
    const int &key = eventArgs.key;
    ofLogNotice("ofxAddonTemplate") << "keyPressed: " << key;

    //modifiers
    bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
    bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
    bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
    bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

    bool debug = false;
    if (debug)
    {
        ofLogNotice("ofxAddonTemplate") << "mod_COMMAND: " << (mod_COMMAND ? "ON" : "OFF");
        ofLogNotice("ofxAddonTemplate") << "mod_CONTROL: " << (mod_CONTROL ? "ON" : "OFF");
        ofLogNotice("ofxAddonTemplate") << "mod_ALT: " << (mod_ALT ? "ON" : "OFF");
        ofLogNotice("ofxAddonTemplate") << "mod_SHIFT: " << (mod_SHIFT ? "ON" : "OFF");
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
            ofLogNotice("ofxAddonTemplate") << "";
        }
        else if (key == OF_KEY_UP)
        {
            ofLogNotice("ofxAddonTemplate") << "";
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
        ofLogNotice("ofxAddonTemplate") << "KEYS: " << (ENABLE_keys ? "ON" : "OFF");

        if (!ENABLE_keys)
        {
            ofLogNotice("ofxAddonTemplate") << "ALL KEYS DISABLED. PRESS 'k' TO ENABLE GAIN!";
        }
        else
        {
            ofLogNotice("ofxAddonTemplate") << "KEYS ENABLED BACK";
        }
    }
}

//--------------------------------------------------------------
void ofxAddonTemplate::keyReleased(ofKeyEventArgs &eventArgs)
{
    int key = eventArgs.key;

    ofLogNotice("ofxAddonTemplate") << "keyReleased: " << key << endl;

    bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
    bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
    bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
    bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);
}

//--------------------------------------------------------------
void ofxAddonTemplate::addKeysListeners()
{
    ofAddListener(ofEvents().keyPressed, this, &ofxAddonTemplate::keyPressed);
}

//--------------------------------------------------------------
void ofxAddonTemplate::removeKeysListeners()
{
    ofRemoveListener(ofEvents().keyPressed, this, &ofxAddonTemplate::keyPressed);
}

//mouse
//--------------------------------------------------------------
void ofxAddonTemplate::mouseDragged(ofMouseEventArgs &eventArgs)
{
    const int &x = eventArgs.x;
    const int &y = eventArgs.y;
    const int &button = eventArgs.button;
    //ofLogNotice("ofxAddonTemplate") << "mouseDragged " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxAddonTemplate::mousePressed(ofMouseEventArgs &eventArgs)
{
    const int &x = eventArgs.x;
    const int &y = eventArgs.y;
    const int &button = eventArgs.button;
    //ofLogNotice("ofxAddonTemplate") << "mousePressed " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxAddonTemplate::mouseReleased(ofMouseEventArgs &eventArgs)
{
    const int &x = eventArgs.x;
    const int &y = eventArgs.y;
    const int &button = eventArgs.button;
    //ofLogNotice("ofxAddonTemplate") << "mouseReleased " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxAddonTemplate::addMouseListeners()
{
    ofAddListener(ofEvents().mouseDragged, this, &ofxAddonTemplate::mouseDragged);
    ofAddListener(ofEvents().mousePressed, this, &ofxAddonTemplate::mousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &ofxAddonTemplate::mouseReleased);
}

//--------------------------------------------------------------
void ofxAddonTemplate::removeMouseListeners()
{
    ofRemoveListener(ofEvents().keyPressed, this, &ofxAddonTemplate::keyPressed);
}


#pragma mark - API

//--------------------------------------------------------------
void ofxAddonTemplate::setActive(bool b)
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
void ofxAddonTemplate::setGuiVisible(bool b)
{
    SHOW_Gui = b;
}


#pragma mark - CALLBACKS

//--------------------------------------------------------------
void ofxAddonTemplate::Changed_params(ofAbstractParameter &e)
{
    string name = e.getName();

    //exclude debugs
    if (name != "exclude"
        && name != "exclude")
    {
        ofLogNotice("ofxAddonTemplate") << "Changed_params: " << name << " : " << e;

    }

    //filter params
    if (name == "")
    {
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

//--------------------------------------------------------------
void ofxAddonTemplate::setKey_MODE_App(int k)
{
    key_MODE_App = k;
}


#pragma mark - FILE SETTINGS

//--------------------------------------------------------------
void ofxAddonTemplate::setPathGlobal(string s)//must call before setup. disabled by default
{
    path_GLOBAL = s;
}

//--------------------------------------------------------------
void ofxAddonTemplate::loadParams(ofParameterGroup &g, string path)
{
    ofLogNotice("ofxAddonTemplate") << "loadParams: " << path;
    ofXml settings;
    settings.load(path);
    ofDeserialize(settings, g);
}

//--------------------------------------------------------------
void ofxAddonTemplate::saveParams(ofParameterGroup &g, string path)
{
    ofLogNotice("ofxAddonTemplate") << "saveParams: " << path;
    ofXml settings;
    ofSerialize(settings, g);
    settings.save(path);
}

