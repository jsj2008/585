#include "JeepEngine.h"
#include <iostream>
#include "Common/SettingsFactory.h"

JeepEngine::JeepEngine() :
engine_torque( LoadFloat("config/jeep_springs.xml", "engine_torque") ),
max_torque( LoadFloat("config/jeep_springs.xml", "max_torque") ),
min_torque( LoadFloat("config/jeep_springs.xml", "min_torque") ),
torque_decay( LoadFloat("config/jeep_springs.xml", "torque_decay") )
{
	torque = 0;
}

void JeepEngine::accelerate()
{
	if(torque < 0)
		torque = 0;
	
	torque += engine_torque;
	if(torque > max_torque)
		torque = max_torque;
}

void JeepEngine::decelerate()
{
	if(torque > 0)
		torque = 0;
	
	torque -= engine_torque;
	if(torque < min_torque)
		torque = min_torque;
}

void JeepEngine::step(float timeStep, float accelerate)
{
	torque /= torque_decay;		
}