#include "Obstacles.h"
#include "MainController.h"
#include "Common/Actor.h"
#include "Common/prelude.h"
#include "Physics/HeightMapManager.h"

Obstacles::Obstacles():
rockModel("map1.png", "", "models/obstacles/groundRock1.obj", 40),
largeRock(rockModel.model, 40),
rockModel2("map1.png", "", "models/obstacles/groundRock2.obj", 40),
largeRock2(rockModel2.model, 40),
rockModel3("map1.png", "", "models/obstacles/groundRock3.obj", 40),
largeRock3(rockModel3.model, 40),
logModel("map1.png", "", "models/obstacles/logBranch.obj", 30),
log(logModel.model, 30),
smallRockModel("map1.png", "", "models/obstacles/fallingRock1.obj", 50),
smallRock(smallRockModel.model, 50),
smallRockModel2("map1.png", "", "models/obstacles/fallingRock2.obj", 50),
smallRock2(smallRockModel2.model, 50),

leafyMod("map1.png", "", "models/obstacles/leafRound.obj", 10),
leafy(leafyMod.model, 10)
{
	hm = HeightMapManager::GetHeightMap();
	
	xscale = LoadFloat("config/world.xml","height_map_scale_x");		// Load the scaling information
	yscale = LoadFloat("config/world.xml","height_map_scale_y");
	zscale = LoadFloat("config/world.xml","height_map_scale_z");
}

void Obstacles::initialize(ActorList & actors)
{
	addObstacle(leafy, leafyMod, btVector3(158,30,22), actors, btQuaternion(btVector3(1,0,0), 1));

	addObstacle(smallRock2, smallRockModel2, btVector3(138,12,57), actors);
	addObstacle(smallRock, smallRockModel, btVector3(141,12,57), actors);
	addObstacle(smallRock, smallRockModel, btVector3(143,15,57), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(147,15,57), actors);
	addObstacle(smallRock, smallRockModel, btVector3(152,21,56), actors);
	addObstacle(smallRock, smallRockModel, btVector3(155,17,56), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(158,22,55), actors);
	addObstacle(smallRock, smallRockModel, btVector3(162,22,55), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(165,29,54), actors);
	addObstacle(smallRock, smallRockModel, btVector3(168,22,54), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(173,22,54), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(177,26,53), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(180,20,53), actors);
	addObstacle(smallRock, smallRockModel, btVector3(183,26,55), actors);
	addObstacle(smallRock, smallRockModel, btVector3(186,37,54), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(189,37,54), actors);

	addObstacle(smallRock2, smallRockModel2, btVector3(171,-2,1), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(171,-2,6), actors);
	addObstacle(smallRock, smallRockModel, btVector3(171,-2,11), actors);
	addObstacle(smallRock, smallRockModel, btVector3(171,-2,16), actors);
	addObstacle(smallRock, smallRockModel, btVector3(171,-2,21), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(171,-2,26), actors);

	addObstacle(smallRock, smallRockModel, btVector3(163,-2,37), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(171,-2,31), actors);//, btQuaternion(btVector3(0,1,0), 3));
	addObstacle(smallRock2, smallRockModel2, btVector3(168,-1,34), actors);
	addObstacle(smallRock2, smallRockModel2, btVector3(166,2,35), actors);//, btQuaternion(btVector3(0,1,0), 2));
	addObstacle(smallRock, smallRockModel, btVector3(158,0,40), actors);//, btQuaternion(btVector3(0,1,0), 1));

	addObstacle(largeRock2, rockModel2, btVector3(231,-35,81), actors);
	addObstacle(largeRock, rockModel, btVector3(230,-38,87), actors);//, btQuaternion(btVector3(0,1,0), 2));
	addObstacle(largeRock, rockModel, btVector3(225,-36,94), actors);
	addObstacle(largeRock2, rockModel2, btVector3(223,-37,100), actors);//, btQuaternion(btVector3(0,1,0), 1));
	addObstacle(largeRock, rockModel, btVector3(226,-42,106), actors);//, btQuaternion(btVector3(0,1,0), 1));

	addObstacle(smallRock, smallRockModel, btVector3(199,10,50), actors);
	addObstacle(log, logModel, btVector3(198,45,57), actors);
	
	addObstacle(largeRock2, rockModel2, btVector3(240,-50,58), actors);
	
	addObstacle(log, logModel, btVector3(67,45,81), actors);
	
	addObstacle(smallRock2, smallRockModel2, btVector3(91,2,82), actors);
	addObstacle(smallRock, smallRockModel, btVector3(44,2,38), actors);
}

void Obstacles::addObstacle(PhysObject const & phys, RenderObject const & render, btVector3 const & mapPosition, ActorList & actors, btQuaternion const & orientation) {
	Actor * c = new Actor(phys, render, convertToWorldPos(mapPosition));
	actors.push_back(c);
	c->orientation = orientation;
	MainController::addActor(c);
}

btVector3 Obstacles::convertToWorldPos(btVector3 const & mapPosition) {
	return btVector3((mapPosition.getX() * xscale) - ((float)(hm->width*xscale))/2.0,
					((float)(hm->map[((int)mapPosition.getZ()*hm->width)+(int)mapPosition.getX()]) * yscale) + mapPosition.getY(),
					(mapPosition.getZ() * zscale) -((float)(hm->height*zscale))/2.0);
}
