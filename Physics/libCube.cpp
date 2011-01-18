#include "libCube.h"
#include <cstdlib>
libCube::libCube()
{
	mass = 1.0;	//1kg
	fallInertia = new btVector3(0,0,0);
	int a = rand() % 2;
	shape = new btBoxShape(btVector3(0.15,0.15,0.15));
	//shape = new btSphereShape(0.15);
}