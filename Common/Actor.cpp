#include "Actor.h"
#include "../Physics/libCube.h"

Actor::Actor(PhysObject const & phys, btVector3* pos, btVector3* vel) : physObject(phys)
{
	this->pos = pos;
	this->initialVel = vel;
	
}