#include "ofxAddonTemplate.h"

#pragma mark - OF

//--------------------------------------------------------------
ofxAddonTemplate::~ofxAddonTemplate()
{
    setActive(false);//remove listeners

    ofRemoveListener(params.parameterChangedE(), this, &ofxAddonTemplate::Changed_params);

    saveParams(params, path_GLOBAL + path_Params);

    exit();
}

//--------------------------------------------------------------
ofxAddonTemplate::ofxAddonTemplate()
{
    setPathGlobal("ofxAddonTemplate/");
    setActive(true);//add listeners
}

//--------------------------------------------------------------
void ofxAddonTemplate::setup()
{
    //log mode
    ofSetLogLevel("ofxAddonTemplate", OF_LOG_NOTICE);
    //ofSetLogLevel("ofxAddonTemplate", OF_LOG_SILENT);

    //window
    screenW = ofGetWidth();
    screenH = ofGetHeight();

    //params
    isActive.set("isActive", true);
    SHOW_Gui.set("SHOW_Gui", true);
    Gui_Position.set("Gui_Position",
        glm::vec2(screenW * 0.5, screenH * 0.5),
        glm::vec2(0, 0),
        glm::vec2(screenW, screenH)
    );
    SHOW_Help.set("SHOW_Help", false);
    MODE_App.set("MODE_App", 0, 0, NUM_MODES_APP - 1);

    params.setName("ofxAddonTemplate");
    params.add(SHOW_Gui);
    params.add(Gui_Position);
    params.add(SHOW_Help);
    params.add(MODE_App);

    ofAddListener(params.parameterChangedE(), this, &ofxAddonTemplate::Changed_params);

    //gui
    gui.setup("gui");
    gui.add(params);

    //-

    //settings
    loadParams(params, path_GLOBAL + path_Params);

    //update after load
    gui.setPosition(Gui_Position.get().x, Gui_Position.get().y);

    //--

    //startup

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


#pragma mark - OF LISTENERS

//--------------------------------------------------------------
void ofxAddonTemplate::windowResized(int w, int h)
{
    screenW = w;
    screenH = h;

    //-

    //user gui (ofxPanel)
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

    //-

    //custom
    if (key == 'm')
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
    else if (key == OF_KEY_TAB)
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
    else if (key == 'a')
    {
        isActive = !isActive;
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
    ofLogNotice("ofxAddonTemplate") << "mouseDragged " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxAddonTemplate::mousePressed(ofMouseEventArgs &eventArgs)
{
    const int &x = eventArgs.x;
    const int &y = eventArgs.y;
    const int &button = eventArgs.button;
    ofLogNotice("ofxAddonTemplate") << "mousePressed " << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxAddonTemplate::mouseReleased(ofMouseEventArgs &eventArgs)
{
    const int &x = eventArgs.x;
    const int &y = eventArgs.y;
    const int &button = eventArgs.button;
    ofLogNotice("ofxAddonTemplate") << "mouseReleased " << x << ", " << y << ", " << button;
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
    isActive = b;
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

    //filter
    if (name == "")
    {
    }
    else if (name == "isActive")
    {
        setActive(isActive);
    }
    else if (name == "SHOW_Gui")
    {
    }

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

