#ifndef PHYSOBJECT_H
#define PHYSOBJECT_H

#include "Common/prelude.h"
#include <btBulletDynamicsCommon.h>

class PhysObject
{
public:	
	PhysObject(Real mass, bool isCourse = false);
	virtual ~PhysObject();
	
	btCollisionShape * shape;
	const Real mass;
	bool isCourse;
	btVector3 * fallInertia;
	
};

#endif