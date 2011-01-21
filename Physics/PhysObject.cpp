#include "PhysObject.h"

PhysObject::PhysObject(Real mass) : mass(mass) {}

PhysObject::~PhysObject()
{
	delete shape;
	delete fallInertia;	
}