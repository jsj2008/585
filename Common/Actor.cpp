#include "Actor.h"
#include "../Physics/libCube.h"

Actor::Actor(PhysObject const & phys, btVector3 const & pos, btVector3 const & vel) : physObject(phys)
{
	this->pos = pos;
	this->initialVel = vel;

	this->height = 0.3; // TESTING
	this->width = 0.3;
	this->depth = 0.3;
	
}