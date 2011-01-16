#pragma once

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <Renderer/Renderer.h>
#include <Renderer/GLDebugDrawer.h>
#include <Physics/Physics.h>
#include <Common/prelude.h>
#include <Common/Actor.h>
#include <Common/Point.h>


class MainController : public QWidget {
	Q_OBJECT
public:
	
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	
	MainController(QWidget* parent = 0);
	virtual ~MainController();


protected:
	virtual void keyPressEvent(QKeyEvent * event);
	void explode();
		
private:
	QTimer * timer;
	Renderer * renderer;
	Physics * physics;
	GLDebugDrawer * debugger;
	ActorList * actorList;
	int counter;
	
	/*objShapes*/
	PhysObject * cube;
	PhysObject * plane;

signals:

public slots:

	void tick();

};
