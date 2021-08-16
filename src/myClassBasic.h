#pragma once

#include "ofMain.h"

class myClassBasic
{

public:

	myClassBasic();
	~myClassBasic();

	void setup();
//	void update();
	// void draw();
	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);
	void keyPressed(ofKeyEventArgs &eventArgs);
	void exit();

	void setBool(bool b);
	bool getBool();

private:

};

