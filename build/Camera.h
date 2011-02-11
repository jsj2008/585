#pragma once

#include "Icamera.h"
#include "math.h"

class Camera :
	public ICamera
{
public:

	Camera();
	~Camera();

	btVector3 getLastPostion();
	void setLastPosition(btVector position);

	void lookAt(btVector3 const &velocity, btVector3 const &location);
	void step(seconds timeStep);

private:

	btVector3 lastPosition;
};
