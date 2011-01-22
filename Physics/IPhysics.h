#ifndef IPHYSICS_H
#define IPHYSICS_H

#include "Common/prelude.h"

class IPhysics
{
public:
	virtual ~IPhysics(){}
	virtual void newActors(ActorList const &)=0;
	virtual void step(seconds)=0;
};

#endif