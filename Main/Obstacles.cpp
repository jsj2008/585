#include "Obstacles.h"
#include "MainController.h"
#include "Common/Actor.h"
#include "Common/prelude.h"
#include "Physics/HeightMapManager.h"

Obstacles::Obstacles():
rockModel("data/textures/map1.png", "data/textures/blank.bmp", "models/obstacles/groundRock1.obj", 40),
largeRock(rockModel.model, 40, true),
rockModel2("data/textures/map1.png", "data/textures/blank.bmp", "models/obstacles/groundRock2.obj", 40),
largeRock2(rockModel2.model, 40, true),
rockModel3("data/textures/map1.png", "data/textures/blank.bmp", "models/obstacles/groundRock3.obj", 40),
largeRock3(rockModel3.model, 40, true),
logModel("data/textures/mapOld.png", "data/textures/blank.bmp", "models/obstacles/logBranch.obj", 30),
log(logModel.model, 30, false),
logSmallModel("data/textures/mapOld.png", "data/textures/blank.bmp", "models/obstacles/logBranch.obj", 8),
logSmall(logModel.model, 8, false, 10.0),
smallRockModel("data/textures/map1.png", "data/textures/blank.bmp", "models/obstacles/fallingRock1.obj", 50),
smallRock(smallRockModel.model, 50, false),
smallerRockModel("data/textures/map1.png", "data/textures/blank.bmp", "models/obstacles/fallingRock1.obj", 5),
smallFallingRock(smallerRockModel.model, 7, false, 7.0),

// leafyMod("data/textures/tree.png", "data/textures/blank.bmp", "models/tree.obj", 20),
// leafy(leafyMod.model, 20),
smallRockModel2("data/textures/map1.png", "data/textures/blank.bmp", "models/obstacles/fallingRock2.obj", 50),
smallRock2(smallRockModel2.model, 50, false),

tunnelTreeModel("data/textures/tree.png", "data/textures/blank.bmp", "models/course/tree_tris.obj", 255),
tunnelTree(tunnelTreeModel.model, 255, true, 0, true),

bridgeModel("data/textures/bridge_uv_small.png", "data/textures/blank.bmp", "models/course/bridge_extension.obj", 12),
bridge(bridgeModel.model, 12, false, 0, true),

mansionModel("data/textures/BuildingsMedieval0004_2_L.png", "data/textures/blank.bmp", "models/course/mansion.obj", 10),
mansion(mansionModel.model, 10, false, 0, true),

finishLineModel("data/textures/finish.png", "data/textures/blank.bmp", "models/course/finish_line.obj", 10),
finishLine(finishLineModel.model, 0, false, 0, true),

alertSignModel("data/textures/alert.png", "data/textures/blank.bmp", "models/obstacles/alert_sign.obj", -10),
alertSign(alertSignModel.model, -10, false, 0, true),
leftSignModel("data/textures/left_arrow.png", "data/textures/blank.bmp", "models/obstacles/arrow_left.obj", -20),
leftSign(leftSignModel.model, -20, false, 0, true),
rightSignModel("data/textures/right_arrow.png", "data/textures/blank.bmp", "models/obstacles/arrow_right.obj", -20),
rightSign(rightSignModel.model, -20, false, 0, true),

blPlantModel("data/textures/mapOld.png", "data/textures/blank.bmp", "models/obstacles/broad_plant_large.obj", 6),
blPlant(blPlantModel.model, 0, false, 0, true),
bmPlantModel("data/textures/mapOld.png", "data/textures/blank.bmp", "models/obstacles/broad_plant_medium.obj", 4),
bmPlant(bmPlantModel.model, 0, false, 0, true),
bsPlantModel("data/textures/mapOld.png", "data/textures/blank.bmp", "models/obstacles/broad_plant_small.obj", 3),
bsPlant(bsPlantModel.model, 0, false, 0, true),
nlPlantModel("data/textures/mapOld.png", "data/textures/blank.bmp", "models/obstacles/narrow_plant_large.obj", 6),
nlPlant(nlPlantModel.model, 0, false, 0, true),
nmPlantModel("data/textures/mapOld.png", "data/textures/blank.bmp", "models/obstacles/narrow_plant_medium.obj", 4),
nmPlant(nmPlantModel.model, 0, false, 0, true),
nsPlantModel("data/textures/mapOld.png", "data/textures/blank.bmp", "models/obstacles/narrow_plant_small.obj", 3),
nsPlant(nsPlantModel.model, 0, false, 0, true)

