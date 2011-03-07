#ifndef IPHYSICS_H
#define IPHYSICS_H

#include "Common/prelude.h"
#include "Driving/IVehicle.h"
#include <btBulletDynamicsCommon.h>


class IPhysics
{
public:
	virtual ~IPhysics(){}
	virtual void newActors(ActorList const &)=0;
	virtual void step(seconds)=0;
	virtual btRigidBody * const newActor(Actor * const)=0;
};

#endif