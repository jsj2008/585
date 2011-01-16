#include "MainController.h"
#include <iostream>


MainController::MainController(QWidget *parent) : QWidget(parent) {
	
	/*create all the static shapes for physics*/
	cube = new libCube();
	plane = new libPlane();
	
	/*setup various lists*/
	actorList = new ActorList();
	
	for(int i=0; i<10; i++)
	{
		Actor * act = new Actor( 1.0 / (rand() % 10 + 1), 2*i + 1, 0 );
		act->physObject = cube;
		actorList->push_back(act);
	}
	
	Actor * act = new Actor(0, -5, 0);
	act->physObject = plane;
	actorList->push_back(act);
	
	/*setup subcomponents*/
	renderer = new Renderer(this);
	debugger = new GLDebugDrawer(renderer);
	physics = new Physics(actorList, debugger);

	
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
	for(ActorList::iterator itr = actorList->begin(); itr != actorList->end(); ++itr)
	{
		delete (*itr);
	}
	delete actorList;
	
	delete cube;
	delete plane;
	delete physics;
	delete debugger;
	delete renderer;
	delete timer;
	
}

QSize MainController::minimumSizeHint() const {
	return QSize(100, 100);
}

QSize MainController::sizeHint() const {
	return QSize(800, 600);
}