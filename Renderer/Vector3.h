#pragma once

#include <math.h>

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679

class Vector3 {
	public:
		double x, y, z;

		Vector3();
		Vector3(double x, double y);
		Vector3(double x, double y, double z);
		Vector3 normalize();
		void rotate(Vector3 axis, double angle);
		double getMagnitude();
		static Vector3 cross(Vector3 u, Vector3 v);
		double angleBetween(Vector3 vec);

		Vector3 operator +(Vector3 v) { 
			double newX = x + v.x;
			double newY = y + v.y;
			double newZ = z + v.z;
			return Vector3(newX, newY, newZ);
		}
		
		Vector3 operator -(Vector3 v) { 
			double newX = x - v.x;
			double newY = y - v.y;
			double newZ = z - v.z;
			return Vector3(newX, newY, newZ);
		}

		Vector3 operator *(double c) { 
			double newX = c * x;
			double newY = c * y;
			double newZ = c * z;
			return Vector3(newX, newY, newZ);
		}
		
		Vector3 operator /(double c) { 
			double newX = x / c;
			double newY = y / c;
			double newZ = z / c;
			return Vector3(newX, newY, newZ);
		}

		// Dot product
		double operator *(Vector3 v) { 
			return x*v.x + y*v.y + z*v.z;
		}
};
