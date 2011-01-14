#pragma once

#include "Vector3.h"
#include <math.h>
#include <QtOpenGL>

class Point {
	public:
		double x, y, z;

		Point();
		Point(double x, double y);
		Point(double x, double y, double z);
		double distanceTo(Point a);
		Vector3 getNormal(Point a, Point b);
		void draw(double size);
		Point round();

		Point operator +(Point p) { 
			double newX = x + p.x;
			double newY = y + p.y;
			double newZ = z + p.z;
			return Point(newX, newY, newZ);
		}
	
		Point operator +(Vector3 v) { 
			double newX = x + v.x;
			double newY = y + v.y;
			double newZ = z + v.z;
			return Point(newX, newY, newZ);
		}

		Vector3 operator -(Point p) { 
			double newX = x - p.x;
			double newY = y - p.y;
			double newZ = z - p.z;
			return Vector3(newX, newY, newZ);
		}

		Point operator -(Vector3 v) { 
			double newX = x - v.x;
			double newY = y - v.y;
			double newZ = z - v.z;
			return Point(newX, newY, newZ);
		}

		Point operator *(double c) { 
			double newX = c * x;
			double newY = c * y;
			double newZ = c * z;
			return Point(newX, newY, newZ);
		}

		Point operator /(double c) { 
			double newX = x / c;
			double newY = y / c;
			double newZ = z / c;
			return Point(newX, newY, newZ);
		}
		
		bool operator ==(Point p) { 
			return (x == p.x) && (y == p.y) && (z == p.z);
		}
		
		bool operator !=(Point p) { 
			return (x != p.x) || (y != p.y) || (z != p.z);
		}
};
