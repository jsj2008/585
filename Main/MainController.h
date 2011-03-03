#pragma once
#include "IController.h"
#include "UI/Window.h"
#include <AI/LevelAI.h>
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
	LevelAI * levelAI;
	Physics * const physics;
	
	/*more game-oriented stuff*/
	JeepActor * jeep;
	JeepActor * jeep2;
	JeepActor * jeep3;
	JeepActor * jeep4;
	JeepActor * jeep5;
	JeepActor * jeep6;
					
	/*objShapes*/
	const libCube mCube;
	const libPlane mPlane;
	const libChasis mChasis;

	/*renderObjectTest*/
	RenderObject jeepModel;
	RenderObject cubeModel;
	RenderObject planeModel;
	
	JeepManager::Jeeps jeeps;
	JeepManager * jeepManager;
	
};
