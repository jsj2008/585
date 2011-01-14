#include "Vector3.h"

Vector3::Vector3() {
	x = 0;
	y = 0;
	z = 0;
}

// 2D constructor
Vector3::Vector3(double newx, double newy) {
	x = newx;
	y = newy;
	z = 0;
}

// 3D constructor
Vector3::Vector3(double newx, double newy, double newz) {
	x = newx;
	y = newy;
	z = newz;
}

// Returns a normalized copy of the current vector
Vector3 Vector3::normalize() {
	double length = getMagnitude();
	if (length == 0) return Vector3(x, y, z);
	double newx = x / length;
	double newy = y / length;
	double newz = z / length;
	return Vector3(newx, newy, newz);
}

// Rotates the current vector about the given axis by the given angle
void Vector3::rotate(Vector3 axis, double angle) {
	axis = axis.normalize();
	angle *= PI/180.0; //degrees to radians
	double cosA = cos(angle);
	double sinA = sin(angle);
	double omcA = 1-cosA;
	// Insanely obtuse 3D vector rotation matrix in expanded form (http://www.cprogramming.com/tutorial/3d/rotation.html)
	double newX = (pow(omcA*axis.x, 2)+cosA)*x + ((omcA*axis.x*axis.y)+(sinA*axis.z))*y + ((omcA*axis.x*axis.z)-(sinA*axis.y))*z;
	double newY = ((omcA*axis.x*axis.y)-(sinA*axis.z))*x + (pow(omcA*axis.y, 2)+cosA)*y + ((omcA*axis.y*axis.z)+(sinA*axis.x))*z;
	double newZ = ((omcA*axis.x*axis.y)+(sinA*axis.y))*x + ((omcA*axis.y*axis.z)-(sinA*axis.x))*y + (pow(omcA*axis.z, 2)+cosA)*z;
	x = newX; y = newY; z = newZ;
}

// Returns the magnitude of the current vector
double Vector3::getMagnitude() {
	return pow((pow(x, 2) + pow(y, 2) + pow(z, 2)), 0.5);
}

// Static function to get the cross product of two vectors
Vector3 Vector3::cross(Vector3 u, Vector3 v) {
	Vector3 result;
	result.x = u.y * v.z - u.z * v.y;
	result.y = u.z * v.x - u.x * v.z;
	result.z = u.x * v.y - u.y * v.x;
	return result;
}

// Gets the angle between this vector and the given vector in radians
double Vector3::angleBetween(Vector3 vec) {
	return acos(normalize()*vec.normalize());
}
