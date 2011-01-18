#pragma once
#include "IsController.h"
#include "UI/IsWindow.h"
#include "UI/Window.h"
#include <Renderer/Renderer.h>
#include <Renderer/GLDebugDrawer.h>
#include <Physics/Physics.h>
#include <Common/prelude.h>
#include <Common/Actor.h>
#include <Common/Point.h>
#include "Renderer/depend.h"

class MainController : public IsController{
public:
	MainController();
	virtual ~MainController();
	void explode();
	void yield();

protected:
	Renderer * renderer;
	Physics * physics;
	GLDebugDrawer * debugger;
	ActorList * actorList;
	IsWindow * window;
	
	/*objShapes*/
	PhysObject * cube;
	PhysObject * plane;
};
