#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}


btVector3 Camera::getLastPostion(){

	return lastPosition;

}


void Camera::setLastPosition(btVector position){

	lastPosition = position;

}

void Camera::lookAt(btVector3 const &velocity, btVector3 const &location)
{
	//if car velocity == 0, start point will be car location (minus length)
	float magnitude = sqrt(velocity.getX()^2 + velocity.getY()^2 + velocity.getZ()^2);
	//camera placed on velocity vector
	//some fixed distance from the car position
	GLUlookAt(getLastPosition().getX(), getLastPosition().getY(), getLastPosition().getZ(),
		location.getX(), location.getY(), location.getZ(), 0.0, 0.0, 1.0);
	//need to reset last to current
}

void Camera::step(seconds timeStep)
{

}