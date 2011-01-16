#include "libCube.h"

libCube::libCube()
{
	mass = 1.0;	//1kg
	fallInertia = new btVector3(0,0,0);
	shape = new btBoxShape(btVector3(0.15,0.15,0.15));
}