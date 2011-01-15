#include "Actor.h"
#include "../Physics/libCube.h"

Actor::Actor()
{
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
}

Actor::Actor(Real x, Real y, Real z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
}