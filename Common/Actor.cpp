#include "Actor.h"

Actor::Actor(PhysObject const & phys, RenderObject const & render, btVector3 const & pos, btVector3 const & vel) : physObject(phys), renderObject(render)
{
	this->pos = pos;
	this->initialVel = vel;
	orientation = btQuaternion(0,0,0,1);
}

Actor::Actor(RenderObject const & render, btVector3 const & pos, btVector3 const & vel) : physObject(temp), renderObject(render)
{
	this->pos = pos;
	this->initialVel = vel;
	orientation = btQuaternion(0,0,0,1);
	
}

void Actor::setOrientation(btQuaternion const & orient)
{
	orientation = orient;
}

void Actor::setPosition(btVector3 const & pos)
{
	this->pos = pos;
}