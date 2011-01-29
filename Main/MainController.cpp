#include "MainController.h"
#include "Physics/PhysicsFactory.h"
#include <iostream>


MainController::MainController()
{
			
	/*setup various lists*/	
	for(int i=0; i<0; i++)
	{
		Actor * act = new Actor(mCube, new btVector3(0,-3, 0) );
		actorList.push_back(act);
	}
	
	Actor * act = new Actor(mPlane, new btVector3(0,-5,0));
	actorList.push_back(act);
		
	/*setup subcomponents*/
	renderer = new Renderer(window);
	physics = PhysicsFactory::newPhysics(actorList, debugger);
	window.run(this);	//launch window

}

void MainController::yield()
{
	
}

void MainController::tick(unsigned long interval)
{
	static int counter = 0;
	
	counter ++;
	
	if(counter > (1000/10) )
	{
		counter = 0;
		explode();
	}

	physics->step( interval / 1000.0 );
	renderer->step();
	
}

void MainController::explode()
{
	static int counter = 0;
	ActorList temp;
	
	Real rad = 3.1415926 / 6 * counter++;	//pick random angle
	
	Actor * act = new Actor(mCube, new btVector3(0,-3, 0), new btVector3(cos(rad)*3, 5, sin(rad)*3 ) );
	actorList.push_back(act);
	temp.push_back(act);
		
	physics->newActors(temp);
}

MainController::~MainController()
{
	for(ActorList::iterator itr = actorList.begin(); itr != actorList.end(); ++itr)
	{
		delete (*itr);
	}
	
	delete physics;
	delete renderer;
	
}