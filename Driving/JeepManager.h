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
	void restartJeep(JeepActor *);
	void initialize(Physics *, Input *);
	static void tickCallback(btDynamicsWorld *world, btScalar timeStep);
	void renderTick();
	JeepActor* getHuman();
	Jeeps getAIs();
	btVector3 getPlayerPos(int p) const;
    void freezeAt(btVector3 const &);
    void release();
	int getPlayerPlace(int p);
	bool isFinished(int p);

	void startEngines();

private:
    
    void hitDetection(btDynamicsWorld *);
    
	Jeeps aiJeeps;
	AIs aiInputs;
	LevelAI * levelAI;
	JeepActor * human;
	const libChasis mChasis;
    int num_players;
    Physics * physics;
	
	RenderObject const * jeepModel1;
	RenderObject const * jeepModel2;
	RenderObject const * jeepModel3;
	RenderObject const * jeepModel4;
	RenderObject const * jeepModel5;
	RenderObject const * jeepModel6;
	RenderObject const * jeepModel7;
	RenderObject const * jeepModel8;
	RenderObject const * jeepModel9;
	RenderObject const * jeepModel10;
	RenderObject const * jeepModel11;
};

#endif