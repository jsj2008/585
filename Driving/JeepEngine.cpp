#include "JeepEngine.h"
#include <iostream>

JeepEngine::JeepEngine()
{
	m_torque = 1000;
}

void JeepEngine::step(seconds const timeStep, btScalar const accelerate)
{
	m_torque += accelerate * 400;	//for now assume linear torque
	m_torque -= timeStep * 1200;	
	
	if(m_torque < MIN_TORQUE)
	{
		m_torque = MIN_TORQUE;
	}
	
	if(m_torque > MAX_TORQUE)
	{
		m_torque = MAX_TORQUE;
	}
	
}

btScalar JeepEngine::getRPM() const
{
	return m_torque / 100;// / 250.0;	//giant hack (do real rpm/torque at some point)
}