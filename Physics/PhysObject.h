#ifndef PHYSOBJECT_H
#define PHYSOBJECT_H

#include "Common/prelude.h"
#include <btBulletDynamicsCommon.h>

class PhysObject
{
public:	
	PhysObject();
	virtual ~PhysObject();
	
	btCollisionShape * shape;
	Real mass;
	btVector3 * fallInertia;
	
};

#endif