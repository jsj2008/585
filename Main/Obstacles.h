#ifndef OBSTACLES_H
#define OBSTACLES_H

#include "Physics/libModel.h"
#include "Renderer/RenderObject.h"

class Obstacles
{
public:
		Obstacles();		
		void initialize(ActorList &);
private:
	/*render objects*/
	const RenderObject rockModel;
	const RenderObject logModel;
	const RenderObject smallRockModel;

	/*objShapes*/
	const libModel large_rock;
	const libModel log;
	const libModel smallRock;
	
};

#endif