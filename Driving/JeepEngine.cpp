#include "JeepEngine.h"
#include <iostream>
#include "Common/SettingsFactory.h"
#include "Common/Debug.h"
#include "Audio/Sound.h"

float gears_max[] = {1.0, 1.2, 1.4, 1.7};
float gears_min[] = {0.6, 0.65, 0.7, 0.75};

int last_gear = 3;

JeepEngine::JeepEngine() :
engine_torque( LoadFloat("config/jeep_springs.xml", "engine_torque") ),
max_torque( LoadFloat("config/jeep_springs.xml", "max_torque") ),
min_torque( LoadFloat("config/jeep_springs.xml", "min_torque") ),
torque_decay( LoadFloat("config/jeep_springs.xml", "torque_decay") )
{
	torque = 0;
    gear = 0;
    engine_pitch = gears_min[0];
}

void JeepEngine::sound(unsigned int alSource)
{
    Sound * ptr = Sound::GetInstance();
    ptr->setPitch(alSource, engine_pitch);
}

void JeepEngine::accelerate(float max)
{
    engine_pitch += 0.005;
    if(engine_pitch > gears_max[gear])
    {
        if(gear < last_gear)
        {
            gear ++;
            engine_pitch = gears_min[gear];
        }else
        {
            engine_pitch -= 0.005;
        }
    }
    
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
    engine_pitch -= 0.007;
    if(engine_pitch < gears_min[gear])
    {
        if(gear > 0)
        {
            gear --;
            engine_pitch = gears_max[gear];
        }else
        {
            engine_pitch += 0.007;
        }
    }
    
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
    engine_pitch -= 0.004;
    if(engine_pitch < gears_min[gear])
    {
        if(gear > 0)
        {
            gear --;
            engine_pitch = gears_max[gear];
        }else
        {
            engine_pitch += 0.004;
        }
    }
    // torque /= torque_decay;      
}