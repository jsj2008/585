#include "Actor.h"
#include "../Physics/libCube.h"

Actor::Actor(PhysObject const & phys, Point pos, Point vel) : physObject(phys)
{
	this->pos = btVector3(pos.x, pos.y, pos.z);
	this->initialVel = vel;
	
}