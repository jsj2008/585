#ifndef ISWINDOW_H
#define ISWINDOW_H

#include "Main/IsController.h"

class IsWindow
{
public:
	virtual void updateGL() = 0;
	virtual int ScreenWidth() = 0;
	virtual int ScreenHeight() = 0;
	virtual int ScreenDepth() = 0;
	virtual void run(IsController *)=0;
	virtual ~IsWindow(){};
};

#endif