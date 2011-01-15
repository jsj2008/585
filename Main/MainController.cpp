#include "MainController.h"
#include <iostream>


MainController::MainController(Renderer * renderer, Physics * physics, QWidget *parent) : QWidget(parent) {
	
	this->renderer = renderer;
	this->physics = physics;

	/*setup timer*/
	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(tick() ) );
	timer->start(30);

}

void MainController::tick()
{	
	physics->step(30/1000.0);
	renderer->updateGL();
	
}

MainController::~MainController()
{
	
	delete timer;
	
}

QSize MainController::minimumSizeHint() const {
	return QSize(100, 100);
}

QSize MainController::sizeHint() const {
	return QSize(800, 600);
}