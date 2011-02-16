#include "Renderer.h"
#include <iostream>


Renderer::Renderer(IWindow const & window, ActorList const & actorList) : actorList(actorList) {

	ratio = 1.0;
	width = window.ScreenWidth();
	height = window.ScreenHeight();

	camPos = btVector3(9,21,45);
	camLook = btVector3(1.5,0,5);
	camUp = btVector3(0,1,0);

	lightPos = btVector3(50,150,240);

	shaderTextures.resize(MAX_TEXTURES);
	for (int i = 0; i < MAX_TEXTURES; i++) {
		shaderTextures[i] = new GLuint;
	}

	attrData = new AttributeData();
	texData = new TextureData(3);
	optData = new OptionsData();

	initializeGL();
	paintGL();
}

void Renderer::setCamera(btVector3 const & pos, btVector3 const & look)
{
	camPos = pos;
	camLook = look;
}

Renderer::~Renderer() { }


void Renderer::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	updateCamera();
	
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, actorList.front()->renderObject.texture);

	shader->on();
		applyShader();
		glColor3f(1,1,1);
		
		drawGround();

	shader->off();

		//light
/*	glColor3f(1,1,1);
	glDisable(GL_TEXTURE);
	glDisable(GL_LIGHTING);
	drawCube(btVector3(lightPos.getX() + 0.5, lightPos.getY() - 0.5, lightPos.getZ() - 0.5),
			 btVector3(lightPos.getX() + 0.5, lightPos.getY() + 0.5, lightPos.getZ() - 0.5),
			 btVector3(lightPos.getX() + 0.5, lightPos.getY() - 0.5, lightPos.getZ() + 0.5),
			 btVector3(lightPos.getX() + 0.5, lightPos.getY() + 0.5, lightPos.getZ() + 0.5),
			 btVector3(lightPos.getX() - 0.5, lightPos.getY() - 0.5, lightPos.getZ() - 0.5),
			 btVector3(lightPos.getX() - 0.5, lightPos.getY() + 0.5, lightPos.getZ() - 0.5),
			 btVector3(lightPos.getX() - 0.5, lightPos.getY() - 0.5, lightPos.getZ() + 0.5),
			 btVector3(lightPos.getX() - 0.5, lightPos.getY() + 0.5, lightPos.getZ() + 0.5));
	// glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE);*/

	renderObjects();
	
}

void Renderer::step() {
	paintGL();
}

void Renderer::renderObjects() {

	Actor* currentActor = 0;
	for (ActorList::const_iterator i = actorList.begin(); i != actorList.end(); i++) {
		currentActor = *i;
		glPushMatrix();

		glTranslated(currentActor->pos.getX(), currentActor->pos.getY(), currentActor->pos.getZ()); // Something meaningful will go here later

		btVector3 h = quatRotate(currentActor->orientation, btVector3(1,0,0));
		btVector3 b = quatRotate(currentActor->orientation, btVector3(0,0,1));
		btVector3 n = quatRotate(currentActor->orientation, btVector3(0,1,0));

		// This matrix is defined columnwise
		GLfloat frameMatrix[16] = { h.getX(), h.getY(), h.getZ(), 0, 
									n.getX(), n.getY(), n.getZ(), 0, 
									b.getX(), b.getY(), b.getZ(), 0, 
									0, 0, 0, 1};
		glMultMatrixf(frameMatrix);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, currentActor->renderObject.texture);
		shader->on();
			applyShader();

			glColor3f(1,1,1);
			glPushMatrix();
			//glScaled(0.2, 0.2, 0.2);
			currentActor->renderObject.draw();
			//currentActor->renderObject.drawNormals();
			glPopMatrix();

			// Clear all textures
			for (int i = MAX_TEXTURES-1; i >= 0; i--) {
				glActiveTexture(GL_TEXTURE0+i);
				glDisable(GL_TEXTURE_2D);
			}
		shader->off();

		glPopMatrix();
	}
}

