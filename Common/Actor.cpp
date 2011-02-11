#include "Actor.h"

Actor::Actor(PhysObject const & phys, Point pos, Point vel) : physObject(phys)
{
	this->pos = btVector3(pos.x, pos.y, pos.z);
	this->initialVel = vel;
	
}

void Actor::setOrientation(btQuaternion const & orient)
{
	orientation = orient;
}

void Actor::setPosition(btVector3 const & pos)
{
	this->pos = pos;
}