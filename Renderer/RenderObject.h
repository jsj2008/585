#pragma once

#include "Common/prelude.h"
#include "Model.h"
#include <SDL.h>
#include <string>
#include <iostream>
using namespace std;

class RenderObject {

private:
	bool loadTexture(string textureName);

public:	
	RenderObject();
	RenderObject(string textureName, string modelName);
	virtual ~RenderObject();

	GLuint texture;
	Model model;

	void draw() const;
	void drawNormals() const;

};
