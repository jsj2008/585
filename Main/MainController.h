#pragma once

#include <QWidget>
#include <QTimer>
#include "../Renderer/Renderer.h"
#include "../Renderer/GLDebugDrawer.h"
#include "../Physics/Physics.h"
#include "../Common/prelude.h"
#include "../Common/Actor.h"


class MainController : public QWidget {
	Q_OBJECT
public:
	
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	
	MainController(QWidget* parent = 0);
	virtual ~MainController();
	
private:
	QTimer * timer;
	Renderer * renderer;
	Physics * physics;
	GLDebugDrawer * debugger;
	ActorList * actorList;
	
	/*objShapes*/
	PhysObject * cube;
	PhysObject * plane;

signals:

public slots:

	void tick();

};