{
	hm = HeightMapManager::GetHeightMap();
	
	xscale = LoadFloat("config/world.xml","height_map_scale_x");		// Load the scaling information
	yscale = LoadFloat("config/world.xml","height_map_scale_y");
	zscale = LoadFloat("config/world.xml","height_map_scale_z");
}

void Obstacles::initialize(ActorList & actors)
{
    //concave tree
    addObstacle(smallRock2, smallRockModel2, btVector3(138,12,57), actors);
	
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

	addObstacle(tunnelTree, tunnelTreeModel, btVector3(23,-130,155), actors, btQuaternion(btVector3(0,1,0), 1.57) );
	addObstacle(largeRock, rockModel, btVector3(158,-5,0), actors);

	addObstacle(bridge, bridgeModel, btVector3(84, 21, 202), actors, btQuaternion(btVector3(0,1,0), 0.785398175));
	addObstacle(bridge, bridgeModel, btVector3(80, 1, 206), actors, btQuaternion(btVector3(0,1,0), 0.785398175));
	
	addObstacle(mansion, mansionModel, btVector3(104, -10, 255), actors, btQuaternion(btVector3(0,1,0), -1.57) );
	addObstacle(finishLine, finishLineModel, btVector3(108, 10, 247), actors);

	
	addObstacle(alertSign, alertSignModel, btVector3(70, 30, 206), actors, btQuaternion(btVector3(0,1,0), -0.785398175) ); // Bridge sign
	addObstacle(alertSign, alertSignModel, btVector3(70, 30, 206), actors, btQuaternion(btVector3(0,1,0), 0.785398175) );

	addObstacle(rightSign, rightSignModel, btVector3(144, 40, 52), actors, btQuaternion(btVector3(0,1,0), -1.57) ); // Rock sign
	addObstacle(rightSign, rightSignModel, btVector3(144, 40, 52), actors, btQuaternion(btVector3(0,1,0), 1.57) );
	
	addObstacle(leftSign, leftSignModel, btVector3(229, 40, 40), actors, btQuaternion(btVector3(0,1,0), 0) );
	addObstacle(leftSign, leftSignModel, btVector3(229, 40, 40), actors, btQuaternion(btVector3(0,1,0), 3.14) );

	addObstacle(leftSign, leftSignModel, btVector3(234, 40, 115), actors, btQuaternion(btVector3(0,1,0), -1.57) );
	addObstacle(leftSign, leftSignModel, btVector3(234, 40, 115), actors, btQuaternion(btVector3(0,1,0), 1.57) );

	addObstacle(leftSign, leftSignModel, btVector3(97, 40, 83), actors, btQuaternion(btVector3(0,1,0), 0) );
	addObstacle(leftSign, leftSignModel, btVector3(97, 40, 83), actors, btQuaternion(btVector3(0,1,0), 3.14) );
	addObstacle(rightSign, rightSignModel, btVector3(97, 40, 85), actors, btQuaternion(btVector3(0,1,0), 0) );
	addObstacle(rightSign, rightSignModel, btVector3(97, 40, 85), actors, btQuaternion(btVector3(0,1,0), 3.14) );

	addObstacle(leftSign, leftSignModel, btVector3(54, 40, 76), actors, btQuaternion(btVector3(0,1,0), 0) );
	addObstacle(leftSign, leftSignModel, btVector3(54, 40, 76), actors, btQuaternion(btVector3(0,1,0), 3.14) );
	
	addObstacle(rightSign, rightSignModel, btVector3(51, 40, 45), actors, btQuaternion(btVector3(0,1,0), -1.57) );
	addObstacle(rightSign, rightSignModel, btVector3(51, 40, 45), actors, btQuaternion(btVector3(0,1,0), 1.57) );

	addObstacle(rightSign, rightSignModel, btVector3(44, 40, 238), actors, btQuaternion(btVector3(0,1,0), -1.57) );
	addObstacle(rightSign, rightSignModel, btVector3(44, 40, 238), actors, btQuaternion(btVector3(0,1,0), 1.57) );
		
	addObstacle(leftSign, leftSignModel, btVector3(102, 40, 188), actors, btQuaternion(btVector3(0,1,0), -0.785398175) );
	addObstacle(leftSign, leftSignModel, btVector3(102, 40, 188), actors, btQuaternion(btVector3(0,1,0), 0.785398175) );

	ifstream in;
	in.open("plants.pth");

	double x, z;
	int plantNum;
	while (in >> x >> z) {
	plantNum = rand() % 6;
		if (plantNum == 0) addObstacle(blPlant, blPlantModel, btVector3(x/2, -1.5, z/2), actors, btQuaternion(btVector3(0,1,0), rand()));
		if (plantNum == 1) addObstacle(bmPlant, bmPlantModel, btVector3(x/2, -1, z/2), actors, btQuaternion(btVector3(0,1,0), rand()));
		if (plantNum == 2) addObstacle(bsPlant, bsPlantModel, btVector3(x/2, -0.5, z/2), actors, btQuaternion(btVector3(0,1,0), rand()));
		if (plantNum == 3) addObstacle(nlPlant, nlPlantModel, btVector3(x/2, -1.5, z/2), actors, btQuaternion(btVector3(0,1,0), rand()));
		if (plantNum == 4) addObstacle(nmPlant, nmPlantModel, btVector3(x/2, -1, z/2), actors, btQuaternion(btVector3(0,1,0), rand()));
		if (plantNum == 5) addObstacle(nsPlant, nsPlantModel, btVector3(x/2, -0.5, z/2), actors, btQuaternion(btVector3(0,1,0), rand()));
	}

	ifstream in2;
	in2.open("rocks.pth");

	while (in2 >> x >> z) {
		for (int i = 0; i < 3; i++)
    		addObstacle(smallFallingRock, smallerRockModel, btVector3(x/2,i*3,z/2), actors);
    }

	in.close();
	in2.close();

	addObstacle(logSmall, logSmallModel, btVector3(140,10,75), actors, btQuaternion(btVector3(0,1,0), 1.7));
	addObstacle(logSmall, logSmallModel, btVector3(22,10,128), actors);
	addObstacle(logSmall, logSmallModel, btVector3(208,20,116), actors);

	//addObstacle(leafy, leafyMod, btVector3(158,30,22), actors, btQuaternion(btVector3(1,0,0), 1));
	// addObstacle(leafy, leafyMod, btVector3(158,30,22), actors);
}

void Obstacles::addObstacle(PhysObject const & phys, RenderObject const & render, btVector3 const & mapPosition, ActorList & actors, btQuaternion const & orientation) {
	Actor * c = new Actor(phys, &render, convertToWorldPos(mapPosition));
	actors.push_back(c);
	c->setOrientation(orientation);
	MainController::addActor(c);
}

btVector3 Obstacles::convertToWorldPos(btVector3 const & mapPosition) {
	return btVector3((mapPosition.getX() * xscale) - ((float)(hm->width*xscale))/2.0,
					((float)(hm->map[((int)mapPosition.getZ()*hm->width)+(int)mapPosition.getX()]) * yscale) + mapPosition.getY(),
					(mapPosition.getZ() * zscale) -((float)(hm->height*zscale))/2.0);
}
