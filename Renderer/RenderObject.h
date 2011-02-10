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

	void draw() const {
		glBegin(GL_TRIANGLES);
		Face currentFace;
		btVector3* currentVector;
		for (int i = 0; i < model.faces.size(); i++) {
			currentFace = model.faces.at(i);

			currentVector = model.normals.at(currentFace.normals.at(0)-1);
			glNormal3d(currentVector->getX(), currentVector->getY(), currentVector->getZ());
			if (currentFace.texCoords.at(0) != 0) {
				currentVector = model.texCoords.at(currentFace.texCoords.at(0)-1);
				glTexCoord2f(currentVector->getX(), currentVector->getY());
			}
			currentVector = model.vertices.at(currentFace.vertices.at(0)-1);
			glVertex3d(currentVector->getX(), currentVector->getY(), currentVector->getZ());
			
			currentVector = model.normals.at(currentFace.normals.at(1)-1);
			glNormal3d(currentVector->getX(), currentVector->getY(), currentVector->getZ());
			if (currentFace.texCoords.at(1) != 0) {
				currentVector = model.texCoords.at(currentFace.texCoords.at(1)-1);
				glTexCoord2f(currentVector->getX(), currentVector->getY());
			}
			currentVector = model.vertices.at(currentFace.vertices.at(1)-1);
			glVertex3d(currentVector->getX(), currentVector->getY(), currentVector->getZ());
			
			currentVector = model.normals.at(currentFace.normals.at(2)-1);
			glNormal3d(currentVector->getX(), currentVector->getY(), currentVector->getZ());
			if (currentFace.texCoords.at(2) != 0) {
				currentVector = model.texCoords.at(currentFace.texCoords.at(2)-1);
				glTexCoord2f(currentVector->getX(), currentVector->getY());
			}
			currentVector = model.vertices.at(currentFace.vertices.at(2)-1);
			glVertex3d(currentVector->getX(), currentVector->getY(), currentVector->getZ());
		}
		glEnd();
	}

};
