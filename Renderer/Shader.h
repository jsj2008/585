#pragma once

#include "depend.h"
#include <iostream>
using namespace std;

class Shader {
public:
	Shader(GLchar *fs, GLchar *vs);

	void on();
	void off();
	int getUniLoc(const char *name);
	int getAttrLoc(const char *name);

	GLuint v, f, p;	//Handles for our vertex shader, fragment shader and the program object.
private:

	char *textFileRead(const char *fn);	//Methods that read and write shader files.
	int textFileWrite(char *fn, char *s);
};
