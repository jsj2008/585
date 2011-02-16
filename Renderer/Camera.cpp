#include "Camera.h"
#include <iostream>
using namespace std;

Camera::Camera()
{
	previousPosition.setX(0);
	previousPosition.setY(4);
	previousPosition.setZ(-10);
	
}

Camera::~Camera()
{

}

void Camera::attachActor(JeepActor *j)
{
	this->jeep = j;
	jeepPosition = jeep->pos;
	jeepVelocity = jeep->pos;
	orientation = jeep->orientation;
}



void Camera::lookAt(btVector3 const &velocity, btVector3 const &location)
{
	///glPushMatrix();

	//for testing purposes, everything for camera lookat has been stripped
	// out and placed into step function

	//glPopMatrix();
	
}

void Camera::step(seconds timeStep)
{
/*
	btVector3 dir_vec;
	dir_vec.setX(sin(orientation.getAngle()));
	dir_vec.setZ(cos(orientation.getAngle()));
	dir_vec.setY(0);
*/
	btVector3 dir_vec = quatRotate(orientation, btVector3(1,0,0));
	dir_vec.setY(0);
	dir_vec.normalize();


	btVector3 look_at;
	look_at.setX(jeepPosition.x() + dir_vec.x());
	//look_at.setY(jeepPosition.y());
	look_at.setY(0);
	look_at.setZ(jeepPosition.z() + dir_vec.z());

	btVector3 cam_location;
	cam_location.setX(jeepPosition.x() - dir_vec.x());
	cam_location.setY(0);
	cam_location.setZ(jeepPosition.z() - dir_vec.z());

	gluLookAt(cam_location.x(), cam_location.y(), cam_location.z(),
		look_at.x(), look_at.y(), look_at.z(), 0.0, 1.0, 0.0);

	jeepPosition = jeep->pos;
	orientation = jeep->orientation;
}