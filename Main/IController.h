#ifndef ISCONTROLLER_H
#define ISCONTROLLER_H

class IController
{
public:
	virtual void yield() =0;
	virtual void tick(unsigned long)=0;
	virtual ~IController(){};
};
#endif