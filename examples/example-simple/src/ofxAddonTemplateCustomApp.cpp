#include "ofxAddonTemplateCustomApp.h"

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::ofxAddonTemplateCustomApp()
{
	ofAddListener(ofEvents().update, this, &ofxAddonTemplateCustomApp::update);
	ofAddListener(ofEvents().keyPressed, this, &ofxAddonTemplateCustomApp::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxAddonTemplateCustomApp::keyReleased);
	ofAddListener(ofEvents().windowResized, this, &ofxAddonTemplateCustomApp::windowResized);
}
//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::ofxAddonTemplateCustomApp()
{
	ofRemoveListener(ofEvents().update, this, &ofxAddonTemplateCustomApp::update);
	ofRemoveListener(ofEvents().keyPressed, this, &ofxAddonTemplateCustomApp::keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &ofxAddonTemplateCustomApp::keyReleased);
	ofRemoveListener(ofEvents().windowResized, this, &ofxAddonTemplateCustomApp::windowResized);
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::setup(){

}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::update(ofEventArgs & args){

}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::draw(){
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::keyPressed(ofKeyEventArgs & args) {
	const int key = args.key;
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::keyReleased(ofKeyEventArgs & args) {
	const int key = args.key;
}

//--------------------------------------------------------------
void ofxSurfingFxPro::windowResized(ofResizeEventArgs & args) {
	int w = args.width;
	int h = args.height;
}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofxAddonTemplateCustomApp::dragEvent(ofDragInfo info){

}
