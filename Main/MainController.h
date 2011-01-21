#pragma once
#include "IController.h"
#include "UI/Window.h"
#include <Renderer/Renderer.h>
#include <Renderer/GLDebugDrawer.h>
#include <Physics/IPhysics.h>
#include <Physics/libCube.h>
#include <Physics/libPlane.h>
#include <Common/prelude.h>
#include <Common/Actor.h>
#include <Common/Point.h>
#include "Renderer/depend.h"

class MainController : public IController{
public:
	MainController();
	virtual ~MainController();
	void explode();
	void yield();
	void tick(unsigned long);

protected:
	ActorList actorList;
	GLDebugDrawer debugger;
	Renderer * renderer;
	IPhysics * physics;
	Window window;
	
	/*objShapes*/
	const libCube mCube;
	const libPlane mPlane;
};
