#include "MainController.h"
#include <iostream>


MainController::MainController(QWidget *parent) : QWidget(parent) {
	
	/*counters and such*/
	counter = 0;
	
	/*create all the static shapes for physics*/
	cube = new libCube();
	plane = new libPlane();
	
	/*setup various lists*/
	actorList = new ActorList();
	
	for(int i=0; i<0; i++)
	{
		Actor * act = new Actor(Point(0,-3, 0) );
		act->physObject = cube;
		actorList->push_back(act);
	}
	
	Actor * act = new Actor(Point(0,-5,0));
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

void MainController::keyPressEvent(QKeyEvent *event) {

	if(event->key() != 32)	//if not SPACE stop
		return;
	
}

void MainController::explode()
{
	static int counter = 0;
	ActorList temp;
	
	/*for(int i=0; i<1;i++)
	{
		Real rad = 3.1415926 / 6 * i;
		Actor * act = new Actor(Point(cos(rad)*1,-3, sin(rad)*1), Point(cos(rad)*2, fabs( sin(rad) * 6 ), sin(rad)*2) );
		act->physObject = cube;
		actorList->push_back(act);
		temp.push_back(act);
	}*/
	
	Real rad = 3.1415926 / 12 * counter++;	//pick random angle
	
	Actor * act = new Actor(Point(0,-3, 0), Point(cos(rad)*3, 5, sin(rad)*3 ) );
	act->physObject = cube;
	actorList->push_back(act);
	temp.push_back(act);
		
	physics->newActors(&temp);
}

void MainController::tick()
{	
	counter += 1;
	if(counter > (1000.0/30) / 3 )
	{
		counter = 0;
		explode();
	}
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