#include "RenderObject.h"

RenderObject::RenderObject() { }

RenderObject::RenderObject(string textureName, string modelName, float scale) {
	loadTexture(textureName);
	model = Model(modelName);
	this->scale = scale;
}

RenderObject::~RenderObject() { }

void RenderObject::draw() const {
	Face currentFace;
	btVector3* currentVector;
	glPushMatrix();
	glScalef(scale, scale, scale);
	for (int i = 0; i < model.faces.size(); i++) {
		currentFace = model.faces.at(i);
		if (currentFace.vertices.size() == 3) glBegin(GL_TRIANGLES);
		if (currentFace.vertices.size() == 4) glBegin(GL_QUADS);

		for (int j = 0; j < currentFace.vertices.size(); j++) {

			currentVector = model.normals.at(currentFace.normals.at(j)-1);
			glNormal3d(currentVector->getX(), currentVector->getY(), currentVector->getZ());
			if (currentFace.texCoords.at(j) != 0) {
				currentVector = model.texCoords.at(currentFace.texCoords.at(j)-1);
				glTexCoord2f(currentVector->getX(), currentVector->getY());
			}
			currentVector = model.vertices.at(currentFace.vertices.at(j)-1);
			glVertex3d(currentVector->getX(), currentVector->getY(), currentVector->getZ());
		}
		glEnd();
	}
	glPopMatrix();
}

// Draw model normals for debugging
void RenderObject::drawNormals() const {
	glPushMatrix();
	glScalef(scale, scale, scale);
	glBegin(GL_LINES);
	Face currentFace;
	btVector3 currentVertex;
	btVector3 currentNormal;
	for (int i = 0; i < model.faces.size(); i++) {
		currentFace = model.faces.at(i);

		currentVertex = *model.vertices.at(currentFace.vertices.at(0)-1);
		glVertex3d(currentVertex.getX(), currentVertex.getY(), currentVertex.getZ());
		currentNormal = *model.normals.at(currentFace.normals.at(0)-1);
		currentVertex = currentVertex + currentNormal;
		glVertex3d(currentVertex.getX(), currentVertex.getY(), currentVertex.getZ());
		
		currentVertex = *model.vertices.at(currentFace.vertices.at(1)-1);
		glVertex3d(currentVertex.getX(), currentVertex.getY(), currentVertex.getZ());
		currentNormal = *model.normals.at(currentFace.normals.at(1)-1);
		currentVertex = currentVertex + currentNormal;
		glVertex3d(currentVertex.getX(), currentVertex.getY(), currentVertex.getZ());
		
		currentVertex = *model.vertices.at(currentFace.vertices.at(2)-1);
		glVertex3d(currentVertex.getX(), currentVertex.getY(), currentVertex.getZ());
		currentNormal = *model.normals.at(currentFace.normals.at(2)-1);
		currentVertex = currentVertex + currentNormal;
		glVertex3d(currentVertex.getX(), currentVertex.getY(), currentVertex.getZ());
	}
	glEnd();
	glPopMatrix();
}

// Loads this render object's texture internally
bool RenderObject::loadTexture(string textureName) {
	SDL_Surface *surface;
	GLenum textureFormat;
	int numColors;
	 
	if ((surface = IMG_Load(textureName.c_str()))) { 
	 
		numColors = surface->format->BytesPerPixel;
		if (numColors == 4) { // Has alpha
				if (surface->format->Rmask == 0x000000ff) textureFormat = GL_RGBA;
				else textureFormat = GL_BGRA;
		} else if (numColors == 3) {
				if (surface->format->Rmask == 0x000000ff) textureFormat = GL_RGB;
				else textureFormat = GL_BGR;
		} else {
			cout << "Invalid texture format (" << textureName << ")" << endl;
			return false;
		}
	 
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
	 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 
		glTexImage2D(GL_TEXTURE_2D, 0, numColors, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	} 
	else {
		cout << "Could not load texture \"" << textureName << "\" (" << SDL_GetError() << ")" << endl;
		return false;
	}    
	 
	if (surface) SDL_FreeSurface(surface);
	return true;
}
