#include "Renderer.h"
#include <iostream>


Renderer::Renderer(IWindow const & window, const ActorList* actorList) {
	
	ratio = 1.0;
	width = window.ScreenWidth();
	height = window.ScreenHeight();

	camPos = btVector3(3,7,15);
	camLook = btVector3(1.5,0,5);
	camUp = btVector3(0,1,0);
	
	this->actorList = actorList;

	initializeGL();
	paintGL();
}

Renderer::~Renderer() { }


void Renderer::paintGL() {
	glLoadIdentity();

	updateCamera();
	//glMultMatrixf(trackball.getMatrix());
	renderObjects();
	
}

void Renderer::step() {
	paintGL();
}

void Renderer::renderObjects() {
	Actor* currentActor = 0;
	
	/*ActorList::iterator i;
	Province* current;
	for (i = map.getProvincesByColour()->begin(); i != map.getProvincesByColour()->end(); i++) {
		pd.setValue(pd.value()+1);
		current = i.value();
		current->findEdges();
	}
	for (int i = 0; i < actorList->size(); i++) {
		currentActor = actorList->
		glPushMatrix();

		glTranslated(currentActor->pos.x, currentActor->pos.y, currentActor->pos.z); // Something meaningful will go here later

		btVector3 axis = currentActor->orientation.getAxis();
		glRotated(currentActor->orientation.getAngle(), axis.getX(), axis.getY(), axis.getZ());
		/*new btVector3 h = currentActor->heading;
		new btVector3 b = currentActor->binormal;
		new btVector3 n = currentActor->normal;*/

		/*****AXES FOR TESTING*****
		glColor3f(1,0,0);
		glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(h.x, h.y, h.z);
		glEnd();

		glColor3f(0,1,0);
		glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(b.x, b.y, b.z);
		glEnd();

		glColor3f(0,0,1);
		glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(n.x, n.y, n.z);
		glEnd();
		/**************************/

		//GLfloat frameMatrix[16] = { h.x, h.y, h.z, 0, n.x, n.y, n.z, 0, b.x, b.y, b.z, 0, 0, 0, 0, 1};
		//glMultMatrixf(frameMatrix);

	//	shader->on();
			glColor3f(1,1,1);
			drawCube(new btVector3( currentActor->height/2, -currentActor->width/2, -currentActor->depth/2),
					 new btVector3( currentActor->height/2,  currentActor->width/2, -currentActor->depth/2),
					 new btVector3( currentActor->height/2, -currentActor->width/2,  currentActor->depth/2),
					 new btVector3( currentActor->height/2,  currentActor->width/2,  currentActor->depth/2),
					 new btVector3(-currentActor->height/2, -currentActor->width/2, -currentActor->depth/2),
					 new btVector3(-currentActor->height/2,  currentActor->width/2, -currentActor->depth/2),
					 new btVector3(-currentActor->height/2, -currentActor->width/2,  currentActor->depth/2),
					 new btVector3(-currentActor->height/2,  currentActor->width/2,  currentActor->depth/2));
	//	shader->off();

		glPopMatrix();
	}
}

void Renderer::initializeGL() {
	glClearColor(0, 0, 0, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL the functable
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	GLfloat ambientLight[] = { 0.1, 0.1, 0.1, 1 };
	GLfloat diffuseLight[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat specularLight[] = { 0.3, 0.3, 0.3, 1 };
	GLfloat position[] = { 5, 5, -5, 1 };
	 
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	
	GLfloat position1[] = { -5, 5, 0, 1 };
	 
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_CULL_FACE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	resizeGL(width, height); // Make the world not suck
	updateCamera();
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
}

void Renderer::drawQuad(btVector3* tl, btVector3* tr, btVector3* bl, btVector3* br) {
	glBegin(GL_QUADS);
	btVector3 normal = (*tl-*bl).cross(*tl-*tr);
	glNormal3d(normal.getX(), normal.getY(), normal.getZ());
	glTexCoord2f(1, 0);
	glVertex3d(tr->getX(), tr->getY(), tr->getZ()); // Top Right Corner
	glTexCoord2f(1, 1);
	glVertex3d(tl->getX(), tl->getY(), tl->getZ()); // Top Left Corner
	glTexCoord2f(0, 0);
	glVertex3d(bl->getX(), bl->getY(), bl->getZ()); // Bottom Right Corner
	glTexCoord2f(0, 1);
	glVertex3d(br->getX(), br->getY(), br->getZ()); // Bottom Left Corner
	glEnd();
}

void Renderer::drawCube(btVector3* tlb, btVector3* trb, btVector3* tlf, btVector3* trf, btVector3* blb, btVector3* brb, btVector3* blf, btVector3* brf) {
	drawQuad(tlb, trb, tlf, trf); // Top face
	drawQuad(brb, blb, brf, blf); // Bottom face
	
	drawQuad(tlf, blf, tlb, blb); // Left face
	drawQuad(trb, brb, trf, brf); // Right face

	drawQuad(tlf, trf, blf, brf); // Front face
	drawQuad(blb, brb, tlb, trb); // Back face
}