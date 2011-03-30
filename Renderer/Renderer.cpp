#include "Renderer.h"
#include <iostream>
#include "Physics/HeightMapManager.h"
#include "Common/SettingsFactory.h"

//#define LOW_RES

Renderer * Renderer::ptr = NULL;

Renderer::Renderer(IWindow const & window, ActorList const & actorList, JeepManager * jeepManager) : 
actorList(actorList), jeepManager(jeepManager) {

    if(ptr == NULL)     //singleton
        ptr = this;

	hm = 0;

	ratio = 1.0;
	width = window.ScreenWidth();
	height = window.ScreenHeight();

	camUp = btVector3(0,1,0);
	showMessage = false;
	setMessage("");

	lightPos = btVector3(500,15000,240);
    initializeGL();
    paintGL();
}

void Renderer::initialize() {
    shaderTexturesG.resize(MAX_TEXTURES);
	shaderTexturesO.resize(MAX_TEXTURES);
	for (int i = 0; i < MAX_TEXTURES; i++)
		shaderTexturesG[i] = new GLuint;
	for (int i = 0; i < MAX_TEXTURES; i++)
		shaderTexturesO[i] = new GLuint;

	//placeNumbers.resize(LoadInt("config/ai.xml","num_players")+1);
	placeNumbers.resize(11);
	for (int i = 0; i < 11; i++)
		placeNumbers[i] = new GLuint;
	loadTexture("data\\UI\\p1.png", placeNumbers[0]);
	loadTexture("data\\UI\\p2.png", placeNumbers[1]);
	loadTexture("data\\UI\\p3.png", placeNumbers[2]);
	loadTexture("data\\UI\\p4.png", placeNumbers[3]);
	loadTexture("data\\UI\\p5.png", placeNumbers[4]);
	loadTexture("data\\UI\\p6.png", placeNumbers[5]);
	loadTexture("data\\UI\\p7.png", placeNumbers[6]);
	loadTexture("data\\UI\\p8.png", placeNumbers[7]);
	loadTexture("data\\UI\\p9.png", placeNumbers[8]);
	loadTexture("data\\UI\\p10.png", placeNumbers[9]);
	loadTexture("data\\UI\\p11.png", placeNumbers[10]);

	attrDataG = new AttributeData(); // Ground shader data
	texDataG = new TextureData(3);
	optDataG = new OptionsData();

	attrDataO = new AttributeData(); // Object shader data
	texDataO = new TextureData(4);
	optDataO = new OptionsData();
	
	initializeGL2();
    paintGL();
}

void Renderer::setCamera(btVector3 const & pos, btVector3 const & look, btVector3 const & up) {
	camPos = pos;
	camLook = look;
	camUp = up;
}

Renderer::~Renderer() { }


void Renderer::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	updateCamera();
    if(jeepManager != NULL) //done loading
    {
        glDisable(GL_DEPTH_TEST); // Ignore depth for the sky so that it is always drawn behind everything else
    		drawSky();
    	glEnable(GL_DEPTH_TEST);
    	
	    drawGround();
    	renderObjects();
    	renderJeeps();
    }

	if (jeepManager != NULL) drawPlayerPlace(jeepManager->getPlayerPlace(LoadInt("config/ai.xml","num_players")));
	if (showMessage) drawMessage();
	
	// TODO: Better checking
}

void Renderer::step() {
	paintGL();
}

