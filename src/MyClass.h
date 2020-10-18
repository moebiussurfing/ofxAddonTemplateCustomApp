#pragma once

#include "ofMain.h"

class MyClass
{

public:

	MyClass();
	~MyClass();

	void setup();
	void update();
	void draw();
	void exit();

	void setBool(bool b);
	bool getBool();

private:

};

