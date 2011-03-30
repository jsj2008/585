#include "LevelAI.h"
#include "Driving/JeepActor.h"
#include "Common/Debug.h"
#include "Main/MainController.h"

LevelAI::LevelAI(Jeeps jeeps, JeepActor* human) :
segments(LoadInt("config/ai.xml","num_players")+1, 0),
pathPositions(LoadInt("config/ai.xml","num_players")+1, Point(0,0,0)) {
	this->jeeps = jeeps;
	this->human = human;
	this->jeeps.push_back(human);
	paths = Paths();
	paths.push_back(Path("mainPath+.pth"));
	paths.push_back(Path("path2x.pth"));
	//paths.push_back(Path("rediculous.pth"));
	//path = Path("crashCourse.pth");

	// redundant!!! Improve?
	xscale = LoadFloat("config/world.xml","height_map_scale_x");		// Load the scaling information
	yscale = LoadFloat("config/world.xml","height_map_scale_y");
	zscale = LoadFloat("config/world.xml","height_map_scale_z");
	width = LoadFloat("config/world.xml","height_map_width");
	height = LoadFloat("config/world.xml","height_map_height");
}

LevelAI::~LevelAI() { }

void LevelAI::restart()
{
	for(jeepSegments::iterator itr = segments.begin(); itr != segments.end(); ++itr)
	{
		(*itr) = 0;
	}
}

void LevelAI::step() {
	int c = 0;
	Path path;
	for (Jeeps::iterator itr = jeeps.begin(); itr != jeeps.end(); ++itr, c++) {
		path = getPlayerPath(c);
		btVector3 playerPos = jeeps[c]->pos;
		Point lookAhead;
		Point playerWorldPos = Point(playerPos.getX() + (width/2.0) * xscale, playerPos.getY(), playerPos.getZ() + (height/2.0) * zscale);
		//std::cout << "(" << playerWorldPos.x << ", " << playerWorldPos.y << ", " << playerWorldPos.z << ") ";
		//std::cout << "(" << pathPositions[c].x << ", " << pathPositions[c].y << ", " << pathPositions[c].z << ")" << std::endl;

		int result = 0;
		int resultl = 0;
		int &currentPlayerSeg = segments[c];
		if (currentPlayerSeg < path.length() - 1) {
			pathPositions[c] = closestPointOnPath(path.at(currentPlayerSeg), path.at(currentPlayerSeg+1), playerWorldPos, &result);
			lookAhead = closestPointOnPath(path.at(currentPlayerSeg), path.at(currentPlayerSeg+1), playerWorldPos, &resultl);

			if (lookAhead.distanceTo(playerWorldPos) < pathPositions[c].distanceTo(playerWorldPos)) {
				pathPositions[c] = lookAhead;
				currentPlayerSeg++;
			} else if (result < 0) {
				if (currentPlayerSeg != 0) currentPlayerSeg--;
				//else std::cout << "Whoa!" << std::endl;
				pathPositions[c] = closestPointOnPath(path.at(currentPlayerSeg), path.at(currentPlayerSeg+1), playerWorldPos, &result);
			} else if (result > 0) {
				if (currentPlayerSeg < path.length() - 2) currentPlayerSeg++;
				else {
					currentPlayerSeg = 0;
					if(c == 0)	//main player
					{
						MainController::finishGame();
					}
					LOG("Player " << c << " has finished the race", "ai");
				}
				pathPositions[c] = closestPointOnPath(path.at(currentPlayerSeg), path.at(currentPlayerSeg+1), playerWorldPos, &result);
			}
		}

		//path.debugDraw(pathPositions[c], Point(&playerPos));
		//LOG("Player place: " << getPlayerPlace(LoadInt("config/ai.xml","num_players")), "ai");
		//LOG("Player progress: " << playerProgress(LoadInt("config/ai.xml","num_players")), "ai");
	}
}

btVector3 LevelAI::getPlayerPosition(int c) {
	Point pos = pathPositions[c];
	return btVector3(pos.x, pos.y, pos.z);
}

