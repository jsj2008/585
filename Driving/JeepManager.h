#ifndef JEEPMANAGER_H
#define JEEPMANAGER_H

#include "Physics/libChasis.h"
#include "Renderer/RenderObject.h"

class Physics;
class JeepActor;
class Input;
class LevelAI;
class AIInput;

class JeepManager
{
public:
	typedef std::vector<JeepActor *> Jeeps;
	typedef std::vector<AIInput *> AIs;
	JeepManager();
	void restart();
	void initialize(Physics *, Input *);
	static void tickCallback(btDynamicsWorld *world, btScalar timeStep);
	void renderTick();
	JeepActor* getHuman();
	Jeeps getAIs();
	btVector3 getPlayerPos(int p) const;

private:
	Jeeps aiJeeps;
	AIs aiInputs;
	LevelAI * levelAI;
	JeepActor * human;
	const libChasis mChasis;
	
	RenderObject jeepModel;
};

#endif