#include "PhysObject.h"
PhysObject::PhysObject(){}

PhysObject::~PhysObject()
{
	delete shape;
	delete fallInertia;	
}