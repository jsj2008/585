#ifndef IVEHICLE_H
#define IVEHICLE_H
#include <btBulletDynamicsCommon.h>

class IVehicle
{
public:
	virtual ~IVehicle(){}
	virtual void tick()=0;
	virtual void getRays(btVector3 * const, btVector3 * const) const =0;
	
};

#endif