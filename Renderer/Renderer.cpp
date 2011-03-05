#include "Renderer.h"
#include <iostream>
#include "Physics/HeightMapManager.h"


Renderer::Renderer(IWindow const & window, ActorList const & actorList) : actorList(actorList) {

	hm = 0;

	ratio = 1.0;
	width = window.ScreenWidth();
	height = window.ScreenHeight();

	//camPos = btVector3(9,21,45);
	//camLook = btVector3(1.5,0,5);
	camUp = btVector3(0,1,0);

	lightPos = btVector3(500,-15000,240);

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

void Renderer::setCamera(btVector3 const & pos, btVector3 const & look) {
	camPos = pos;
	camLook = look;
}

Renderer::~Renderer() { }


void Renderer::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	updateCamera();
	
	drawGround();
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

		glTranslated(currentActor->pos.getX(), currentActor->pos.getY(), currentActor->pos.getZ());

		btVector3 h = quatRotate(currentActor->orientation, btVector3(1,0,0));
		btVector3 b = quatRotate(currentActor->orientation, btVector3(0,0,1));
		btVector3 n = quatRotate(currentActor->orientation, btVector3(0,1,0));

		// This matrix is defined columnwise
		GLfloat frameMatrix[16] = { h.getX(), h.getY(), h.getZ(), 0, 
									n.getX(), n.getY(), n.getZ(), 0, 
									b.getX(), b.getY(), b.getZ(), 0, 
									0, 0, 0, 1};
		glMultMatrixf(frameMatrix);

		glActiveTexture(GL_TEXTURE4); // Apply the current actor's texture
		glBindTexture(GL_TEXTURE_2D, currentActor->renderObject.texture);
		glActiveTexture(GL_TEXTURE5); // Apply the current actor's bump map
		glBindTexture(GL_TEXTURE_2D, currentActor->renderObject.bumpMap);
		shader->on();
			applyShader();

			glColor3f(1,1,1);
			currentActor->renderObject.draw();
			//currentActor->renderObject.drawNormals();

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
	glUniform1i(colourMapLoc, 4);

	glActiveTexture(GL_TEXTURE5); // Apply the bump map, right now, the same one for everything :-o
	glBindTexture(GL_TEXTURE_2D, groundBump);

	glUniform1i(normalMapLoc, 5);

	glUniform1fv(texPosLoc, MAX_TEXTURES, texPos);
	glUniform1fv(texHskewLoc, MAX_TEXTURES, texHSkew);
	glUniform1fv(texVskewLoc, MAX_TEXTURES, texVSkew);

	// Auto shading options
	glUniform1i(autoDiffuseLoc, (int) optData->autoDiffuse);
	glUniform1i(autoSpecularLoc, (int) optData->autoSpecular);

	glActiveTexture(GL_TEXTURE0+MAX_TEXTURES);
}

void Renderer::initializeGL() {
	//glClearColor(0.63, 0.77, 0.77, 0);
	glClearColor(1, 1, 1, 0);

	GLfloat whiteDir[4] = {1.0, 1.0, 1.0, 1.0};
	GLfloat blackDir[4] = {0.0, 0.0, 0.0, 1.0};
	GLfloat position[] = { lightPos.getX(), lightPos.getY(), lightPos.getZ(), 1 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDir);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteDir);
	glLightfv(GL_LIGHT0, GL_AMBIENT, blackDir);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_DEPTH_TEST);

	glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
	glEnable(GL_LIGHTING);
	//glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glFrontFace(GL_CW);
	glEnable(GL_RESCALE_NORMAL);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe for testing

	glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDir);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteDir);
	glMaterialf(GL_FRONT, GL_SHININESS, 1.0f);

	glEnable(GL_SAMPLE_BUFFERS_ARB);

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

		tex0Loc = shader->getUniLoc("tex0");
		tex1Loc = shader->getUniLoc("tex1");
		tex2Loc = shader->getUniLoc("tex2");
		tex3Loc = shader->getUniLoc("tex3");
		colourMapLoc = shader->getUniLoc("colourMap");
		normalMapLoc = shader->getUniLoc("normalMap");

		tangentLoc = shader->getAttrLoc("vertTangent");

		autoDiffuseLoc = shader->getUniLoc("autoDiffuse");
		autoSpecularLoc = shader->getUniLoc("autoSpecular");
		
		//load3DTexture("goldmist.tx3");
		load3DTexture(LoadString2("config/renderer.xml","shader_texture"));
		loadTextures();
	}
	shader->off();

	resizeGL(width, height); // Make the world not suck
	initGround();
}

// When the window is resized update the height and width, and recalculate the aspect ratio
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
	gluPerspective(90.0f, ratio, 0.01f, 2000.0f); // 90 degree field of view
	glMatrixMode(GL_MODELVIEW);
}

