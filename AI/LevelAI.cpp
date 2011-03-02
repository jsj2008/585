#include "LevelAI.h"

LevelAI::LevelAI(Actor* playerActor) {
	this->playerActor = playerActor;
	path = Path();

	currentPlayerSeg = 0;

	// redundant!!! Improve?
	xscale = LoadFloat("config/world.xml","height_map_scale_x");		// Load the scaling information
	yscale = LoadFloat("config/world.xml","height_map_scale_y");
	zscale = LoadFloat("config/world.xml","height_map_scale_z");
}

LevelAI::~LevelAI() { }

void LevelAI::step() {
	// 256 is hm width / 2 <- make it less hacky
	btVector3 playerPos = playerActor->pos;
	Point playerWorldPos = Point(playerPos.getX() + 256*xscale, playerPos.getY() * yscale, playerPos.getZ() + 256 * zscale);
	//std::cout << "(" << playerWorldPos.x << ", " << playerWorldPos.y << ", " << playerWorldPos.z << ")" << std::endl;

	int result = 0;
	if (currentPlayerSeg < path.length() - 1) {
		playerPathPos = closestPointOnPath(path.at(currentPlayerSeg), path.at(currentPlayerSeg+1), playerWorldPos, &result);

		if (result < 0) {
			if (currentPlayerSeg != 0) currentPlayerSeg--;
			else std::cout << "Whoa!" << std::endl;
			playerPathPos = closestPointOnPath(path.at(currentPlayerSeg), path.at(currentPlayerSeg+1), playerWorldPos, &result);
		} else if (result > 0) {
			if (currentPlayerSeg < path.length() - 2) currentPlayerSeg++;
			else std::cout << "Your teh Winnre!!!11one" << std::endl;
			playerPathPos = closestPointOnPath(path.at(currentPlayerSeg), path.at(currentPlayerSeg+1), playerWorldPos, &result);
		}
	} else {
		std::cout << "Your teh Winnre!!!11one" << std::endl;
	}

	path.debugDraw(playerPathPos);
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