void Renderer::renderJeeps() {

	Actor* currentActor = 0;
	for (int i = 0; i < jeepManager->getAIs().size(); i++) {
		currentActor = jeepManager->getAIs().at(i);
		glPushMatrix();

		glTranslated(currentActor->pos.getX(), currentActor->pos.getY(), currentActor->pos.getZ());

		btVector3 h = quatRotate(currentActor->orientation, btVector3(1,0,0));
		btVector3 b = quatRotate(currentActor->orientation, btVector3(0,0,1));
		btVector3 n = quatRotate(currentActor->orientation, btVector3(0,1,0));
		h.normalize();
		b.normalize();
		n.normalize();

		// This matrix is defined columnwise
		GLfloat frameMatrix[16] = { h.getX(), h.getY(), h.getZ(), 0, 
									n.getX(), n.getY(), n.getZ(), 0, 
									b.getX(), b.getY(), b.getZ(), 0, 
									0, 0, 0, 1};
		glMultMatrixf(frameMatrix);

		glActiveTexture(GL_TEXTURE4); // Apply the current actor's texture
		glBindTexture(GL_TEXTURE_2D, currentActor->renderObject->texture);
		glActiveTexture(GL_TEXTURE6); // Apply the current actor's bump map
		glBindTexture(GL_TEXTURE_2D, currentActor->renderObject->bumpMap);
		objectShader->on();
			applyObjectShader();

			glColor3f(1,1,1);
			currentActor->renderObject->draw();
			//currentActor->renderObject.drawNormals();

			// Clear all textures
			for (int i = MAX_TEXTURES+2; i >= 0; i--) {
				glActiveTexture(GL_TEXTURE0+i);
				glDisable(GL_TEXTURE_2D);
			}
		objectShader->off();

		glPopMatrix();
	}
	// Draw the human controlled jeep separately
	currentActor = jeepManager->getHuman();
	glPushMatrix();

	glTranslated(currentActor->pos.getX(), currentActor->pos.getY(), currentActor->pos.getZ());

	btVector3 h = quatRotate(currentActor->orientation, btVector3(1,0,0));
	btVector3 b = quatRotate(currentActor->orientation, btVector3(0,0,1));
	btVector3 n = quatRotate(currentActor->orientation, btVector3(0,1,0));
	h.normalize();
	b.normalize();
	n.normalize();

	// This matrix is defined columnwise
	GLfloat frameMatrix[16] = { h.getX(), h.getY(), h.getZ(), 0, 
								n.getX(), n.getY(), n.getZ(), 0, 
								b.getX(), b.getY(), b.getZ(), 0, 
								0, 0, 0, 1};
	glMultMatrixf(frameMatrix);

	glActiveTexture(GL_TEXTURE4); // Apply the current actor's texture
	glBindTexture(GL_TEXTURE_2D, currentActor->renderObject->texture);
	glActiveTexture(GL_TEXTURE6); // Apply the current actor's bump map
	glBindTexture(GL_TEXTURE_2D, currentActor->renderObject->bumpMap);
	objectShader->on();
		applyObjectShader();

		glColor3f(1,1,1);
		currentActor->renderObject->draw();
		//currentActor->renderObject.drawNormals();

		// Clear all textures
		for (int i = MAX_TEXTURES+2; i >= 0; i--) {
			glActiveTexture(GL_TEXTURE0+i);
			glDisable(GL_TEXTURE_2D);
		}
	objectShader->off();

	glPopMatrix();
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
		h.normalize();
		b.normalize();
		n.normalize();

		// This matrix is defined columnwise
		GLfloat frameMatrix[16] = { h.getX(), h.getY(), h.getZ(), 0, 
									n.getX(), n.getY(), n.getZ(), 0, 
									b.getX(), b.getY(), b.getZ(), 0, 
									0, 0, 0, 1};
		glMultMatrixf(frameMatrix);

		glActiveTexture(GL_TEXTURE4); // Apply the current actor's texture
		glBindTexture(GL_TEXTURE_2D, currentActor->renderObject->texture);
		glActiveTexture(GL_TEXTURE6); // Apply the current actor's bump map
		glBindTexture(GL_TEXTURE_2D, currentActor->renderObject->bumpMap);
		objectShader->on();
			applyObjectShader();
			
			GLfloat texPos[MAX_TEXTURES];
			vector<int> index = texDataG->getTextureIndices();
			for (unsigned int i = 0; i < MAX_TEXTURES; i++) {
				if (i < index.size()) {
					texPos[i] = texDataG->getTexturePos(index[i]);

					glActiveTexture(GL_TEXTURE0+i);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, *shaderTexturesG[index[i]]);
				}
				else
					texPos[i] = 5.0f;
			}

			glColor3f(1,1,1);
			currentActor->renderObject->draw();
			//currentActor->renderObject.drawNormals();

			// Clear all textures
			for (int i = MAX_TEXTURES+2; i >= 0; i--) {
				glActiveTexture(GL_TEXTURE0+i);
				glDisable(GL_TEXTURE_2D);
			}
		objectShader->off();

		glPopMatrix();
	}
}

