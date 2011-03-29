#ifndef JEEPENGINE_H
#define JEEPENGINE_H

class JeepEngine
{
public:
	JeepEngine();
	void step(bool);
	void accelerate(float, bool);
	void decelerate(float, bool);
    void sound(unsigned int);
	float torque;

private:
    float engine_pitch;
	float engine_torque;
	const float min_torque;
	const float max_torque;
	const float torque_decay;
    int gear;
	
};

#endif