#include "Actor.h"

Actor::Actor(PhysObject const & phys, RenderObject const & render, btVector3 const & pos, btVector3 const & vel) : physObject(phys), renderObject(render)
{
	this->pos = btVector3(pos.x, pos.y, pos.z);
	this->initialVel = vel;

	this->height = 03.3; // TESTING
	this->width = 03.3;
	this->depth = 03.3;
	
}

void Actor::setOrientation(btQuaternion const & orient)
{
	orientation = orient;
}

void Actor::setPosition(btVector3 const & pos)
{
	this->pos = pos;
}