#pragma once
#include "Icamera.h"
#include "../Driving/JeepActor.h"
#include "../Common/Point.h"
#include "math.h"

class Camera :
	public ICamera
{
public:

	Camera();
	~Camera();

	JeepActor * jeep;

	btVector3 jeepPosition;
	btVector3 previousPosition;
	btVector3 jeepVelocity;
	float camDistance;

	btQuaternion orientation;
	
	void attachActor(JeepActor *j);

	void lookAt(btVector3 const &velocity, btVector3 const &location);
	void step(seconds timeStep);

};
