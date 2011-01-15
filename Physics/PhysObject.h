#ifndef PHYSOBJECT_H
#define PHYSOBJECT_H

#include "../Common/prelude.h"
#include <btBulletDynamicsCommon.h>

class PhysObject
{
public:
	PhysObject(Point startPos);	
	~PhysObject();
private:
	btDefaultMotionState* motionState;
};

#endif