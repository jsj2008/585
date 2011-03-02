#ifndef LEVELAI_H
#define LEVELAI_H

#include "Common/prelude.h"
#include "Common/Actor.h"
#include "Path.h"
#include <SDL.h>
#include <string>
#include <iostream>
using namespace std;

class LevelAI {

private:
	float xscale;
	float yscale;
	float zscale;

	int currentPlayerSeg;
	Point playerPathPos;

	Point closestPointOnPath(Point pathSegStart, Point pathSegEnd, Point actorPos, int* end);

public:	
	LevelAI();
	LevelAI(Actor* playerActor);
	virtual ~LevelAI();

	Actor* playerActor;
	Path path;

	void step();

};
#endif
