#include "AIInput.h"

AIInput::AIInput(){
	XAxis = 0;  
	YAxis = 0;
	AcceleratePressed = false;  
	BrakePressed = false;  
	EBrakePressed = false;
}

void AIInput::step(JeepActor* jeep, btVector3 const & pathDir1, btVector3 const & pathDir2, btVector3 const & trackVector, btVector3 const & segmentVec1, btVector3 const & segmentVec2) {
	btVector3 trackDirection = pathDir1;
	btVector3 actorHeading = quatRotate(jeep->orientation, btVector3(1,0,0)).normalize();

	AcceleratePressed = true;

	float angleToTrack = trackDirection.dot(actorHeading);
	float turnDir = trackDirection.cross(actorHeading).getY();
	if (turnDir > 0) turnDir = 1;
	else if (turnDir < 0) turnDir = -1;
	else turnDir = 0;

	int parallelize = 0;
	//if ((1.0 - angleToTrack) * turnDir > 0.2) parallelize = 2;
	 if ((1.0 - angleToTrack) * turnDir > 0.02) parallelize = 1;
	//else if ((1.0 - angleToTrack) * turnDir < -0.2) parallelize = -2;
	else if ((1.0 - angleToTrack) * turnDir < -0.02) parallelize = -1;

	int onTrack = 0;
	btVector3 pathVec = trackVector;
	float distFromTrack = pathVec.length();

	turnDir = trackDirection.cross(pathVec).getY();
	if (turnDir > 0) turnDir = 1;
	else if (turnDir < 0) turnDir = -1;
	else turnDir = 0;

	//if (distFromTrack > 150 && turnDir > 0)  onTrack = 2;
	if (distFromTrack > 40 && turnDir > 0) onTrack = 1;
	//else if (distFromTrack > 150 && turnDir < 0)  onTrack = -2;
	else if (distFromTrack > 40 && turnDir < 0) onTrack = -1;
	//std::cout << distFromTrack << std::endl;

	float distToNextSeg = segmentVec1.length();
	float distToNextSeg2 = segmentVec2.length();
	int turnAnticipation1 = 0;
	int turnAnticipation2 = 0;
	//std::cout << distToNextSeg << "\t" << distToNextSeg2 << std::endl;

	if (distToNextSeg < 100) {
		trackDirection = pathDir1;
		angleToTrack = trackDirection.dot(actorHeading);
		turnDir = trackDirection.cross(actorHeading).getY();
		if (turnDir > 0) turnDir = 1;
		else if (turnDir < 0) turnDir = -1;
		else turnDir = 0;
		
		if ((1.0 - angleToTrack) * turnDir > 0.2) turnAnticipation1 = 2;
		else if ((1.0 - angleToTrack) * turnDir < -0.2) turnAnticipation1 = -2;
	}
	if (distToNextSeg2 < 200) {
		trackDirection = pathDir2;
		angleToTrack = trackDirection.dot(actorHeading);
		turnDir = trackDirection.cross(actorHeading).getY();
		if (turnDir > 0) turnDir = 1;
		else if (turnDir < 0) turnDir = -1;
		else turnDir = 0;
		
		if ((1.0 - angleToTrack) * turnDir > 0.2) turnAnticipation2 = 1;
		else if ((1.0 - angleToTrack) * turnDir < -0.2) turnAnticipation2 = -1;
	}
	//std::cout << parallelize << " " << onTrack << " " << turnAnticipation1 << " " << turnAnticipation2 << std::endl;

	XAxis = parallelize + onTrack + turnAnticipation1 + turnAnticipation2;
	if (XAxis > 0) XAxis = 1;
	else if (XAxis < 0) XAxis = -1;
	else XAxis = 0;
}
