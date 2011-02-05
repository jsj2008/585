#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "Common/prelude.h"
#include <SDL.h>
#include <string>
#include <iostream>
using namespace std;

class RenderObject {
public:	
	RenderObject();
	RenderObject(string textureName);
	virtual ~RenderObject();
	
	GLuint texture;

private:
	bool loadTexture();

	string textureName;
};

#endif