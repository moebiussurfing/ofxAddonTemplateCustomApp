#include "ofxSurfingBeatSync.h"

//--------------------------------------------------------------
ofxSurfingBeatSync::ofxSurfingBeatSync()
{
	// path for settings
	setPathGlobal("ofxSurfingBeatSync/");
	path_Params_AppSettings = "ofxSurfingBeatSync_AppSettings.xml";
	path_Params_Control = "Params_Control.xml";

	bCallbacksEnable = false; // disable until ends setup
	setActive(true); // adds key and mouse listeners

	ofAddListener(ofEvents().update, this, &ofxSurfingBeatSync::update);
	ofAddListener(ofEvents().draw, this, &ofxSurfingBeatSync::draw);
	//ofAddListener(ofEvents().draw, this, &ofxSurfingBeatSync::draw, OF_EVENT_ORDER_AFTER_APP);

	//-

	// Auto called on creation
	setup();
}

//--------------------------------------------------------------
ofxSurfingBeatSync::~ofxSurfingBeatSync()
{
	setActive(false); // removes keys and mouse listeners

	// removes params callbacks listeners
	ofRemoveListener(params.parameterChangedE(), this, &ofxSurfingBeatSync::Changed_params);
	ofRemoveListener(params_Control.parameterChangedE(), this, &ofxSurfingBeatSync::Changed_params_Control);
	ofRemoveListener(params_Addon.parameterChangedE(), this, &ofxSurfingBeatSync::Changed_params_Addon);

	// removes update/draw callbacks
	ofRemoveListener(ofEvents().update, this, &ofxSurfingBeatSync::update);
	ofRemoveListener(ofEvents().draw, this, &ofxSurfingBeatSync::draw);
	//ofRemoveListener(ofEvents().draw, this, &ofxSurfingBeatSync::draw, OF_EVENT_ORDER_AFTER_APP);

	//-

	// Auto called on destruction
	exit();
}

////--------------------------------------------------------------
//void ofxSurfingBeatSync::audioReceived(float* input, int bufferSize, int nChannels) {
//	beat.audioReceived(input, bufferSize, nChannels);
//}

