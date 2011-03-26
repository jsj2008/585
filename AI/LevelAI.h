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
typedef std::vector<Path> Paths;

private:
	float xscale;
	float yscale;
	float zscale;
	float width;
	float height;

	jeepSegments segments;
	jeepPositions pathPositions;
	Paths paths;

	Point closestPointOnPath(Point pathSegStart, Point pathSegEnd, Point actorPos, int* end);
	double progressOnSeg(Point pathSegStart, Point pathSegEnd, Point actorPos);
	Path getPlayerPath(int p);
	double playerProgress(int p);

public:	
	LevelAI();
	LevelAI(Jeeps jeeps, JeepActor* human);
	virtual ~LevelAI();
	void restart();

	Jeeps jeeps;
	JeepActor* human;

	void step();
	btVector3 getPathDirection(int lookAhead, int c);
	btVector3 getVectorToTrack(int c);
	btVector3 getVectorToSeg(int lookAhead, int c);

	int getPlayerPlace(int p);

};
#endif