void Renderer::setMessage(string const & texName) {
	if (texName.compare("") == 0) showMessage = false;
	else {
		loadTexture(texName, &messageTex);
		showMessage = true;
	}
}

void Renderer::loadingMessage(string const & textName) {
	setMessage(textName);
	setUIProjection();

	glColor4f(1,1,1,1);
    basicShader->on();
	glActiveTexture(GL_TEXTURE4); // Apply the sky texture
	
	glBindTexture(GL_TEXTURE_2D, messageTex);
	glUniform1i(basicShaderTexLocS, 4);
	glPushMatrix();
		glLoadIdentity();

		glBegin(GL_QUADS);
		glTexCoord2f(1, 1);
		glVertex3f(width, height, 0);
		glTexCoord2f(0, 1);
		glVertex3f(0, height, 0);
		glTexCoord2f(0, 0);
		glVertex3f(0, 0, 0);
		glTexCoord2f(1, 0);
		glVertex3f(width, 0, 0);

		glEnd();
	glPopMatrix();
    basicShader->off();

	setProjection();
}

void Renderer::drawMessage() {
	setUIProjection();

	glColor4f(1,1,1,1);
    basicShader->on();
	glActiveTexture(GL_TEXTURE4); // Apply the sky texture
	glBindTexture(GL_TEXTURE_2D, messageTex);
	glUniform1i(basicShaderTexLocS, 4);
	glPushMatrix();
		glLoadIdentity();

		glBegin(GL_QUADS);
		glTexCoord2f(1, 1);
		glVertex3f(width, height, 0);
		glTexCoord2f(0, 1);
		glVertex3f(0, height, 0);
		glTexCoord2f(0, 0);
		glVertex3f(0, 0, 0);
		glTexCoord2f(1, 0);
		glVertex3f(width, 0, 0);

		glEnd();
	glPopMatrix();
    basicShader->off();

	setProjection();
}

void Renderer::drawPlayerPlace(int place) {

	setUIProjection();

	glColor4f(1,1,1,1);
    basicShader->on();
	glActiveTexture(GL_TEXTURE4); // Apply the sky texture
	glBindTexture(GL_TEXTURE_2D, *placeNumbers[min(place-1, 10)]);
	glUniform1i(basicShaderTexLocS, 4);
	glPushMatrix();
		glLoadIdentity();

		glBegin(GL_QUADS);
		glTexCoord2f(1, 1);
		glVertex3f((double)width/10.0, (double)height/8.0, 0);
		glTexCoord2f(0, 1);
		glVertex3f(0, (double)height/8.0, 0);
		glTexCoord2f(0, 0);
		glVertex3f(0, 0, 0);
		glTexCoord2f(1, 0);
		glVertex3f((double)width/10.0, 0, 0);

		glEnd();
	glPopMatrix();
    basicShader->off();

	setProjection();
}

