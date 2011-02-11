#pragma once

#include "Common/prelude.h"
#include "btBulletDynamicsCommon.h"

class ICamera
{
public:

	virtual void lookAt(btVector3 const &camVec, btVector3 const &carVec) = 0;
	virtual void step(seconds timeStep) = 0;

};
