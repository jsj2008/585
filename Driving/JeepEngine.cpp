#include "JeepEngine.h"
#include <iostream>
#include "Common/SettingsFactory.h"
#include "Common/Debug.h"
#include "Audio/Sound.h"

float gears_max[] = {3.4, 3.5, 3.7, 3.9, 3.0};
float gears_top[] = {2.5, 2.7, 3.0, 2.9, 3.0};
float gears_min[] = {1.6, 1.75, 1.85, 1.95, 2.0};
float gears_ratio[] = {0.012, 0.012, 0.013, 0.014, 0.014};
float air_boost = 1.5;
int last_gear = 4;

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

void JeepEngine::accelerate(float max, bool inAir)
{
	float delta = gears_ratio[gear];
	if(inAir)
		delta *= air_boost;

    engine_pitch += delta;
    if(engine_pitch > gears_max[gear] && !inAir)
    {
        if(gear < last_gear)
        {
            gear ++;
            engine_pitch = gears_min[gear] + 0.1;
        }else
        {
            engine_pitch = gears_max[gear];
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

void JeepEngine::decelerate(float max, bool inAir)
{
	float delta = gears_ratio[gear];
	if(inAir)
		delta *= air_boost;

    engine_pitch -= delta;
    if(engine_pitch < gears_min[gear] && !inAir)
    {
        if(gear > 0)
        {
            gear --;
            engine_pitch = gears_top[gear + 1];
        }else
        {
            engine_pitch = gears_min[gear];
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

void JeepEngine::step(bool inAir)
{
	float delta = 0.02;
	if(inAir)
		delta *= air_boost;

    engine_pitch -= delta;
    if(engine_pitch < gears_min[gear]  && !inAir)
    {
        if(gear > 0)
        {
            gear --;
            engine_pitch = gears_top[gear + 1];
        }else
        {
            engine_pitch += delta;
        }
    }
    // torque /= torque_decay;      
}