Point LevelAI::closestPointOnPath(Point pathSegStart, Point pathSegEnd, Point actorPos, int* end) {
	*end = 0; // Indicates result somewhere in the middle
	Vector3 startToPos = actorPos - pathSegStart;
	Vector3 heading = pathSegEnd - pathSegStart;
	float ab2 = heading.x*heading.x + heading.z*heading.z;
	float ap_ab = startToPos.x*heading.x + startToPos.z*heading.z;
	float t = ap_ab / ab2;
	
	if (t < 0.0f) {
		t = 0.0f;
		*end = -1; // Indicates result at start of line segment
	} else if (t > 1.0f) {
		t = 1.0f;
		*end = 1; // Indicates result at end of line segment
	}

	Point closest = pathSegStart + (heading * t);
	return closest;
}

double LevelAI::progressOnSeg(Point pathSegStart, Point pathSegEnd, Point actorPos) {
	Vector3 startToPos = actorPos - pathSegStart;
	Vector3 heading = pathSegEnd - pathSegStart;
	float ab2 = heading.x*heading.x + heading.z*heading.z;
	float ap_ab = startToPos.x*heading.x + startToPos.z*heading.z;
	float t = ap_ab / ab2;
	
	if (t < 0.0f) t = 0.0f;
	else if (t > 1.0f) t = 1.0f;

	return t;
}

btVector3 LevelAI::getPathDirection(int lookAhead, int c) {
	Point bp, ep;
	int currentPlayerSeg = segments[c];
	Path path = getPlayerPath(c); // The path AI player 'c' is following
	if (currentPlayerSeg < path.length() - 1 - lookAhead) {
		bp = path.at(currentPlayerSeg + lookAhead);
		ep = path.at(currentPlayerSeg + 1 + lookAhead);

	} else {
		return getPathDirection(lookAhead-1, c); // Give a previous result
	}
	return btVector3(ep.x - bp.x, ep.y - bp.y, ep.z - bp.z).normalize();
}

btVector3 LevelAI::getVectorToTrack(int c) {
	Point playerPathPos = pathPositions[c];
	btVector3 playerPos = jeeps[c]->pos;
	btVector3 playerWorldPos = btVector3(playerPos.getX() + (width/2.0) * xscale, playerPos.getY(), playerPos.getZ() + (height/2.0) * zscale);
	btVector3 playerPathPosv = btVector3(playerPathPos.x, playerPathPos.y, playerPathPos.z);
	return playerWorldPos - playerPathPosv;
}

btVector3 LevelAI::getVectorToSeg(int lookAhead, int c) {
	int currentPlayerSeg = segments[c];
	Path path = getPlayerPath(c); // The path AI player 'c' is following
	Point nextSegPoint;
	btVector3 playerPos = jeeps[c]->pos;
	btVector3 playerWorldPos = btVector3(playerPos.getX() + (width/2.0) * xscale, playerPos.getY(), playerPos.getZ() + (height/2.0) * zscale);
	if (currentPlayerSeg < path.length() - lookAhead) {
		nextSegPoint = path.at(currentPlayerSeg+lookAhead);
	} else
		return getVectorToSeg(lookAhead-1, c);
	btVector3 nextSegV = btVector3(nextSegPoint.x, nextSegPoint.y, nextSegPoint.z);
	return playerWorldPos - nextSegV;
}

int LevelAI::getPlayerPlace(int p) {
	int place = 1;
	for (int i = 0; i < LoadInt("config/ai.xml","num_players")+1; i++) {
		if (i == p) continue; // Don't compare to self
		if (playerProgress(i) > playerProgress(p)) place++;
	}
	return place;
}

Path LevelAI::getPlayerPath(int p) {
	return paths[p % paths.size()]; // The path player 'p' is following
}

double LevelAI::playerProgress(int p) {
	Path playerPath = getPlayerPath(p);
	btVector3 playerPos = jeeps[p]->pos;
	Point playerWorldPos = Point(playerPos.getX() + (width/2.0) * xscale, playerPos.getY(), playerPos.getZ() + (height/2.0) * zscale);

	double segStartProgress = playerPath.pointProgress.at(segments[p]);
	double segEndProgress = playerPath.pointProgress.at(segments[p]+1);
	double segProgress = progressOnSeg(playerPath.points.at(segments[p]), playerPath.points.at(segments[p]+1), playerWorldPos);
	return segStartProgress * (1-segProgress) + segEndProgress * segProgress;
}
