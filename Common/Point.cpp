#include "Point.h"

Point::Point() {
	x = 0;
	y = 0;
	z = 0;
}

Point::Point(btVector3* vec) {
	x = vec->getX();
	y = vec->getY();
	z = vec->getZ();
}

// 2D constructor
Point::Point(double x, double y) {
	this->x = x;
	this->y = y;
	this->z = 0;
}

// 3D constructor
Point::Point(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

// Get the distance to a point from this one
double Point::distanceTo(Point a) {
	double distance = pow((pow((x - a.x), 2.0) + pow((y - a.y), 2.0) + pow((z - a.z), 2.0)) ,0.5);
	return distance;
}

// Get the normal based on the triangle formed by this point and the given neighbours
Vector3 Point::getNormal(Point a, Point b) { 
	Vector3 v1 = a - *this;
	Vector3 v2 = b - *this;
	Vector3 c = Vector3::cross(v1, v2);
	c = c / c.getMagnitude();
	return c;
}

// Draw the point using the given size
void Point::draw(double size) {
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex3f(x, y, z);
	glEnd();
}

// Rounds the coordinates to integers and returns the result
Point Point::round() {
	int rx, ry, rz;
	if ((int)(x*10) % 10 >= 5) rx = x + 1;
	else rx = x;
	if ((int)(y*10) % 10 >= 5) ry = y + 1;
	else ry = y;
	if ((int)(z*10) % 10 >= 5) rz = z + 1;
	else rz = z;

	return Point(rx, ry, rz);
}
