#ifndef PATH_H
#define PATH_H

#include "Common/prelude.h"
#include "Renderer/RenderObject.h"
#include "Common/SettingsFactory.h"
#include "Physics/Heightmap.h"
#include <SDL.h>
#include <string>
#include <iostream>
using namespace std;

class Path {

private:
	void addPoint(int x, int z);

public:	
	Path();
	virtual ~Path();

	RenderObject testCube;
	vector<Point> points;
	HeightMap* hm;

	float xscale;
	float yscale;
	float zscale;

	void debugDraw(Point playerPos);
	Point at(int i);
	int length();

};
#endif