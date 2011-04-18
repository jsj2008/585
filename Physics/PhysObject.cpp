#include "PhysObject.h"

PhysObject::PhysObject(Real mass, bool isCourse) : mass(mass), isCourse(isCourse) {}

PhysObject::~PhysObject()
{
	delete shape;
	delete fallInertia;	
}