void Renderer::applyGroundShader() {
	vector<int> index = texDataG->getTextureIndices();

	// Set all the uniforms in the shaders to the values supplied by the attribute data structure
	glUniform1i(xAttrLocG, (int) attrDataG->xAttr);
	glUniform1f(xModLocG, attrDataG->xMod);
	glUniform1f(xZminLocG, attrDataG->xZmin);
	glUniform1i(xFlipLocG, (int) attrDataG->xFlip);

	glUniform1i(yAttrLocG, (int) attrDataG->yAttr);
	glUniform1f(yModLocG, attrDataG->yMod);
	glUniform1f(yZminLocG, attrDataG->yZmin);
	glUniform1i(yFlipLocG, (int) attrDataG->yFlip);

	glUniform1i(zAttrLocG, (int) attrDataG->zAttr);
	glUniform1f(zModLocG, attrDataG->zMod);
	glUniform1f(zZminLocG, attrDataG->zZmin);
	glUniform1i(zFlipLocG, (int) attrDataG->zFlip);

	glUniform1i(numTexLocG, index.size());
	
	glUniform3f(jeep0ShadowPosLocG, jeepManager->getPlayerPos(0).getX(), jeepManager->getPlayerPos(0).getY(), jeepManager->getPlayerPos(0).getZ());
	glUniform3f(jeep1ShadowPosLocG, jeepManager->getPlayerPos(1).getX(), jeepManager->getPlayerPos(1).getY(), jeepManager->getPlayerPos(1).getZ());
	glUniform3f(jeep2ShadowPosLocG, jeepManager->getPlayerPos(2).getX(), jeepManager->getPlayerPos(2).getY(), jeepManager->getPlayerPos(2).getZ());
	glUniform3f(jeep3ShadowPosLocG, jeepManager->getPlayerPos(3).getX(), jeepManager->getPlayerPos(3).getY(), jeepManager->getPlayerPos(3).getZ());
	glUniform3f(jeep4ShadowPosLocG, jeepManager->getPlayerPos(4).getX(), jeepManager->getPlayerPos(4).getY(), jeepManager->getPlayerPos(4).getZ());
	glUniform3f(jeep5ShadowPosLocG, jeepManager->getPlayerPos(5).getX(), jeepManager->getPlayerPos(5).getY(), jeepManager->getPlayerPos(5).getZ());
	glUniform3f(jeep6ShadowPosLocG, jeepManager->getPlayerPos(6).getX(), jeepManager->getPlayerPos(6).getY(), jeepManager->getPlayerPos(6).getZ());
	glUniform3f(jeep7ShadowPosLocG, jeepManager->getPlayerPos(7).getX(), jeepManager->getPlayerPos(7).getY(), jeepManager->getPlayerPos(7).getZ());
	glUniform3f(jeep8ShadowPosLocG, jeepManager->getPlayerPos(8).getX(), jeepManager->getPlayerPos(8).getY(), jeepManager->getPlayerPos(8).getZ());
	glUniform3f(jeep9ShadowPosLocG, jeepManager->getPlayerPos(9).getX(), jeepManager->getPlayerPos(9).getY(), jeepManager->getPlayerPos(9).getZ());
	glUniform3f(jeep10ShadowPosLocG, jeepManager->getPlayerPos(10).getX(), jeepManager->getPlayerPos(10).getY(), jeepManager->getPlayerPos(10).getZ());

	GLfloat texPos[MAX_TEXTURES];
	GLfloat texHSkew[MAX_TEXTURES];
	GLfloat texVSkew[MAX_TEXTURES];
	GLint texInterp[MAX_TEXTURES];

	// For each texture, pass the texture data for this texture from the texture data structure
	for (unsigned int i = 0; i < MAX_TEXTURES; i++) {
		if (i < index.size()) {
			texPos[i] = texDataG->getTexturePos(index[i]);
			texHSkew[i] = texDataG->getTextureHSkew(index[i]);
			texVSkew[i] = texDataG->getTextureVSkew(index[i]);

			glActiveTexture(GL_TEXTURE0+i);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, *shaderTexturesG[index[i]]);
		}
		else {
			texPos[i] = 5.0f;
			texHSkew[i] = 0.0f;
			texVSkew[i] = 0.0f;
			texInterp[i] = 1;
		}
	}

	// Pass the texture indices into the shader
	glUniform1i(tex0LocG, 0);
	glUniform1i(tex1LocG, 1);
	glUniform1i(tex2LocG, 2);
	glUniform1i(tex3LocG, 3);
	glUniform1i(groundTexLocG, 4);
	glUniform1i(cliffTexLocG, 5);

	glActiveTexture(GL_TEXTURE6); // Apply the bump map, right now, the same one for everything :-o
	glBindTexture(GL_TEXTURE_2D, groundBump);
	glActiveTexture(GL_TEXTURE7); // Apply the bump map, right now, the same one for everything :-o
	glBindTexture(GL_TEXTURE_2D, cliffBump);

	glUniform1i(groundNormalMapLocG, 6);
	glUniform1i(cliffNormalMapLocG, 7);

	glUniform1fv(texPosLocG, MAX_TEXTURES, texPos);
	glUniform1fv(texHskewLocG, MAX_TEXTURES, texHSkew);
	glUniform1fv(texVskewLocG, MAX_TEXTURES, texVSkew);

	// Auto shading options
	glUniform1i(autoDiffuseLocG, (int) optDataG->autoDiffuse);
	glUniform1i(autoSpecularLocG, (int) optDataG->autoSpecular);

	glActiveTexture(GL_TEXTURE0+MAX_TEXTURES);
}

