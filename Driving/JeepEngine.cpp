#include "JeepEngine.h"
#include <iostream>
#include "Common/SettingsFactory.h"
#include "Common/Debug.h"

JeepEngine::JeepEngine() :
engine_torque( LoadFloat("config/jeep_springs.xml", "engine_torque") ),
max_torque( LoadFloat("config/jeep_springs.xml", "max_torque") ),
min_torque( LoadFloat("config/jeep_springs.xml", "min_torque") ),
torque_decay( LoadFloat("config/jeep_springs.xml", "torque_decay") )
{
	torque = 0;
}

void JeepEngine::accelerate(float max)
{
	if(torque < 0)
		torque = 0;
	
	torque += engine_torque;
	if(max <= 0.05)
		max = 0.05;
	if(torque > max_torque*max)
		torque = max_torque*max;
}

void JeepEngine::decelerate(float max)
{
	if(torque > 0)
		torque = 0;
	
	torque -= engine_torque;
	
	if(max <= 0.05)
		max = 0.05;
	
	if(torque < min_torque * max)
		torque = min_torque*max;
}

void JeepEngine::step(float timeStep, float accelerate)
{
	torque /= torque_decay;		
}