#ifndef PHYSICSFACTORY_H
#define PHYSICSFACTORY_H

#include "Physics.h"
#include "IPhysics.h"
#include "Common/prelude.h"
#include <LinearMath/btIDebugDraw.h>

class PhysicsFactory
{

public:
	static Physics * newPhysics(ActorList const & , btIDebugDraw &);
	
};

#endif