void Renderer::applyShader() {
	vector<int> index = texData->getTextureIndices();

	// Set all the uniforms in the shaders to the values supplied by the attribute data structure
	glUniform1i(xAttrLoc, (int) attrData->xAttr);
	glUniform1f(xModLoc, attrData->xMod);
	glUniform1f(xZminLoc, attrData->xZmin);
	glUniform1i(xFlipLoc, (int) attrData->xFlip);

	glUniform1i(yAttrLoc, (int) attrData->yAttr);
	glUniform1f(yModLoc, attrData->yMod);
	glUniform1f(yZminLoc, attrData->yZmin);
	glUniform1i(yFlipLoc, (int) attrData->yFlip);

	glUniform1i(zAttrLoc, (int) attrData->zAttr);
	glUniform1f(zModLoc, attrData->zMod);
	glUniform1f(zZminLoc, attrData->zZmin);
	glUniform1i(zFlipLoc, (int) attrData->zFlip);

	glUniform1i(numTexLoc, index.size());

	GLfloat texPos[MAX_TEXTURES];
	GLfloat texHSkew[MAX_TEXTURES];
	GLfloat texVSkew[MAX_TEXTURES];
	GLint texInterp[MAX_TEXTURES];

	// For each texture, pass the texture data for this texture from the texture data structure
	for (unsigned int i = 0; i < MAX_TEXTURES; i++) {
		if (i < index.size()) {
			texPos[i] = texData->getTexturePos(index[i]);
			texHSkew[i] = texData->getTextureHSkew(index[i]);
			texVSkew[i] = texData->getTextureVSkew(index[i]);
			texInterp[i] = (int) texData->getTextureInterpolate(index[i]);

			glActiveTexture(GL_TEXTURE0+i);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, *shaderTextures[index[i]]);
		}
		else {
			texPos[i] = 5.0f;
			texHSkew[i] = 0.0f;
			texVSkew[i] = 0.0f;
			texInterp[i] = 1;
		}
	}

	// Pass the texture indices into the shader
	glUniform1i(tex0Loc, 0);
	glUniform1i(tex1Loc, 1);
	glUniform1i(tex2Loc, 2);
	glUniform1i(tex3Loc, 3);

	glUniform1fv(texPosLoc, MAX_TEXTURES, texPos);
	glUniform1fv(texHskewLoc, MAX_TEXTURES, texHSkew);
	glUniform1fv(texVskewLoc, MAX_TEXTURES, texVSkew);
	glUniform1iv(texInterpLoc, MAX_TEXTURES, texInterp);

	// Auto shading options
	glUniform1i(autoDiffuseLoc, (int) optData->autoDiffuse);
	glUniform1i(autoSpecularLoc, (int) optData->autoSpecular);

	glActiveTexture(GL_TEXTURE0+MAX_TEXTURES);
}

void Renderer::initializeGL() {
	glClearColor(0.7, 0.8, 1, 0);

	GLfloat whiteDir[4] = {1.0, 1.0, 1.0, 1.0};
	GLfloat blackDir[4] = {0.0, 0.0, 0.0, 1.0};
	GLfloat position[] = { lightPos.getX(), lightPos.getY(), lightPos.getZ(), 1 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDir);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteDir);
	glLightfv(GL_LIGHT0, GL_AMBIENT, blackDir);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_DEPTH_TEST);

	//glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
	glEnable(GL_RESCALE_NORMAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDir);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteDir);
	glMaterialf(GL_FRONT, GL_SHININESS, 1.0f);

	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_CULL_FACE);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	GLenum err = glewInit();
	if (GLEW_OK == err) {
		shader = new Shader("attr3D.frag", "model.vert");
		xAttrLoc = shader->getUniLoc("xAttr");
		xModLoc = shader->getUniLoc("xMod");
		xZminLoc = shader->getUniLoc("xZMin");
		xFlipLoc = shader->getUniLoc("xFlip");

		yAttrLoc = shader->getUniLoc("yAttr");
		yModLoc = shader->getUniLoc("yMod");
		yZminLoc = shader->getUniLoc("yZMin");
		yFlipLoc = shader->getUniLoc("yFlip");

		zAttrLoc = shader->getUniLoc("zAttr");
		zModLoc = shader->getUniLoc("zMod");
		zZminLoc = shader->getUniLoc("zZMin");
		zFlipLoc = shader->getUniLoc("zFlip");

		numTexLoc = shader->getUniLoc("numTex");
		texPosLoc = shader->getUniLoc("texPos");
		texHskewLoc = shader->getUniLoc("texHSkew");
		texVskewLoc = shader->getUniLoc("texVSkew");
		texInterpLoc = shader->getUniLoc("texInterp");

		tex0Loc = shader->getUniLoc("tex0");
		tex1Loc = shader->getUniLoc("tex1");
		tex2Loc = shader->getUniLoc("tex2");
		tex3Loc = shader->getUniLoc("tex3");

		autoDiffuseLoc = shader->getUniLoc("autoDiffuse");
		autoSpecularLoc = shader->getUniLoc("autoSpecular");
		
		load3DTexture("sunrisecopper.tx3");
		//load3DTexture("toonhill.tx3");
		//load3DTexture("gold.tx3");
		loadTextures();
	}
	shader->off();

	resizeGL(width, height); // Make the world not suck
	//updateCamera();
}

void Renderer::updateMousePosition(int x, int y) {
	mouseX = x;
	mouseY = y;
}

btVector3* Renderer::getScreenPosition(int x, int y) {
	btVector3* result;
	result->setX((double)x/(double)width*2.0-1.0);
	result->setY(-((double)y/(double)height*2.0-1.0));
	return result;
}

void Renderer::resizeGL(int w, int h) {
	width = w;
	height = h;
	ratio = (double)width / (double)height;

	setProjection();
}

void Renderer::setProjection() {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, ratio, 0.05*camPos.getZ(), max(1000.0*camPos.getZ(), 10.0));
	glMatrixMode(GL_MODELVIEW);
}

