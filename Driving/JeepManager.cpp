#include "JeepManager.h"
#include "JeepActor.h"
#include "AI/LevelAI.h"
#include "Main/MainController.h"
#include "Physics/Physics.h"
#include "AI/AIInput.h"
#include "UI/Input.h"

JeepManager::JeepManager() :
jeepModel("textures/jeep_uv2.png", "", "models/Jeep/jeep7.obj")
{}

void JeepManager::initialize(Physics * physics, Input * playerInput)
{
	float jeepX			= LoadFloat("config/start.xml", "jeepX");
	float jeepY			= LoadFloat("config/start.xml", "jeepY");
	float jeepZ 		= LoadFloat("config/start.xml", "jeepZ");
	float jeepRotation	= LoadFloat("config/start.xml", "jeepRotation");

	btQuaternion rotation(btVector3(0,1,0), jeepRotation);	//initial rotation
	
	human = new JeepActor(mChasis, jeepModel, physics, playerInput, btVector3(jeepX, jeepY, jeepZ), rotation );
	MainController::addActor(human);
	int num_players = LoadInt("config/ai.xml","num_players");
	for (int i = 0; i < num_players; ++i) {
		aiInputs.push_back(new AIInput());
		JeepActor* jeep = new JeepActor(mChasis, jeepModel, physics, aiInputs[i], btVector3(jeepX + (10*i + 10), jeepY, jeepZ), rotation );
		jeep->setOrientation( rotation );
		
		aiJeeps.push_back(jeep);
		MainController::addActor(jeep);
	}

	levelAI = new LevelAI(aiJeeps, human);

	physics->dynamicsWorld.setInternalTickCallback(tickCallback, static_cast<void *>(this), true );
	
}

void JeepManager::restart()
{
	/*TODO: reset AI segments*/
	float jeepX			= LoadFloat("config/start.xml", "jeepX");
	float jeepY			= LoadFloat("config/start.xml", "jeepY");
	float jeepZ 		= LoadFloat("config/start.xml", "jeepZ");
	float jeepRotation	= LoadFloat("config/start.xml", "jeepRotation");

	btQuaternion rotation(btVector3(0,1,0), jeepRotation);	//initial rotation
	
	human->reset(rotation, btVector3(jeepX, jeepY, jeepZ));
	int num_players = LoadInt("config/ai.xml","num_players");
	for (int i = 0; i < num_players; ++i) {
		aiJeeps[i]->reset(rotation, btVector3(jeepX + (10*i + 10), jeepY, jeepZ) );
	}
	
}

void JeepManager::tickCallback(btDynamicsWorld *world, btScalar timeStep)
{
	JeepManager * m = static_cast<JeepManager *>(world->getWorldUserInfo());
	
	m->human->tick(timeStep);
	for(Jeeps::iterator itr = m->aiJeeps.begin(); itr != m->aiJeeps.end(); ++itr)
		(*itr)->tick(timeStep);
}

void JeepManager::renderTick() {
	levelAI->step();
	human->render();
	int c = 0;
	for (Jeeps::iterator itr = aiJeeps.begin(); itr != aiJeeps.end(); ++itr, c++) {
		(*itr)->render();
		btVector3 pathDir1 = levelAI->getPathDirection(0, c);
		btVector3 pathDir2 = levelAI->getPathDirection(1, c);
		btVector3 trackVector = levelAI->getVectorToTrack(c);
		btVector3 segmentVec1 = levelAI->getVectorToSeg(1, c);
		btVector3 segmentVec2 = levelAI->getVectorToSeg(2, c);
		aiInputs[c]->step((*itr), pathDir1, pathDir2, trackVector, segmentVec1, segmentVec2);
	}
}

JeepActor* JeepManager::getHuman() {
	return human;
}

JeepManager::Jeeps JeepManager::getAIs() {
	return aiJeeps;
}
