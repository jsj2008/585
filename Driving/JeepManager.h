#ifndef JEEPMANAGER_H
#define JEEPMANAGER_H

#include "Physics/Physics.h"
#include "Physics/libChasis.h"
#include "AI/AIInput.h"
#include "AI/LevelAI.h"
#include "Renderer/RenderObject.h"

class JeepActor;
class JeepManager
{
public:
	typedef std::vector<JeepActor *> Jeeps;
	typedef std::vector<AIInput *> AIs;
	JeepManager(Physics *, ActorList *, Input *);
	static void tickCallback(btDynamicsWorld *world, btScalar timeStep);
	void renderTick();
	JeepActor* getHuman();
	Jeeps getAIs();

private:
	Jeeps aiJeeps;
	AIs aiInputs;
	LevelAI * levelAI;
	JeepActor * human;
	const libChasis mChasis;
	
	RenderObject jeepModel;
};

#endif