void Renderer::updateCamera() {
	gluLookAt(camPos.getX(), camPos.getY(), camPos.getZ(),
				  camLook.getX(), camLook.getY(), camLook.getZ(),
				  camUp.getX(), camUp.getY(), camUp.getZ());

	GLfloat position[] = { lightPos.getX(), lightPos.getY(), lightPos.getZ(), 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
}

// Loads the data of a .tx3 file into the appropreate structures
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

// Loads a texture into the specified GL texture location
bool Renderer::loadTexture(string name, GLuint *texID) {
	SDL_Surface *surface;
	GLenum textureFormat;
	int numColors;
	 
	if ((surface = IMG_Load(name.c_str()))) { 
	 
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
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	 
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
	//drawGroundNormals();
	shader->on();
		applyShader();
		glColor3f(1,1,1);
		glActiveTexture(GL_TEXTURE4); // Apply the ground texture
		glBindTexture(GL_TEXTURE_2D, groundTex);
		glCallList(groundGeometry);
	shader->off();
}

// Does all the initial calculations for rendering the ground efficiently
void Renderer::initGround() {
	//loadTexture("testc_c.png", &groundTex);
	loadTexture("ground_wrap.bmp", &groundTex);		// Load the ground texture
	//loadTexture("sand_wrap_NRM.png", &groundBump);	// Load the ground bump map

	hm = HeightMapManager::GetHeightMap();
	

	xscale = LoadFloat("config/world.xml","height_map_scale_x");		// Load the scaling information
	yscale = LoadFloat("config/world.xml","height_map_scale_y");
	zscale = LoadFloat("config/world.xml","height_map_scale_z");

	btVector3 v1, v2, v3, v4, n, t;
	vector<vector<Point> > faceNormals;
	vector<vector<Point> > faceTangents;

	for (int x = 0; x < hm->width - 1; x++) { // Calculate all face normals and tangents
		vector<Point> row = vector<Point>();
		vector<Point> rowt = vector<Point>();
		for (int z = 0; z < hm->height - 1; z++) {
			v1 = btVector3((float)x * xscale, (float)(hm->map[x*hm->width+z]) * yscale, (float)z * zscale);
			v2 = btVector3((float)(x+1) * xscale, (float)(hm->map[(x+1)*hm->width+z]) * yscale, (float)z * zscale);
			v3 = btVector3((float)(x+1) * xscale, (float)(hm->map[(x+1)*hm->width+(z+1)]) * yscale, (float)(z+1) * zscale);
			n = (v1-v3).cross(v1-v2);
			t = (v1-v3).cross(n);
			row.push_back(Point(n.getX(), n.getY(), n.getZ()));
			rowt.push_back(Point(t.getX(), t.getY(), t.getZ()));
		}
		faceNormals.push_back(row);
		faceTangents.push_back(rowt);
	}

	for (int x = 0; x < hm->width; x++) { // Calculate and save all the vertex normals and tangents
		vector<Point> row = vector<Point>();
		vector<Point> rowt = vector<Point>();
		for (int z = 0; z < hm->height; z++) {
			v1 = btVector3((float)x * xscale, (float)(hm->map[x*hm->width+z]) * yscale, (float)z * zscale);
			v2 = btVector3((float)(x+1) * xscale, (float)(hm->map[(x+1)*hm->width+z]) * yscale, (float)z * zscale);
			v3 = btVector3((float)(x+1) * xscale, (float)(hm->map[(x+1)*hm->width+(z+1)]) * yscale, (float)(z+1) * zscale);
			v4 = btVector3((float)x * xscale, (float)(hm->map[x*hm->width+(z+1)]) * yscale, (float)(z+1) * zscale);
			n = (v1-v3).cross(v1-v2);
			t = (v1-v3).cross(n);

			if (x == 0 || x == hm->width - 1 || z == 0 || z == hm->height - 1) {
				row.push_back(Point(n.getX(), n.getY(), n.getZ()));
				rowt.push_back(Point(t.getX(), t.getY(), t.getZ()));
			} else {
				Point pn =  faceNormals.at(x-1).at(z-1)+
							faceNormals.at( x ).at(z-1)+
							faceNormals.at(x-1).at( z )+
							faceNormals.at( x ).at( z );
				Point pt =  faceTangents.at(x-1).at(z-1)+
							faceTangents.at( x ).at(z-1)+
							faceTangents.at(x-1).at( z )+
							faceTangents.at( x ).at( z );
				row.push_back(pn);
				rowt.push_back(pt);
			}
		}
		mapVertexNormals.push_back(row);
		mapVertexTangents.push_back(rowt);
	}

	groundGeometry = glGenLists(1); // Begin the ground geometry display list and draw the ground
	glNewList(groundGeometry, GL_COMPILE);
		glPushMatrix();
		glTranslated(-((float)(hm->width*xscale))/2.0, 0, -((float)(hm->height*zscale))/2.0); // centering of map
		//glTranslated(xscale/2.0, 0, zscale/2.0); // centering of tiles

			glBegin(GL_QUADS);
			for (int x = 0; x < hm->width - 1; x++) { // TODO: The faces have been reordered to accomodate backface culling. Doublecheck for issues
				for (int z = 0; z < hm->height - 1; z++) {
					v1 = btVector3((float)x * xscale, (float)(hm->map[x*hm->width+z]) * yscale, (float)z * zscale);
					v2 = btVector3((float)x * xscale, (float)(hm->map[x*hm->width+(z+1)]) * yscale, (float)(z+1) * zscale);
					v3 = btVector3((float)(x+1) * xscale, (float)(hm->map[(x+1)*hm->width+(z+1)]) * yscale, (float)(z+1) * zscale);
					v4 = btVector3((float)(x+1) * xscale, (float)(hm->map[(x+1)*hm->width+z]) * yscale, (float)z * zscale);
					n = (v1-v3).cross(v1-v2);

					Point pn =  mapVertexNormals.at(x).at(z+1);
					Point pt = 	mapVertexTangents.at(x).at(z+1);
					glNormal3f(pn.x, pn.y, pn.z);
					glVertexAttrib3f(tangentLoc, pt.x, pt.y, pt.z);
					groundTexCoord(x, z+1, false, true);
					glVertex3f(v2.getZ() + zscale/2, v2.getY(), v2.getX() + xscale/2);

					pn  = mapVertexNormals.at(x).at(z);
					pt = mapVertexTangents.at(x).at(z);
					glNormal3f(pn.x, pn.y, pn.z);
					glVertexAttrib3f(tangentLoc, pt.x, pt.y, pt.z);
					groundTexCoord(x, z, false, false);
					glVertex3f(v1.getZ() + zscale/2, v1.getY(), v1.getX() + xscale/2);
					
					pn = mapVertexNormals.at(x+1).at(z);
					pt = mapVertexTangents.at(x+1).at(z);
					glNormal3f(pn.x, pn.y, pn.z);
					glVertexAttrib3f(tangentLoc, pt.x, pt.y, pt.z);
					groundTexCoord(x+1, z, true, false);
					glVertex3f(v4.getZ() + zscale/2, v4.getY(), v4.getX() + xscale/2);
					
					pn = mapVertexNormals.at(x+1).at(z+1);
					pt = mapVertexTangents.at(x+1).at(z+1);
					glNormal3f(pn.x, pn.y, pn.z);
					glVertexAttrib3f(tangentLoc, pt.x, pt.y, pt.z);
					groundTexCoord(x+1, z+1, true, true);
					glVertex3f(v3.getZ() + zscale/2, v3.getY(), v3.getX() + xscale/2);
					
				}
			}
			glEnd();
		glPopMatrix();
	glEndList();
}

// Defines the texture coordinate of the ground at the given location
// xend and zend define which edge of the polygon this is, and therefore whether the texture coordinate should be 1 or 0 for propper wrapping
void Renderer::groundTexCoord(int x, int z, bool xend, bool zend) {
/*	float xc = ((float)(x%10))/10.0;
	float zc = ((float)(z%10))/10.0;
	if (xend && xc == 0) xc = 1;
	if (zend && zc == 0) zc = 1;*/
	float xc = (float)(x)/(float)(hm->width);
	float zc = (float)(z)/(float)(hm->height);
	glTexCoord2f(zc, xc);
}

// Draws the ground normal vectors for debugging
void Renderer::drawGroundNormals() {
	glPushMatrix();
		glTranslated(-((float)(hm->width*xscale))/2.0, 0, -((float)(hm->height*zscale))/2.0); // centering of map
		glTranslated(xscale/2.0, 0, zscale/2.0); // centering of tiles
		btVector3 p, v;
		Point pv;
		for (int x = 0; x < hm->width; x++) {
			for (int z = 0; z < hm->height; z++) {
				glBegin(GL_LINES);
				p = btVector3((float)x * xscale, (float)(hm->map[x*hm->width+z]) * yscale, (float)z * zscale);
				pv = mapVertexNormals.at(x).at(z);
				v = btVector3(pv.x, pv.y, pv.z).normalize()*3;
				v += p;
				glVertex3f(p.getZ(), p.getY(), p.getX());
				glVertex3f(v.getZ(), v.getY(), v.getX());
				glEnd();
			}
		}
	glPopMatrix();
}
