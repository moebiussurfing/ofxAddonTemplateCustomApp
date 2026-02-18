#pragma once

#include "ofMain.h"

#include "ofxAddonTemplateCustomApp.h"

class ofxAddonTemplateCustomApp{

	public:
		void setup();
		// void update();
		void draw();

		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void gotMessage(ofMessage msg);

	private:
		void update(ofEventArgs & args);
		void windowResized(ofResizeEventArgs & args);
		void keyPressed(int key);

	public:

		ofxAddonTemplateCustomApp classObject;

		void dragEvent(ofDragInfo dragInfo);
};
