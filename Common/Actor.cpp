#include "Actor.h"
#include "../Physics/libCube.h"

Actor::Actor(Point pos, Point vel)
{
	this->pos = pos;
	this->initialVel = vel;
	
}