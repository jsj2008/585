#pragma once

#include <math.h>
#include <QtOpenGL>
#include "Point.h"
using namespace std;

class Trackball{
	public:

		Trackball();
		Trackball(double scale);
		void press(Point point);
		void release();
		void move(Point point);
		void reset();
		float* getMatrix();
		void setMatrix(float* matrix);
		Vector3 getPoint();

	private:
		void calculateMatrix(Point point);

		Vector3 previousPoint;	// Vector to  the previous point clicked on the trackball, being rotated from
		double scale;			// The scale of the trackball, dictating how sensitive it is to mouse movements
		bool pressed;			// Specifies whether or not the trackball is currently being pressed
		float rotMatrix[16];	// The trackball's rotation matrix
		Vector3 spherePoint;	// Vector to the current point on the sphere
};

