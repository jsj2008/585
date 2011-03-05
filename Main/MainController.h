#pragma once
#include "IController.h"
#include "UI/Window.h"
#include <AI/LevelAI.h>
#include <Renderer/Renderer.h>
#include <Renderer/GLDebugDrawer.h>
#include <Physics/IPhysics.h>
#include <Physics/libCube.h>
#include <Physics/libPlane.h>
#include <Common/prelude.h>
#include <Common/Actor.h>
#include <Common/Point.h>
#include "Renderer/depend.h"
#include "Common/SettingsFactory.h"
#include "Driving/JeepActor.h"
#include "Driving/JeepManager.h"


class MainController : public IController{
public:
	MainController();
	virtual ~MainController();
	void explode();
	void yield();
	void tick(unsigned long);
	
	static void addActor(Actor *);
	Window window;
	
protected:
	static MainController * ptr;
	ActorList actorList;
	GLDebugDrawer debugger;
	Renderer * renderer;
	Physics * const physics;

	/*objShapes*/
	const libCube mCube;
	const libPlane mPlane;
	
	JeepManager * jeepManager;
	
};
