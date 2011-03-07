#include "Obstacles.h"
#include "MainController.h"
#include "Common/Actor.h"
#include "Common/prelude.h"
#include "Physics/HeightMapManager.h"

Obstacles::Obstacles():
rockModel("map1.png", "", "models/obstacles/groundRock1.obj", 50),
largeRock(rockModel.model, 50),
rockModel2("map1.png", "", "models/obstacles/groundRock2.obj", 50),
largeRock2(rockModel2.model, 50),
rockModel3("map1.png", "", "models/obstacles/groundRock3.obj", 50),
largeRock3(rockModel3.model, 50),
logModel("map1.png", "", "models/obstacles/logBranch.obj", 30),
log(logModel.model, 30),
smallRockModel("map1.png", "", "models/obstacles/fallingRock1.obj", 50),
smallRock(smallRockModel.model, 50),
smallRockModel2("map1.png", "", "models/obstacles/fallingRock2.obj", 50),
smallRock2(smallRockModel2.model, 50)
{
	hm = HeightMapManager::GetHeightMap();
	
	xscale = LoadFloat("config/world.xml","height_map_scale_x");		// Load the scaling information
	yscale = LoadFloat("config/world.xml","height_map_scale_y");
	zscale = LoadFloat("config/world.xml","height_map_scale_z");
}

void Obstacles::initialize(ActorList & actors)
{
	addObstacle(smallRock, smallRockModel, btVector3(141,-5,58), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(147,-4,59), actors);
	addObstacle(smallRock, smallRockModel, btVector3(155,-4,61), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(165,-4,57), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(177,-4,53), actors);
	addObstacle(smallRock, smallRockModel, btVector3(186,-3,54), actors);

	addObstacle(smallRock, smallRockModel, btVector3(163,-2,37), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(171,-2,31), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(168,-1,34), actors);

	addObstacle(smallRock, smallRockModel, btVector3(199,10,50), actors);
	addObstacle(log, logModel, btVector3(198,45,57), actors);
	
	addObstacle(largeRock2, rockModel2, btVector3(240,-50,58), actors);
	
	addObstacle(log, logModel, btVector3(67,45,81), actors);
}

void Obstacles::addObstacle(PhysObject const & phys, RenderObject const & render, btVector3 const & mapPosition, ActorList & actors) {
	Actor * c = new Actor(phys, render, convertToWorldPos(mapPosition));
	actors.push_back(c);	
	MainController::addActor(c);
}

btVector3 Obstacles::convertToWorldPos(btVector3 const & mapPosition) {
	return btVector3((mapPosition.getX() * xscale) - ((float)(hm->width*xscale))/2.0,
					((float)(hm->map[((int)mapPosition.getZ()*hm->width)+(int)mapPosition.getX()]) * yscale) + mapPosition.getY(),
					(mapPosition.getZ() * zscale) -((float)(hm->height*zscale))/2.0);
}
