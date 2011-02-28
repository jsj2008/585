#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "Common/prelude.h"
#include "Model.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
using namespace std;

class RenderObject {

private:
	bool loadTexture(string textureName, GLuint *texID);

public:	
	RenderObject();
	RenderObject(string textureName, string bumpMapName, string modelName, float scale = 1.0);
	virtual ~RenderObject();

	GLuint texture;	// Texture to apply to this object when rendered
	GLuint bumpMap;	// Bump map to apply to this object when rendered
	Model model;	// The model that represents this object (rendering only, not collision)
	float scale;	// Scale factor to resize

	void draw() const;
	void drawNormals() const;

};
#endif