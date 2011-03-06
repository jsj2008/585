#include "Obstacles.h"
#include "MainController.h"
#include "Common/Actor.h"
#include "Common/prelude.h"

Obstacles::Obstacles():
rockModel("map1.png", "", "models/obstacles/groundRock1.obj", 50),
large_rock(rockModel.model, 50),
logModel("map1.png", "", "models/obstacles/logBranch.obj", 30),
log(logModel.model, 30),
smallRockModel("map1.png", "", "models/obstacles/groundRock2.obj", 50),
smallRock(smallRockModel.model, 50)
{}

void Obstacles::initialize(ActorList & actors)
{
	for(int i=0; i<3; i++)	//big rocks
	{
		Actor * c = new Actor(large_rock, rockModel, btVector3(134 + 120 * i, 100, -1006));
		c->setOrientation(btQuaternion(0,0,0,1));
		actors.push_back(c);	
		MainController::addActor(c);
	}
	
		Actor * c = new Actor(log, logModel, btVector3(1000, 30, -1006));
		c->setOrientation(btQuaternion(0,0,0,1));
		actors.push_back(c);	
		MainController::addActor(c);

}