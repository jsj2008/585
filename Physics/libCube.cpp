#include "libCube.h"
#include <cstdlib>
libCube::libCube() : PhysObject(5.0) //5.0 kg
{
	fallInertia = new btVector3(0,0,0);
	shape = new btBoxShape(btVector3(1.65,1.65,1.65));
	//shape = new btSphereShape(0.15);
}