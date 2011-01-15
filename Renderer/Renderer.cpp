#include "Renderer.h"
#include <iostream>


Renderer::Renderer(QWidget *parent) : QGLWidget(parent) {
	ratio = 1.0;
	width = 800;
	height = 600;

	camPos = Point(0,0,3);
	camLook = Point(0,0,0);
	camUp = Vector3(0,1,0);
	
	trackball = Trackball(75);
}

Renderer::~Renderer() { }

QSize Renderer::minimumSizeHint() const {
	return QSize(100, 100);
}

QSize Renderer::sizeHint() const {
	return QSize(800, 600);
}

void Renderer::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL the functable

	glLoadIdentity();

	updateCamera();
	glMultMatrixf(trackball.getMatrix()); // Rotate the 3D fractal by the trackball's rotation matrix
	renderObjects();
	
	glCallList(listIndex);
	
	glFlush();
}

void Renderer::renderObjects() {
	/*glBegin(GL_QUADS);
	drawCube(Point( 1, 1, -1), Point( 1, -1, -1), Point( 1, 1, 1), Point( 1, -1, 1),
			 Point(-1, 1, -1), Point(-1, -1, -1), Point(-1, 1, 1), Point(-1, -1, 1));
	glEnd();*/
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

	trackball.reset();
}

void Renderer::mousePressEvent(QMouseEvent *event) {
	updateMousePosition(event->x(), event->y());
	if (event->buttons() == Qt::LeftButton) {
		trackball.press(getScreenPosition(mouseX, mouseY)); // Press the trackball at this point
		updateGL();
	}
}

void Renderer::mouseReleaseEvent(QMouseEvent *event) {
	updateCamera();
	trackball.release();
	updateGL();
}

void Renderer::mouseMoveEvent(QMouseEvent *event) { 
	if (event->buttons() & Qt::RightButton) { // Pan the camera on right mouse button drag
		int oldMouseX = mouseX; // Remember where the mouse was
		int oldMouseY = mouseY;
		updateMousePosition(event->x(), event->y());
		int diffX = oldMouseX - mouseX; // Find the difference in the mouse position
		int diffY = mouseY - oldMouseY;
		camPos = camPos + (Point(diffX, diffY)*camPos.z)/(double)min(width, height); // Move the camera position and look point to pan it
		camLook = camLook + (Point(diffX, diffY)*camPos.z)/(double)min(width, height);
		updateCamera();
		updateGL();
	}
	else if (event->buttons() & Qt::LeftButton) {
		updateMousePosition(event->x(), event->y());
		trackball.move(getScreenPosition(mouseX, mouseY)); // Rotate the trackball on left mouse button drag
		updateCamera();
		updateGL();
	}
}

void Renderer::wheelEvent(QWheelEvent *event) {
	if (event->orientation() == Qt::Vertical) { // Zoom in and out with the mouse wheel
		int numDegrees = event->delta() / 8;
		int numSteps = numDegrees / 15;

		camPos.z -= ((camPos.z)/20.0)*numSteps;
		setProjection();

		updateGL();
	}
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