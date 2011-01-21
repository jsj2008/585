#include "libCube.h"
#include <cstdlib>
libCube::libCube() : PhysObject(1.0) //1.0 kg
{
	fallInertia = new btVector3(0,0,0);
	shape = new btBoxShape(btVector3(0.15,0.15,0.15));
	//shape = new btSphereShape(0.15);
}