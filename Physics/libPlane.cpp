#include "libPlane.h"

libPlane::libPlane()
{
	mass = 0.0;	//static
	fallInertia = new btVector3(0,0,0);
	shape = new btStaticPlaneShape(btVector3(0,1,0),1);
}