#include "libCube.h"

libCube::libCube()
{
	mass = 1.0;	//1kg
	fallInertia = new btVector3(0,0,0);
	shape = new btSphereShape(1);	//sphere for now	
}