// TODO: This is a bit repetative
void Renderer::applyObjectShader() {
	vector<int> index = texDataO->getTextureIndices();

	// Set all the uniforms in the shaders to the values supplied by the attribute data structure
	glUniform1i(xAttrLocO, (int) attrDataO->xAttr);
	glUniform1f(xModLocO, attrDataO->xMod);
	glUniform1f(xZminLocO, attrDataO->xZmin);
	glUniform1i(xFlipLocO, (int) attrDataO->xFlip);

	glUniform1i(yAttrLocO, (int) attrDataO->yAttr);
	glUniform1f(yModLocO, attrDataO->yMod);
	glUniform1f(yZminLocO, attrDataO->yZmin);
	glUniform1i(yFlipLocO, (int) attrDataO->yFlip);

	glUniform1i(zAttrLocO, (int) attrDataO->zAttr);
	glUniform1f(zModLocO, attrDataO->zMod);
	glUniform1f(zZminLocO, attrDataO->zZmin);
	glUniform1i(zFlipLocO, (int) attrDataO->zFlip);

	glUniform1i(numTexLocO, index.size());

	GLfloat texPos[MAX_TEXTURES];
	GLfloat texHSkew[MAX_TEXTURES];
	GLfloat texVSkew[MAX_TEXTURES];
	GLint texInterp[MAX_TEXTURES];

	// For each texture, pass the texture data for this texture from the texture data structure
	for (unsigned int i = 0; i < MAX_TEXTURES; i++) {
		if (i < index.size()) {
			texPos[i] = texDataO->getTexturePos(index[i]);
			texHSkew[i] = texDataO->getTextureHSkew(index[i]);
			texVSkew[i] = texDataO->getTextureVSkew(index[i]);

			glActiveTexture(GL_TEXTURE0+i);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, *shaderTexturesO[index[i]]);
		}
		else {
			texPos[i] = 5.0f;
			texHSkew[i] = 0.0f;
			texVSkew[i] = 0.0f;
			texInterp[i] = 1;
		}
	}

	// Pass the texture indices into the shader
	glUniform1i(tex0LocO, 0);
	glUniform1i(tex1LocO, 1);
	glUniform1i(tex2LocO, 2);
	glUniform1i(tex3LocO, 3);
	glUniform1i(colourMapLocO, 4);

	glUniform1fv(texPosLocO, MAX_TEXTURES, texPos);
	glUniform1fv(texHskewLocO, MAX_TEXTURES, texHSkew);
	glUniform1fv(texVskewLocO, MAX_TEXTURES, texVSkew);

	// Auto shading options
	glUniform1i(autoDiffuseLocO, (int) optDataO->autoDiffuse);
	glUniform1i(autoSpecularLocO, (int) optDataO->autoSpecular);

	glActiveTexture(GL_TEXTURE0+MAX_TEXTURES);
}

