#include "Actor.h"
#include "../Physics/libCube.h"

Actor::Actor(PhysObject const & phys, RenderObject const & render, btVector3 const & pos, btVector3 const & vel) : physObject(phys), renderObject(render)
{
	this->pos = pos;
	this->initialVel = vel;

	this->height = 03.3; // TESTING
	this->width = 03.3;
	this->depth = 03.3;
	
}