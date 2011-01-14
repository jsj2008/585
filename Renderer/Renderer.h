#pragma once

#include <QGLWidget>
#include "Point.h"
#include "Trackball.h"
#include <stdlib.h>
#include <math.h>
using namespace std;

class Renderer;

class Renderer : public QGLWidget {
	Q_OBJECT
public:
	Renderer(const ActorList* actorList = 0, QWidget* parent = 0);
	~Renderer();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	void reset();
	void resetView();

signals:

public slots:

protected:
	void paintGL();
	void initializeGL();
	void resizeGL(int w, int h);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

private:
	void drawAxes();
	void updateCamera();
	void setProjection();
	void drawQuad(Point tl, Point tr, Point bl, Point br);
	void drawCube(Point tlb, Point trb, Point tlf, Point trf, Point blb, Point brb, Point blf, Point brf);
	void updateMousePosition(int x, int y);
	Point getScreenPosition(int x, int y);
	
	int mouseX;
	int mouseY;

	Point camPos;				// Position of the camera
	Point camLook;				// Point that the camera is looking at
	Vector3 camUp;				// Up vector for the camera

	double ratio;
	int width;
	int height;	

	Trackball trackball;		// The trackball for rotating the surface
};