//--------------------------------------------------------------
void ofxSurfingBeatSync::setup()
{
	//ofSoundStreamSetup(0, 1, this, 44100, beat.getBufferSize(), 4);

	setupBtrack();
	
	//----

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
void ofxSurfingBeatSync::setupBtrack()
{
	ofSetFrameRate(60);

	//-

	sampleRate = 44100;
	bufferSize = 256;

	// Setup frame size
	btrack.setup(bufferSize);

	// FIX: this is experimental implementation.
	btrack.setConfidentThreshold(0.75);

	// Beat circle
	float _radius = 200;
	ofSetCircleResolution(80);
	circleBeat.setColor(ofColor(255, 225));
	circleBeat.setSize(_radius);
	circleBeat.setSpeed(0.2f);
	glm::vec2 _pos = glm::vec2(ofGetWindowWidth() * 0.5f, ofGetWindowHeight() * 0.5f);
	circleBeat.setPosition(_pos);

	//-

	// Setup Audio Input
	audioDevices.setup(sampleRate, bufferSize);

	// Or use your own sound manager
	//ofSoundStreamSetup(0, 1, sampleRate, bufferSize, 4);
}
//--------------------------------------------------------------
void ofxSurfingBeatSync::setupParameters()
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
	ofAddListener(params_Addon.parameterChangedE(), this, &ofxSurfingBeatSync::Changed_params_Addon);

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
	ofAddListener(params_Control.parameterChangedE(), this, &ofxSurfingBeatSync::Changed_params_Control);

	//-

	// all params
	params.setName("All Params");
	params.add(params_AppSettings);
	params.add(params_Addon);
	params.add(params_Control);

	// callback
	ofAddListener(params.parameterChangedE(), this, &ofxSurfingBeatSync::Changed_params);
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::startup()
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
void ofxSurfingBeatSync::setupHelp()
{
	strHelpInfo = "ofxSurfingBeatSync\n\n";
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
void ofxSurfingBeatSync::setupGui()
{
	ofLogNotice(__FUNCTION__);

	//-

	// setup ofxGui
	{
		// theme
		ofxSurfingHelpers::setThemeDarkMini_ofxGui();

		//-

		gui_Advanced.setup("ofxSurfingBeatSync");
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
void ofxSurfingBeatSync::setupImGui()
{
	ofLogNotice(__FUNCTION__);

	//-

	//guiManager.setImGuiViewPort(true); // -> For floating windows

	guiManager.setName("ofxSurfingBeatSync");
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
void ofxSurfingBeatSync::setupImGuiStyles()
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
void ofxSurfingBeatSync::drawImGui_User() { // user gui
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
void ofxSurfingBeatSync::drawImGui() {
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
void ofxSurfingBeatSync::drawHelp()
{
	helpTextBoxWidget.setText(strHelpInfo);
	helpTextBoxWidget.draw();
}

//----

//--------------------------------------------------------------
void ofxSurfingBeatSync::update(ofEventArgs & args) {

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
void ofxSurfingBeatSync::draw(ofEventArgs & args)
{
	beat.update(ofGetElapsedTimeMillis());

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
void ofxSurfingBeatSync::update() {
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::draw() {
	//stringstream ss;
	cout << beat.kick() << "," << beat.snare() << "," << beat.hihat() << endl;
	//ofLogNotice() << ss;

	drawBtrack();
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::drawBtrack() {

	// BPM
	float bpm = btrack.getEstimatedBPM();
	ofDrawBitmapStringHighlight("ESTIMATED BPM: " + ofToString(bpm, 1), 30, 300);
	ofDrawBitmapStringHighlight("CONFIDENCE: " + ofToString(btrack.getConfidence(), 2), 30, 325);
	// confidence is an experimental feature at this moment

	// Beat callback
	circleBeat.draw();
	if (btrack.hasBeat())
	{
		circleBeat.bang();
#ifdef INCLUDE_ofxTextFlow//LOG
		logLine("BEAT! \t BPM: " + ofToString(bpm, 1));
#endif
	}

	// Without circleBeat
	//ofDrawBitmapString("BEAT", 85, 195);
	//if (btrack.hasBeat()) {    //FIXME: calling this method resets hasBeat flag in ofxBTrack object
	//	ofSetColor(ofColor::magenta);
	//}
	//else ofSetColor(ofColor::lightGray);
	//ofDrawCircle(100, 150, 30);

	//--

	drawWaveform();

	audioDevices.drawGui();
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::drawWaveform() {

	ofPushStyle();
	ofPushMatrix();

	ofFill();
	ofSetColor(0, 225);
	ofSetLineWidth(3.0f);

	//float _max = ofGetHeight() / 5.f;
	float _max = 200;

	ofTranslate(0, ofGetHeight() / 2.f);
	ofDrawLine(0, 0, 1, waveformInput[1] * _max); //first line
	for (int i = 1; i < (ofGetWidth() - 1); ++i) {
		ofDrawLine(i, waveformInput[i] * _max, i + 1, waveformInput[i + 1] * _max);
	}

	ofPopMatrix();
	ofPopStyle();
}

//
//// Audio Callbacks
////--------------------------------------------------------------
//void ofxSurfingBeatSync::audioIn(float* input, int _bufferSize, int _nChannels) {
//
//	// feed audio frame into ofxBTrack object
//	btrack.audioIn(input, _bufferSize, _nChannels);
//
//	/*
//	{
//		https://github.com/satoruhiga/ofxBTrack/blob/master/example-soundPlayer/src/ofxFmodSoundPlayer.cpp
//
//		// handle 512 sample each time /  called 23 time per sec
//
//		// pass the data along
//		memcpy(outbuffer, inbuffer, sizeof(float) * length * outchannels);
//
//		void* userdata = NULL;
//		FMOD_DSP_GetUserData(dsp_state->instance, &userdata);
//		ofxFmodSoundPlayer* player = (ofxFmodSoundPlayer*)userdata;
//
//		// BPM analysis
//		ofxBTrack* btrack = &(player->btrack);
//		if (btrack) btrack->audioIn(inbuffer, length, inchannels);
//
//		return FMOD_OK;
//	}
//	*/
//
//	/*
//	//TODO
//	{
//		// Fill the buffer, frame by frame
//		for (int i = 0; i < buffer.getNumFrames(); i++) {
//			// Read 1 sample from a file and put it in our buffer
//			play(rBuffer.getSample(i, 0), lBuffer.getSample(i, 0));
//		}
//
//		buffer.setChannel(rBuffer, 0);
//		buffer.setChannel(lBuffer, 1);
//
//		//sampleRate = 44100;
//		//bufferSize = 256;
//		ofSoundBuffer buff(buffer, bufferSize, 2, sampleRate);
//
//		audioDevices.audioIn(&input);
//	}
//	*/
//
//	//--
//
//	// Draw Waveform
//	std::size_t nChannels = _nChannels;
//	for (size_t i = 0; i < _bufferSize; i++)
//	{
//		// Handle input here
//		// Hold the values so the draw method can draw them
//		waveformInput[waveInputIndex] = input[i * _nChannels];
//		if (waveInputIndex < (ofGetWidth() - 1)) {
//			++waveInputIndex;
//		}
//		else {
//			waveInputIndex = 0;
//		}
//	}
//}

//--------------------------------------------------------------
void ofxSurfingBeatSync::drawGui() {

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
void ofxSurfingBeatSync::windowResized(int w, int h)
{
	wWindow = w;
	hWindow = h;

	rectWindow = ofGetCurrentViewport();

	//-

	// user gui deopending on window dimensions
	//gui_Advanced.setPosition(wWindow * 0.5 - 200, hWindow - 200);
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::exit()
{
	saveSettings();
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::setActive(bool b)
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
void ofxSurfingBeatSync::setGuiVisible(bool b)
{
	ofLogNotice(__FUNCTION__);
	bGui = b;
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::setGuiVisibleToggle()
{
	ofLogNotice(__FUNCTION__);
	bGui = !bGui;
}

// keys
//--------------------------------------------------------------
void ofxSurfingBeatSync::keyPressed(ofKeyEventArgs &eventArgs)
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

	if (key == 'g') audioDevices.setVisibleToggle();

}

//--------------------------------------------------------------
void ofxSurfingBeatSync::keyReleased(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;
	ofLogNotice(__FUNCTION__) << (char)key << " [" << key << "]";

	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::addKeysListeners()
{
	ofLogNotice(__FUNCTION__);
	ofAddListener(ofEvents().keyPressed, this, &ofxSurfingBeatSync::keyPressed);
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::removeKeysListeners()
{
	ofLogNotice(__FUNCTION__);
	ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingBeatSync::keyPressed);
}

// mouse
//--------------------------------------------------------------
void ofxSurfingBeatSync::mouseDragged(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	ofLogVerbose(__FUNCTION__) << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::mousePressed(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	ofLogVerbose(__FUNCTION__) << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::mouseReleased(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	ofLogVerbose(__FUNCTION__) << x << ", " << y << ", " << button;
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::addMouseListeners()
{
	ofAddListener(ofEvents().mouseDragged, this, &ofxSurfingBeatSync::mouseDragged);
	ofAddListener(ofEvents().mousePressed, this, &ofxSurfingBeatSync::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxSurfingBeatSync::mouseReleased);
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::removeMouseListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingBeatSync::keyPressed);
}

//-

// Callbacks

//--------------------------------------------------------------
void ofxSurfingBeatSync::Changed_params(ofAbstractParameter &e)
{
	if (!bCallbacksEnable) return;

	string name = e.getName();
	ofLogNotice(__FUNCTION__) << name << " : " << e;

	if (0) {}
}

// addon engine params
//--------------------------------------------------------------
void ofxSurfingBeatSync::Changed_params_Addon(ofAbstractParameter &e)
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
void ofxSurfingBeatSync::Changed_params_Control(ofAbstractParameter &e)
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
void ofxSurfingBeatSync::setKeyAppMode(int k)
{
	keyAppMode = k;
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::setLogLevel(ofLogLevel logLevel)
{
	ofLogWarning(__FUNCTION__) << ofLogLevel(logLevel);
	ofSetLogLevel("ofxSurfingBeatSync", logLevel);
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::dragEvent(ofDragInfo info) {
}

//-

// Settings

//--------------------------------------------------------------
void ofxSurfingBeatSync::setPathGlobal(string s)//must call before setup. disabled by default
{
	path_Global = s;

	ofxSurfingHelpers::CheckFolder(path_Global);
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::saveSettings() {
	ofLogVerbose(__FUNCTION__);

	// get gui position before save
	positionGuiAdvanced = glm::vec2(gui_Advanced.getPosition());

	ofxSurfingHelpers::saveGroup(params_Control, path_Global + path_Params_Control);
	ofxSurfingHelpers::saveGroup(params_AppSettings, path_Global + path_Params_AppSettings);
}

//--------------------------------------------------------------
void ofxSurfingBeatSync::loadSettings() {
	ofLogVerbose(__FUNCTION__);

	ofxSurfingHelpers::CheckFolder(path_Global);

	// settings
	ofxSurfingHelpers::loadGroup(params_Control, path_Global + path_Params_Control);
	ofxSurfingHelpers::loadGroup(params_AppSettings, path_Global + path_Params_AppSettings);
}
