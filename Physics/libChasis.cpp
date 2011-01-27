#include "libChasis.h"
libChasis::libChasis() : PhysObject(10.0) //10.0 kg
{
	fallInertia = new btVector3(0,0,0);
	shape = new btBoxShape(btVector3(1.5,0.15,0.5));
}