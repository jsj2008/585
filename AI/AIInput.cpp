#include "AIInput.h"
#include "Common/Debug.h"
#include "Driving/JeepActor.h"
#include <btBulletDynamicsCommon.h>

AIInput::AIInput(){
	XAxis = 0;  
	YAxis = 1.5;
	AcceleratePressed = false;  
	BrakePressed = false;  
	EBrakePressed = false;
	recovering = LoadFloat("config/ai.xml","recovery_cooldown");
}

void AIInput::step(JeepActor* jeep, Jeeps allJeeps, btVector3 const & pathDir1, btVector3 const & pathDir2, btVector3 const & trackVector, btVector3 const & segmentVec1, btVector3 const & segmentVec2) {
	btVector3 trackDirection = pathDir1;
	btVector3 actorHeading = quatRotate(jeep->orientation, btVector3(1,0,0)).normalize();

	AcceleratePressed = true;
	BrakePressed = false;

	float angleToTrack = trackDirection.dot(actorHeading);
	float turnDir = trackDirection.cross(actorHeading).getY();
	if (turnDir > 0) turnDir = 1;
	else if (turnDir < 0) turnDir = -1;
	else turnDir = 0;

	int parallelize = 0; // Try to stay parallel with the path
	float parallelizationThreshold = LoadFloat("config/ai.xml","parallelization_threshold"); // Try to align with the track if off by this angle
	float hardParallelizationThreshold = LoadFloat("config/ai.xml","hard_parallelization_threshold"); // Try hard to align with the track if off by this angle
	if ((1.0 - angleToTrack) * turnDir > hardParallelizationThreshold) parallelize = 2;
	if ((1.0 - angleToTrack) * turnDir > parallelizationThreshold) parallelize = 1;
	else if ((1.0 - angleToTrack) * turnDir < -hardParallelizationThreshold) parallelize = -2;
	else if ((1.0 - angleToTrack) * turnDir < -parallelizationThreshold) parallelize = -1;

	int onTrack = 0; // Don't stray too far from the path
	btVector3 pathVec = trackVector;
	float distFromTrack = pathVec.length();

	turnDir = trackDirection.cross(pathVec).getY();
	if (turnDir > 0) turnDir = 1;
	else if (turnDir < 0) turnDir = -1;
	else turnDir = 0;

	float distanceThreshold = LoadFloat("config/ai.xml","distance_threshold"); // Get back on track if farther than this distance from the path
	float farDistanceThreshold = LoadFloat("config/ai.xml","far_distance_threshold"); // Turn harder if this far from the path
	if (distFromTrack > farDistanceThreshold && turnDir > 0)  onTrack = 2;
	else if (distFromTrack > distanceThreshold && turnDir > 0) onTrack = 1;
	else if (distFromTrack > farDistanceThreshold && turnDir < 0)  onTrack = -2;
	else if (distFromTrack > distanceThreshold && turnDir < 0) onTrack = -1;
	//std::cout << distFromTrack << std::endl;

	float distToNextSeg = segmentVec1.length();
	float distToNextSeg2 = segmentVec2.length();
	int turnAnticipation1 = 0; // Anticipate curves that are one track segment away
	int turnAnticipation2 = 0; // Anticipate curves that are two track segments away
	//std::cout << distToNextSeg << "\t" << distToNextSeg2 << std::endl;

	float turnAnticipationThreshold = LoadFloat("config/ai.xml","turn_anticipation_threshold"); // Don't anticipate if the next curve is farther than this
	float farTurnAnticipationThreshold = LoadFloat("config/ai.xml","far_turn_anticipation_threshold");
	if (distToNextSeg < turnAnticipationThreshold) {
		trackDirection = pathDir1;
		angleToTrack = trackDirection.dot(actorHeading);
		turnDir = trackDirection.cross(actorHeading).getY();
		if (turnDir > 0) turnDir = 1;
		else if (turnDir < 0) turnDir = -1;
		else turnDir = 0;
		
		if ((1.0 - angleToTrack) * turnDir > 0.2) turnAnticipation1 = 2;
		else if ((1.0 - angleToTrack) * turnDir < -0.2) turnAnticipation1 = -2;
	}
	if (distToNextSeg2 < farTurnAnticipationThreshold) {
		trackDirection = pathDir2;
		angleToTrack = trackDirection.dot(actorHeading);
		turnDir = trackDirection.cross(actorHeading).getY();
		if (turnDir > 0) turnDir = 1;
		else if (turnDir < 0) turnDir = -1;
		else turnDir = 0;
		
		if ((1.0 - angleToTrack) * turnDir > 0.2) turnAnticipation2 = 1;
		else if ((1.0 - angleToTrack) * turnDir < -0.2) turnAnticipation2 = -1;
	}

	int jeepAvoidance = 0; // turn away from other jeeps if too close
	btVector3 jeepVec;
	float distFromJeep;
	for (Jeeps::iterator itr = allJeeps.begin(); itr != allJeeps.end(); ++itr) {
		if ((*itr) == jeep) continue; // Don't avoid yourself!
		jeepVec = (*itr)->pos - jeep->pos;
		distFromJeep = jeepVec.length();
		//LOG((*itr)->speed << "\t\t" << distFromJeep, "ai");

		turnDir = trackDirection.cross(jeepVec).getY();
		if (turnDir > 0) turnDir = 1;
		else if (turnDir < 0) turnDir = -1;
		else turnDir = 0;

		float jeepDistanceThreshold = LoadFloat("config/ai.xml","jeep_distance_threshold"); // Turn of closer than this to other jeeps
		if (distFromJeep < jeepDistanceThreshold && turnDir > 0) jeepAvoidance += 2;
		else if (distFromJeep < jeepDistanceThreshold && turnDir < 0) jeepAvoidance -= 2;
	}

	float easeThreshold = LoadFloat("config/ai.xml","ease_threshold"); // Ease off the gas if going faster than this (on a sharp turn)
	float brakeThreshold = LoadFloat("config/ai.xml","brake_threshold"); // Brake if going faster than this (on a very sharp turn)
	float stuckThreshold = LoadFloat("config/ai.xml","stuck_threshold"); // If slower than this, try backing up
	float recoveryTime = LoadFloat("config/ai.xml","recovery_time"); // How long to reverse when stuck
	float recoveryCooldown = LoadFloat("config/ai.xml","recovery_cooldown"); // How long to wait before trying to recover again

	XAxis = parallelize + onTrack + turnAnticipation1 + turnAnticipation2 + jeepAvoidance;
	if ((XAxis > 2 || XAxis < -2) && jeep->speed > easeThreshold) AcceleratePressed = false; // Slow down if a sharp turn needs to be made
	if ((XAxis > 4 || XAxis < -4) && jeep->speed > brakeThreshold) BrakePressed = true; // Brake if a very sharp turn needs to be made
	if (XAxis > 0) XAxis = 1;
	else if (XAxis < 0) XAxis = -1;
	else XAxis = 0;
	//if (recovering != 0) LOG(recovering, "ai");
	if (recovering < 0) recovering++; // Burn through the recovery cooldown period
	if (recovering == recoveryTime) recovering = -recoveryCooldown; // If recovery time has elapsed, start the cooldown period
	else if ((jeep->speed < stuckThreshold && recovering == 0) || recovering > 0) { // If stuck...
		recovering++; // Increment the recovery time
		AcceleratePressed = false; // Stop accelerating
		BrakePressed = true; // Back up
		XAxis *= -1; // Turn the other way
	}
	//LOG(jeep->speed, "ai");
	//LOG(parallelize << " " << onTrack << " " << turnAnticipation1 << " " << turnAnticipation2 << " " << jeepAvoidance << "\t" << AcceleratePressed, "ai");
}
