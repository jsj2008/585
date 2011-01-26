#include "Actor.h"
#include "../Physics/libCube.h"

Actor::Actor(PhysObject const & phys, Point pos, Point vel) : physObject(phys)
{
//	this->pos = pos;
	this->initialVel = vel;
	
}