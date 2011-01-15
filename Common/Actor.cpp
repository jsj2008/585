#include "Actor.h"
#include "../Physics/libCube.h"

Actor::Actor()
{
	physObject = new libCube();
	
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
}