#include "Renderer.h"
#include <iostream>
#include "Common/SettingsFactory.h"
#include "Driving/JeepActor.h"


Renderer::Renderer(IWindow const & window) {
	
	ratio = 1.0;
	width = window.ScreenWidth();
	height = window.ScreenHeight();

	//camPos = Point(0,17,1);
	//camLook = Point(0,0,0);
	//camUp = Vector3(1,0,0);
	
	float const & camPosX = LoadFloat("config/camera.xml", "camPosX");
	float const & camPosY = LoadFloat("config/camera.xml", "camPosY");
	float const & camPosZ = LoadFloat("config/camera.xml", "camPosZ");
	float const & camLookX = LoadFloat("config/camera.xml", "camLookX");
	float const & camLookY = LoadFloat("config/camera.xml", "camLookY");
	float const & camLookZ = LoadFloat("config/camera.xml", "camLookZ");
	float const & camUpX = LoadFloat("config/camera.xml", "camUpX");
	float const & camUpY = LoadFloat("config/camera.xml", "camUpY");
	float const & camUpZ = LoadFloat("config/camera.xml", "camUpZ");

	camPos = Point(camPosX,camPosY,camPosZ);
	camLook = Point(camLookX,camLookY,camLookZ);
	camUp = Vector3(camUpX,camUpY,camUpZ);
	
	trackball = Trackball(75);
	initializeGL();
	paintGL();
}

Renderer::~Renderer() { }


void Renderer::paintGL() {
	glLoadIdentity();

	//updateCamera();
	
}

void Renderer::step()
{
	
	paintGL();
}

void Renderer::renderObjects() {
	glBegin(GL_QUADS);
	drawCube(Point( 1, 1, -1), Point( 1, -1, -1), Point( 1, 1, 1), Point( 1, -1, 1),
			 Point(-1, 1, -1), Point(-1, -1, -1), Point(-1, 1, 1), Point(-1, -1, 1));
	glEnd();
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
	gluLookAt(camPos.x, camPos.y, camPos.z,
		camLook.x, camLook.y, camLook.z,
		camUp.x, camUp.y, camUp.z);

	//trackball.reset();
}


void Renderer::updateMousePosition(int x, int y) {
	mouseX = x;
	mouseY = y;
}

Point Renderer::getScreenPosition(int x, int y) {
	Point result;
	result.x = ((double)x/(double)width*2.0-1.0);
	result.y = -((double)y/(double)height*2.0-1.0);
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
	gluPerspective(50.0, ratio, 0.05*camPos.z, max(1000.0*camPos.z, 10.0));
	glMatrixMode(GL_MODELVIEW);
}

void Renderer::updateCamera() {
	gluLookAt(camPos.x, camPos.y, camPos.z,
				  camLook.x, camLook.y, camLook.z,
				  camUp.x, camUp.y, camUp.z);
}

void Renderer::drawQuad(Point tl, Point tr, Point bl, Point br) {
	glBegin(GL_QUADS);
	Vector3 normal = tl.getNormal(bl, tr);
	glNormal3d(normal.x, normal.y, normal.z);
	glTexCoord2f(1, 0);
	glVertex3d(tr.x, tr.y, tr.z); // Top Right Corner
	glTexCoord2f(1, 1);
	glVertex3d(tl.x, tl.y, tl.z); // Top Left Corner
	glTexCoord2f(0, 0);
	glVertex3d(bl.x, bl.y, bl.z); // Bottom Right Corner
	glTexCoord2f(0, 1);
	glVertex3d(br.x, br.y, br.z); // Bottom Left Corner
	glEnd();
}

void Renderer::drawCube(Point tlb, Point trb, Point tlf, Point trf, Point blb, Point brb, Point blf, Point brf) {
	drawQuad(tlb, trb, tlf, trf); // Top face
	drawQuad(brb, blb, brf, blf); // Bottom face
	
	drawQuad(tlf, blf, tlb, blb); // Left face
	drawQuad(trb, brb, trf, brf); // Right face

	drawQuad(tlf, trf, blf, brf); // Front face
	drawQuad(blb, brb, tlb, trb); // Back face
}