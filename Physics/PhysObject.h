#ifndef PHYSOBJECT_H
#define PHYSOBJECT_H

#include "Common/prelude.h"
#include <btBulletDynamicsCommon.h>

class PhysObject
{
public:	
	PhysObject(Real mass);
	virtual ~PhysObject();
	
	btCollisionShape * shape;
	const Real mass;
	btVector3 * fallInertia;
	
};

#endif