void Renderer::updateCamera() {
	gluLookAt(camPos.getX(), camPos.getY(), camPos.getZ(),
				  camLook.getX(), camLook.getY(), camLook.getZ(),
				  camUp.getX(), camUp.getY(), camUp.getZ());

	GLfloat position[] = { lightPos.getX(), lightPos.getY(), lightPos.getZ(), 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
}
/*
void Renderer::drawQuad(btVector3 const & tl, btVector3 const & tr, btVector3 const & bl, btVector3 const & br) {
	glBegin(GL_QUADS);
	btVector3 normal = (tl-tr).cross(tl-bl);
	glNormal3d(normal.getX(), normal.getY(), normal.getZ());
	glTexCoord2f(0, 0);
	glVertex3d(tr.getX(), tr.getY(), tr.getZ()); // Top Right Corner
	glTexCoord2f(0, 1);
	glVertex3d(tl.getX(), tl.getY(), tl.getZ()); // Top Left Corner
	glTexCoord2f(1, 1);
	glVertex3d(bl.getX(), bl.getY(), bl.getZ()); // Bottom Right Corner
	glTexCoord2f(1, 0);
	glVertex3d(br.getX(), br.getY(), br.getZ()); // Bottom Left Corner
	glEnd();
}

void Renderer::drawCube(btVector3 const & tlb, btVector3 const & trb, btVector3 const & tlf, btVector3 const & trf, btVector3 const & blb, btVector3 const & brb, btVector3 const & blf, btVector3 const & brf) {
	drawQuad(tlb, trb, tlf, trf); // Top face
	drawQuad(brb, blb, brf, blf); // Bottom face
	
	drawQuad(tlf, blf, tlb, blb); // Left face
	drawQuad(trb, brb, trf, brf); // Right face

	drawQuad(tlf, trf, blf, brf); // Front face
	drawQuad(blb, brb, tlb, trb); // Back face
}
*/
void Renderer::load3DTexture(string filename) {
	texData->load(filename);
	attrData->load(filename);
	optData->load(filename);
}

void Renderer::loadTextures() {
	vector<int> index = texData->getTextureIndices();
	for (unsigned int i = 0; i < index.size(); i++) {
		loadTexture(texData->getTextureFilename(index[i]), shaderTextures[index[i]]);
	}
}

bool Renderer::loadTexture(string name, GLuint *texID) {
	SDL_Surface *surface;
	GLenum textureFormat;
	int numColors;
	 
	if ((surface = SDL_LoadBMP(name.c_str()))) { 
	 
		numColors = surface->format->BytesPerPixel;
		if (numColors == 4) { // Has alpha
				if (surface->format->Rmask == 0x000000ff) textureFormat = GL_RGBA;
				else textureFormat = GL_BGRA;
		} else if (numColors == 3) {
				if (surface->format->Rmask == 0x000000ff) textureFormat = GL_RGB;
				else textureFormat = GL_BGR;
		} else {
			cout << "Invalid texture format (" << name << ")" << endl;
			return false;
		}
	 
		glGenTextures(1, texID);
		glBindTexture(GL_TEXTURE_2D, *texID);
	 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 
		glTexImage2D(GL_TEXTURE_2D, 0, numColors, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	} 
	else {
		cout << "Could not load texture \"" << name << "\" (" << SDL_GetError() << ")" << endl;
		return false;
	}    
	 
	if (surface) SDL_FreeSurface(surface);
	return true;
}

void Renderer::drawGround() {
	HeightMap* hm = new HeightMap(LoadString2("config/world.xml","height_map"));

	float xscale = LoadFloat("config/world.xml","height_map_scale_x");
	float yscale = LoadFloat("config/world.xml","height_map_scale_y");
	float zscale = LoadFloat("config/world.xml","height_map_scale_z");

	glPushMatrix();
	glTranslated(-((float)(hm->width*xscale))/2.0, 0, -((float)(hm->height*zscale))/2.0);
	glBegin(GL_QUADS);
	btVector3 v1, v2, v3, v4, n;
	for (int x = 0; x < hm->width - 1; x++) {
		for (int z = 0; z < hm->height - 1; z++) {
			v1 = btVector3((float)x * xscale, (float)(hm->map[x*hm->width+z]) * yscale, (float)z * zscale);
			v2 = btVector3((float)(x+1) * xscale, (float)(hm->map[(x+1)*hm->width+z]) * yscale, (float)z * zscale);
			v3 = btVector3((float)(x+1) * xscale, (float)(hm->map[(x+1)*hm->width+(z+1)]) * yscale, (float)(z+1) * zscale);
			v4 = btVector3((float)x * xscale, (float)(hm->map[x*hm->width+(z+1)]) * yscale, (float)(z+1) * zscale);
			n = (v1-v3).cross(v1-v2);
			glNormal3f(n.getX(), n.getY(), n.getZ());
			glVertex3f(v1.getZ() + zscale/2, v1.getY(), v1.getX() + xscale/2);
			glVertex3f(v2.getZ() + zscale/2, v2.getY(), v2.getX() + xscale/2);
			glVertex3f(v3.getZ() + zscale/2, v3.getY(), v3.getX() + xscale/2);
			glVertex3f(v4.getZ() + zscale/2, v4.getY(), v4.getX() + xscale/2);
		}
	}
	glEnd();
	glPopMatrix();
	
	delete hm;
}
