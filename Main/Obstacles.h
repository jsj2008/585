#ifndef OBSTACLES_H
#define OBSTACLES_H

#include "Physics/libModel.h"
#include "Renderer/RenderObject.h"
#include "Physics/Heightmap.h"
#include "Common/SettingsFactory.h"

class Obstacles
{
public:
		Obstacles();		
		void initialize(ActorList &);
private:
	btVector3 convertToWorldPos(btVector3 const & mapPosition);
	void addObstacle(PhysObject const & phys, RenderObject const & render, btVector3 const & mapPosition, ActorList & actors, btQuaternion const & orientation = btQuaternion(0,0,0,1));

	/*render objects*/
	const RenderObject rockModel;
	const RenderObject rockModel2;
	const RenderObject rockModel3;
	const RenderObject logModel;
	const RenderObject smallRockModel;
	const RenderObject smallRockModel2;

	const RenderObject leafyMod;

	/*objShapes*/
	const libModel largeRock;
	const libModel largeRock2;
	const libModel largeRock3;
	const libModel log;
	const libModel smallRock;
	const libModel smallRock2;

	const libModel leafy;
	
	HeightMap* hm;
	float xscale;
	float yscale;
	float zscale;
};

#endif