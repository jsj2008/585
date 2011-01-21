#ifndef IWINDOW_H
#define IWINDOW_H

#include "Main/IController.h"

class IWindow
{
public:
	virtual void updateGL() = 0;
	virtual int ScreenWidth() const = 0;
	virtual int ScreenHeight() const = 0;
	virtual int ScreenDepth() const = 0;
	virtual void run(IController *)=0;
	virtual ~IWindow(){}
};

#endif