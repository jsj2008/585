#include "MainController.h"
#include <iostream>


MainController::MainController(){
		
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
	
	/*setup window*/
	window = new Window();
	
	/*setup subcomponents*/
	renderer = new Renderer(window);
	debugger = new GLDebugDrawer(window);
	physics = new Physics(actorList, debugger);

	
	window->run(this);	//launch window
	

}

void MainController::yield()
{
	
}

void MainController::tick(unsigned long interval)
{
	static int counter = 0;
	
	counter ++;
	
	if(counter > (1000/10) / 3.0)
	{
		counter = 0;
		explode();
	}
	
	
	physics->step( interval / 1000.0 );
	
}

void MainController::explode()
{
	static int counter = 0;
	ActorList temp;
	
	Real rad = 3.1415926 / 6 * counter++;	//pick random angle
	
	Actor * act = new Actor(Point(0,-3, 0), Point(cos(rad)*3, 5, sin(rad)*3 ) );
	act->physObject = cube;
	actorList->push_back(act);
	temp.push_back(act);
		
	physics->newActors(&temp);
}

/*void MainController::tick()
{	
	counter += 1;
	if(counter > (1000.0/30) / 3 )
	{
		counter = 0;
		explode();
	}
	physics->step(30/1000.0);
	renderer->updateGL();
	
}*/

MainController::~MainController()
{
	delete window;
	/*for(ActorList::iterator itr = actorList->begin(); itr != actorList->end(); ++itr)
	{
		delete (*itr);
	}
	delete actorList;
	
	delete cube;
	delete plane;
	delete physics;
	delete debugger;
	delete renderer;
	delete timer;*/
	
}