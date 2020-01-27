#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{
    AddonTemplate.setup();

	//AddonTemplate.setLogLevel(OF_LOG_NOTICE);
    //AddonTemplate.setKey_MODE_App('*');
}

//--------------------------------------------------------------
void ofApp::update()
{
    AddonTemplate.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    AddonTemplate.draw();
}

//--------------------------------------------------------------
void ofApp::exit()
{
    AddonTemplate.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    AddonTemplate.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