void Renderer::initializeGL() {
	//glClearColor(0.94, 0.97, 0.97, 0);
	glClearColor(1, 0, 0.5, 0);

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

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_COLOR_MATERIAL);
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
	resizeGL(width, height); // Make the world not suck
    
	GLenum err3 = glewInit();
	if (GLEW_OK == err3) {
		basicShader = new Shader(LoadString2("config/renderer.xml","basic_shader_f").c_str(),
			LoadString2("config/renderer.xml","basic_shader_v").c_str());
		basicShaderTexLocS = basicShader->getUniLoc("texMap");
	}
	basicShader->off();
}

void Renderer::initializeGL2()  //more advanced stuff
{
    GLenum err = glewInit();
	if (GLEW_OK == err) {
		groundShader = new Shader(LoadString2("config/renderer.xml","ground_shader_f").c_str(),
			LoadString2("config/renderer.xml","ground_shader_v").c_str());
		xAttrLocG = groundShader->getUniLoc("xAttr");
		xModLocG = groundShader->getUniLoc("xMod");
		xZminLocG = groundShader->getUniLoc("xZMin");
		xFlipLocG = groundShader->getUniLoc("xFlip");

		yAttrLocG = groundShader->getUniLoc("yAttr");
		yModLocG = groundShader->getUniLoc("yMod");
		yZminLocG = groundShader->getUniLoc("yZMin");
		yFlipLocG = groundShader->getUniLoc("yFlip");

		zAttrLocG = groundShader->getUniLoc("zAttr");
		zModLocG = groundShader->getUniLoc("zMod");
		zZminLocG = groundShader->getUniLoc("zZMin");
		zFlipLocG = groundShader->getUniLoc("zFlip");

		numTexLocG = groundShader->getUniLoc("numTex");
		texPosLocG = groundShader->getUniLoc("texPos");
		texHskewLocG = groundShader->getUniLoc("texHSkew");
		texVskewLocG = groundShader->getUniLoc("texVSkew");

		tex0LocG = groundShader->getUniLoc("tex0");
		tex1LocG = groundShader->getUniLoc("tex1");
		tex2LocG = groundShader->getUniLoc("tex2");
		tex3LocG = groundShader->getUniLoc("tex3");
		groundTexLocG = groundShader->getUniLoc("groundTex");
		cliffTexLocG = groundShader->getUniLoc("cliffTex");
		groundNormalMapLocG = groundShader->getUniLoc("groundNormalMap");
		cliffNormalMapLocG = groundShader->getUniLoc("cliffNormalMap");

		jeep0ShadowPosLocG = groundShader->getUniLoc("jeep0shadowPos");
		jeep1ShadowPosLocG = groundShader->getUniLoc("jeep1shadowPos");
		jeep2ShadowPosLocG = groundShader->getUniLoc("jeep2shadowPos");
		jeep3ShadowPosLocG = groundShader->getUniLoc("jeep3shadowPos");
		jeep4ShadowPosLocG = groundShader->getUniLoc("jeep4shadowPos");
		jeep5ShadowPosLocG = groundShader->getUniLoc("jeep5shadowPos");
		jeep6ShadowPosLocG = groundShader->getUniLoc("jeep6shadowPos");
		jeep7ShadowPosLocG = groundShader->getUniLoc("jeep7shadowPos");
		jeep8ShadowPosLocG = groundShader->getUniLoc("jeep8shadowPos");
		jeep9ShadowPosLocG = groundShader->getUniLoc("jeep9shadowPos");
		jeep10ShadowPosLocG = groundShader->getUniLoc("jeep10shadowPos");

		tangentLocG = groundShader->getAttrLoc("vertTangent");

		autoDiffuseLocG = groundShader->getUniLoc("autoDiffuse");
		autoSpecularLocG = groundShader->getUniLoc("autoSpecular");
		
		loadGround3DTexture(LoadString2("config/renderer.xml","ground_shader_texture"));
		loadGroundTextures();
	}
	groundShader->off();

	GLenum err2 = glewInit();
	if (GLEW_OK == err2) {
		objectShader = new Shader(LoadString2("config/renderer.xml","object_shader_f").c_str(),
			LoadString2("config/renderer.xml","object_shader_v").c_str());
		xAttrLocO = objectShader->getUniLoc("xAttr");
		xModLocO = objectShader->getUniLoc("xMod");
		xZminLocO = objectShader->getUniLoc("xZMin");
		xFlipLocO = objectShader->getUniLoc("xFlip");

		yAttrLocO = objectShader->getUniLoc("yAttr");
		yModLocO = objectShader->getUniLoc("yMod");
		yZminLocO = objectShader->getUniLoc("yZMin");
		yFlipLocO = objectShader->getUniLoc("yFlip");

		zAttrLocO = objectShader->getUniLoc("zAttr");
		zModLocO = objectShader->getUniLoc("zMod");
		zZminLocO = objectShader->getUniLoc("zZMin");
		zFlipLocO = objectShader->getUniLoc("zFlip");

		numTexLocO = objectShader->getUniLoc("numTex");
		texPosLocO = objectShader->getUniLoc("texPos");
		texHskewLocO = objectShader->getUniLoc("texHSkew");
		texVskewLocO = objectShader->getUniLoc("texVSkew");

		tex0LocO = objectShader->getUniLoc("tex0");
		tex1LocO = objectShader->getUniLoc("tex1");
		tex2LocO = objectShader->getUniLoc("tex2");
		tex3LocO = objectShader->getUniLoc("tex3");
		colourMapLocO = objectShader->getUniLoc("colourMap");
		normalMapLocO = objectShader->getUniLoc("normalMap");

		tangentLocO = objectShader->getAttrLoc("vertTangent");

		autoDiffuseLocO = objectShader->getUniLoc("autoDiffuse");
		autoSpecularLocO = objectShader->getUniLoc("autoSpecular");
		
		loadObject3DTexture(LoadString2("config/renderer.xml","object_shader_texture"));
		loadObjectTextures();
	}
	objectShader->off();

	initSky();
	initGround();
}

