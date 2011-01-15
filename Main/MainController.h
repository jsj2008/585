#pragma once

#include <QWidget>
#include <QTimer>
#include "../Renderer/Renderer.h"
#include "../Physics/Physics.h"



class MainController : public QWidget {
	Q_OBJECT
public:
	
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	
	MainController(Renderer * renderer, Physics * physics, QWidget* parent = 0);
	virtual ~MainController();
	
private:
	QTimer * timer;
	Renderer * renderer;
	Physics * physics;

signals:

public slots:

	void tick();

};
