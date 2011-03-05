#ifndef LEVELAI_H
#define LEVELAI_H

#include "Common/prelude.h"
#include "Path.h"
#include <SDL.h>
#include <string>
#include <iostream>
#include <vector>

class JeepActor;
class LevelAI {

typedef std::vector<JeepActor *> Jeeps;
typedef std::vector<int> jeepSegments;
typedef std::vector<Point> jeepPositions;

private:
	float xscale;
	float yscale;
	float zscale;

	jeepSegments segments;
	jeepPositions pathPositions;

	Point closestPointOnPath(Point pathSegStart, Point pathSegEnd, Point actorPos, int* end);

public:	
	LevelAI();
	LevelAI(Jeeps jeeps, JeepActor* human);
	virtual ~LevelAI();

	Jeeps jeeps;
	JeepActor* human;
	Path path;

	void step();
	btVector3 getPathDirection(int lookAhead, int c);
	btVector3 getVectorToTrack(int c);
	btVector3 getVectorToSeg(int lookAhead, int c);

};
#endif
