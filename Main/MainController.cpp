#include "MainController.h"
#include "Physics/PhysicsFactory.h"
#include <iostream>
#include "Common/SettingsFactory.h"

MainController * MainController::ptr = NULL;


MainController::MainController() : 
physics(PhysicsFactory::newPhysics(actorList, debugger) ),
jeepModel("blank.bmp", "jeep2_flipx.obj"),
cubeModel("RAADicle.bmp", "cube.obj"),
planeModel("RAADicleXtreme.bmp", "quad.obj")

{
	if(ptr == NULL)
		ptr = this;

	float const & planeY = LoadFloat("config/start.xml", "planeY");
	float const & jeepX = LoadFloat("config/start.xml", "jeepX");
	float const & jeepY = LoadFloat("config/start.xml", "jeepY");
	float const & jeepZ = LoadFloat("config/start.xml", "jeepZ");

		//renderTest = RenderObject("testBox.bmp", "ducky.obj");

	ActorList temp;
	 // Actor * act = new Actor(mPlane, planeModel, btVector3(0,-5,0));
	 // actorList.push_back(act);	

	
	

	 // temp.push_back(act);
		
	/*pass jeep into physics/renderer but don't add to dynamicWorld (this is done by jeep internally)*/
	jeep = new JeepActor(mChasis, jeepModel, physics, window.aInput , btVector3(jeepX, jeepY, jeepZ));
	actorList.push_back(jeep);
		
	/*setup subcomponents*/
	physics->newActors(temp);
	renderer = new Renderer(window, actorList);
	window.run(this);	//launch window

}

void MainController::yield()
{
	
}

void MainController::tick(unsigned long interval)
{
	//std::cout << interval << std::endl;
	renderer->step();
	physics->step( interval / 1000.0);
	jeep->tick(interval / 1000.0);
	
	/*giant hack for camera*/

	static btVector3 pos = btVector3(9,11,15);
	btVector3 look = jeep->pos;
	btVector3 behind = quatRotate(jeep->orientation, btVector3(-1,0.4,0) );
	pos += (look + 40*behind - pos ) / 30.0;
	
	renderer->setCamera(pos,look);
	
}

void MainController::explode()
{
	static int counter = 0;
	ActorList temp;
	Real rad = 3.1415926 / 6 * counter++;	//pick random angle
	
	Actor * act = new Actor(mCube, cubeModel, btVector3(0,-3, 0), btVector3(cos(rad)*5, 10, sin(rad)*5 ) );
	actorList.push_back(act);
	temp.push_back(act);
		
	physics->newActors(temp);
}

void MainController::addActor(Actor * actor)
{
	ptr->actorList.push_back(actor);
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