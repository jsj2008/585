#ifndef ISCONTROLLER_H
#define ISCONTROLLER_H

class IsController
{
public:
	virtual void yield() =0;
	virtual void tick()=0;
	virtual ~IsController(){};
};
#endif