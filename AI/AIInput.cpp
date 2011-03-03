#include "AIInput.h"

AIInput::AIInput(){
	XAxis = 0;  
	YAxis = 0;
	AcceleratePressed = false;  
	BrakePressed = false;  
	EBrakePressed = false;
}

AIInput::AIInput(LevelAI* levelAI){
	this->levelAI = levelAI;
	XAxis = 0;  
	YAxis = 0;
	AcceleratePressed = false;  
	BrakePressed = false;  
	EBrakePressed = false;
}

void AIInput::step() {
	btVector3 trackDirection = levelAI->getPathDirection();
	btVector3 actorHeading = quatRotate(levelAI->playerActor->orientation, btVector3(1,0,0)).normalize();

	AcceleratePressed = true;

	float angleToTrack = trackDirection.dot(actorHeading);
	float turnDir = trackDirection.cross(actorHeading).getY();
	if (turnDir > 0) turnDir = 1;
	else if (turnDir < 0) turnDir = -1;
	else turnDir = 0;

	int parallelize = 0;
	if ((1.0 - angleToTrack) * turnDir > 0.5) parallelize = 2;
	else if ((1.0 - angleToTrack) * turnDir > 0.02) parallelize = 1;
	else if ((1.0 - angleToTrack) * turnDir < -0.2) parallelize = -2;
	else if ((1.0 - angleToTrack) * turnDir < -0.02) parallelize = -1;

	int onTrack = 0;
	btVector3 pathVec = levelAI->getVectorToTrack();
	float distFromTrack = pathVec.length();

	turnDir = trackDirection.cross(pathVec).getY();
	if (turnDir > 0) turnDir = 1;
	else if (turnDir < 0) turnDir = -1;
	else turnDir = 0;

	if (distFromTrack > 100 && turnDir > 0) {
		onTrack = 2;
		//AcceleratePressed = false;
	} else if (distFromTrack > 15 && turnDir > 0) onTrack = 1;
	else if (distFromTrack > 100 && turnDir < 0) {
		onTrack = -2;
		//AcceleratePressed = false;
	} else if (distFromTrack > 15 && turnDir < 0) onTrack = -1;

	float distToNextSeg = levelAI->getVectorToSeg(1).length();
	float distToNextSeg2 = levelAI->getVectorToSeg(2).length();
	int turnAnticipation1 = 0;
	int turnAnticipation2 = 0;
//	std::cout << distToNextSeg << "\t" << distToNextSeg2 << std::endl;

	if (distToNextSeg < 50) {
		trackDirection = levelAI->getPathDirection(1);
		angleToTrack = trackDirection.dot(actorHeading);
		turnDir = trackDirection.cross(actorHeading).getY();
		if (turnDir > 0) turnDir = 1;
		else if (turnDir < 0) turnDir = -1;
		else turnDir = 0;
		
		if ((1.0 - angleToTrack) * turnDir > 0.2) turnAnticipation1 = 1;
		else if ((1.0 - angleToTrack) * turnDir < -0.2) turnAnticipation1 = -1;
	}
	if (distToNextSeg2 < 150) {
		trackDirection = levelAI->getPathDirection(2);
		angleToTrack = trackDirection.dot(actorHeading);
		turnDir = trackDirection.cross(actorHeading).getY();
		if (turnDir > 0) turnDir = 1;
		else if (turnDir < 0) turnDir = -1;
		else turnDir = 0;
		
		if ((1.0 - angleToTrack) * turnDir > 0.2) turnAnticipation2 = 1;
		else if ((1.0 - angleToTrack) * turnDir < -0.2) turnAnticipation2 = -1;
	}
	std::cout << parallelize << " " << onTrack << " " << turnAnticipation1 << " " << turnAnticipation2 << std::endl;

	XAxis = parallelize + onTrack + turnAnticipation1 + turnAnticipation2;
	if (XAxis > 0) XAxis = 1;
	else if (XAxis < 0) XAxis = -1;
	else XAxis = 0;

	
	/*std::cout << "--------------------" << std::endl;
	std::cout << trackDirection.getX() << " " << trackDirection.getY() << " " << trackDirection.getZ() << std::endl;
	std::cout << actorHeading.getX() << " " << actorHeading.getY() << " " << actorHeading.getZ() << std::endl;
	std::cout << angleToTrack << " " << turnDir << std::endl;*/
}

void AIInput::setLevelAI(LevelAI* levelAI) {
	this->levelAI = levelAI;
}