// When the window is resized update the height and width, and recalculate the aspect ratio
void Renderer::resizeGL(int w, int h) {
	width = w;
	height = h;
	ratio = (double)width / (double)height;

	setProjection();
}

void Renderer::setUIProjection() {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, 1, -1);
	glMatrixMode(GL_MODELVIEW);
}

void Renderer::setProjection() {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, ratio, 0.1f, 1500.0f); // 90 degree field of view
	//gluPerspective(90.0f, ratio, 100.0f, 15000.0f); // for overhead view (testing)
	glMatrixMode(GL_MODELVIEW);
}

void Renderer::updateCamera() {
	gluLookAt(camPos.getX(), camPos.getY(), camPos.getZ(),
				  camLook.getX(), camLook.getY(), camLook.getZ(),
				  camUp.getX(), camUp.getY(), camUp.getZ());
	
	//gluLookAt(0, 3000, 0, 0, 0, 0, 0, 0, -1); // Overhead camera for testing

	GLfloat position[] = { lightPos.getX(), lightPos.getY(), lightPos.getZ(), 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
}

// Loads the data of a .tx3 file into the data structures
void Renderer::loadGround3DTexture(string filename) {
	texDataG->load(filename);
	attrDataG->load(filename);
	optDataG->load(filename);
}

// Loads the data of a .tx3 file into the data structures
void Renderer::loadObject3DTexture(string filename) {
	texDataO->load(filename);
	attrDataO->load(filename);
	optDataO->load(filename);
}

void Renderer::loadGroundTextures() {
	vector<int> index = texDataG->getTextureIndices();
	for (unsigned int i = 0; i < index.size(); i++)
		loadTexture(texDataG->getTextureFilename(index[i]), shaderTexturesG[index[i]]);
}

void Renderer::loadObjectTextures() {
	vector<int> index = texDataO->getTextureIndices();
	for (unsigned int i = 0; i < index.size(); i++)
		loadTexture(texDataO->getTextureFilename(index[i]), shaderTexturesO[index[i]]);
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

void Renderer::drawSky() {
	glMatrixMode(GL_PROJECTION); // Change the projection so that the skybox is visible
	glLoadIdentity();
	gluPerspective(90.0f, ratio, 60.0f, 6000.0f);
	glMatrixMode(GL_MODELVIEW);

	basicShader->on();
		glColor3f(1,1,1);
		glActiveTexture(GL_TEXTURE4); // Apply the sky texture
		glBindTexture(GL_TEXTURE_2D, sky.texture);
		glUniform1i(basicShaderTexLocS, 4);
		glPushMatrix();
			//glTranslated(434, 290, -1736);
			sky.draw();
			//sky.drawNormals();
		glPopMatrix();
	basicShader->off();

	setProjection();
}

// Does all the initial calculations for rendering the ground efficiently
void Renderer::initSky() {
	sky = RenderObject(LoadString2("config/renderer.xml", "sky_texture"), "data/textures/blank.bmp", "models/dome.obj", 2000);
}

void Renderer::drawGround() {
	//drawGroundNormals();
	groundShader->on();
		applyGroundShader();
		glColor3f(1,1,1);
		glActiveTexture(GL_TEXTURE4); // Apply the ground textures
		glBindTexture(GL_TEXTURE_2D, groundTex);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, cliffTex);
		glCallList(groundGeometry);
	groundShader->off();
}

// Does all the initial calculations for rendering the ground efficiently
void Renderer::initGround() {
	#ifndef LOW_RES
	loadTexture(LoadString2("config/renderer.xml", "ground_texture"), &groundTex);		// Load the ground texture
	loadTexture(LoadString2("config/renderer.xml", "cliff_texture"), &cliffTex);		// Load the cliff texture
	loadTexture(LoadString2("config/renderer.xml", "ground_bump"), &groundBump);		// Load the ground bump map
	loadTexture(LoadString2("config/renderer.xml", "cliff_bump"), &cliffBump);			// Load the cliff bump map
	#else
	loadTexture(LoadString2("config/renderer.xml", "ground_texture_small"), &groundTex);// Load the ground texture
	loadTexture(LoadString2("config/renderer.xml", "cliff_texture_small"), &cliffTex);	// Load the cliff texture
	loadTexture(LoadString2("config/renderer.xml", "ground_bump_small"), &groundBump);	// Load the ground bump map
	loadTexture(LoadString2("config/renderer.xml", "cliff_bump_small"), &groundBump);	// Load the cliff bump map
	#endif

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
				row.push_back(pn*-1);
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
					glVertexAttrib3f(tangentLocG, pt.x, pt.y, pt.z);
					groundTexCoord(x, z+1, false, true);
					glVertex3f(v2.getZ() + zscale/2, v2.getY(), v2.getX() + xscale/2);

					pn  = mapVertexNormals.at(x).at(z);
					pt = mapVertexTangents.at(x).at(z);
					glNormal3f(pn.x, pn.y, pn.z);
					glVertexAttrib3f(tangentLocG, pt.x, pt.y, pt.z);
					groundTexCoord(x, z, false, false);
					glVertex3f(v1.getZ() + zscale/2, v1.getY(), v1.getX() + xscale/2);
					
					pn = mapVertexNormals.at(x+1).at(z);
					pt = mapVertexTangents.at(x+1).at(z);
					glNormal3f(pn.x, pn.y, pn.z);
					glVertexAttrib3f(tangentLocG, pt.x, pt.y, pt.z);
					groundTexCoord(x+1, z, true, false);
					glVertex3f(v4.getZ() + zscale/2, v4.getY(), v4.getX() + xscale/2);
					
					pn = mapVertexNormals.at(x+1).at(z+1);
					pt = mapVertexTangents.at(x+1).at(z+1);
					glNormal3f(pn.x, pn.y, pn.z);
					glVertexAttrib3f(tangentLocG, pt.x, pt.y, pt.z);
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
	float xc = ((float)(x%5))/5.0;
	float zc = ((float)(z%5))/5.0;
	if (xend && xc == 0) xc = 1;
	if (zend && zc == 0) zc = 1;
	//float xc = (float)(x)/(float)(hm->width);
	//float zc = (float)(z)/(float)(hm->height);
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
