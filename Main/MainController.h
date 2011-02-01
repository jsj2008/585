#pragma once
#include "IController.h"
#include "UI/Window.h"
#include <Renderer/Renderer.h>
#include <Renderer/GLDebugDrawer.h>
#include <Physics/IPhysics.h>
#include <Physics/libCube.h>
#include <Physics/libPlane.h>
#include "Physics/libChasis.h"
#include <Common/prelude.h>
#include <Common/Actor.h>
#include <Common/Point.h>
#include "Renderer/depend.h"
#include "Common/SettingsFactory.h"
#include "Driving/JeepActor.h"

class MainController : public IController{
public:
	MainController();
	virtual ~MainController();
	void explode();
	void yield();
	void tick(unsigned long);
	void moveForward(bool);
	void moveBackward(bool);
	
protected:
	SettingsFactory settings_singleton;
	ActorList actorList;
	GLDebugDrawer debugger;
	Renderer * renderer;
	Physics * const physics;
	Window window;
	
	/*more game-oriented stuff*/
	JeepActor * jeep;
	
	/*objShapes*/
	const libCube mCube;
	const libPlane mPlane;
	const libChasis mChasis;
};
