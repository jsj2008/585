#pragma once

#include "Common/prelude.h"
#include "btBulletDynamicsCommon.h"

class ICamera
{
public:

	virtual void lookAt(btVector3 const &velocity, btVector3 const &location) = 0;
	virtual void step(seconds timeStep) = 0;

};
