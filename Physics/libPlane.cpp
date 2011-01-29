#include "libPlane.h"

libPlane::libPlane() : PhysObject(0.0) //static
{
	fallInertia = new btVector3(0,0,0);
	shape = new btStaticPlaneShape(btVector3(0.0,1,0),1);
}