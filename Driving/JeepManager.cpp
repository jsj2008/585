#include "JeepManager.h"
#include "JeepActor.h"

JeepManager::JeepManager(Physics * physics, ActorList * actorList, Input * playerInput) :
jeepModel("blank.bmp", "blank.bmp", "models/jeep2_flipx.obj")
{
	float const & planeY = LoadFloat("config/start.xml", "planeY");
	float const & jeepX = LoadFloat("config/start.xml", "jeepX");
	float const & jeepY = LoadFloat("config/start.xml", "jeepY");
	float const & jeepZ = LoadFloat("config/start.xml", "jeepZ");

	/*pass jeep into physics/renderer but don't add to dynamicWorld (this is done by jeep internally)*/
	human = new JeepActor(mChasis, jeepModel, physics, playerInput, btVector3(jeepX, jeepY, jeepZ));
	actorList->push_back(human);
	for (int i = 0; i < LoadInt("config/ai.xml","num_players"); ++i) {
		aiInputs.push_back(new AIInput());
		JeepActor* jeep = new JeepActor(mChasis, jeepModel, physics, aiInputs[i], btVector3(jeepX, jeepY, jeepZ+(20*i+20)));
		
		aiJeeps.push_back(jeep);
		actorList->push_back(jeep);
	}

	levelAI = new LevelAI(aiJeeps, human);

	physics->dynamicsWorld.setInternalTickCallback(tickCallback, static_cast<void *>(this), true );
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
