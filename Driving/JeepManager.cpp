#include "Common/Debug.h"
#include "JeepManager.h"
#include "JeepActor.h"
#include "AI/LevelAI.h"
#include "Main/MainController.h"
#include "Physics/Physics.h"
#include "AI/AIInput.h"
#include "UI/Input.h"
#include "Renderer/Models.h"

JeepManager::JeepManager() :
jeepModel( Models::GetModel("jeep") )
{}

void JeepManager::initialize(Physics * physics, Input * playerInput)
{
	float jeepX			= LoadFloat("config/start.xml", "jeepX");
	float jeepY			= LoadFloat("config/start.xml", "jeepY");
	float jeepZ 		= LoadFloat("config/start.xml", "jeepZ");
	float jeepRotation	= LoadFloat("config/start.xml", "jeepRotation");

	btQuaternion rotation(btVector3(0,1,0), jeepRotation);	//initial rotation
	
	human = new JeepActor(mChasis, jeepModel, physics, playerInput, btVector3(jeepX, jeepY, jeepZ), rotation );
	human->isHuman = true;
	human->ID = 10;
	//MainController::addActor(human);
	num_players = LoadInt("config/ai.xml","num_players");
	for (int i = 0; i < num_players; ++i) {
		aiInputs.push_back(new AIInput());
		JeepActor* jeep = new JeepActor(mChasis, jeepModel, physics, aiInputs[i], btVector3(jeepX + (10*i + 10), jeepY, jeepZ), rotation );
		jeep->setOrientation( rotation );
		jeep->ID = i;
		
		aiJeeps.push_back(jeep);
		//MainController::addActor(jeep);
	}

	levelAI = new LevelAI(aiJeeps, human);

	physics->dynamicsWorld.setInternalTickCallback(tickCallback, static_cast<void *>(this), true );
    freezeAt(btVector3(jeepX, jeepY, jeepZ));
    
    physics = physics;
	
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
	for (int i = 0; i < num_players; ++i) {
		aiJeeps[i]->reset(rotation, btVector3(jeepX + (10*i + 10), jeepY, jeepZ) );
		this->aiInputs[i]->restart();
	}
	levelAI->restart();
	
    freezeAt(btVector3(jeepX, jeepY, jeepZ));
	
}

//respawn a single jeep
void JeepManager::restartJeep(JeepActor * jeep)
{
	btVector3 pos = levelAI->getPlayerPosition(jeep->ID);
	float jeepRotation	= LoadFloat("config/start.xml", "jeepRotation");

	btQuaternion rotation(btVector3(0,1,0), jeepRotation);	//initial rotation
	
	jeep->reset( rotation, pos);	
	jeep->release();
}

void JeepManager::release()
{
    human->release();
    for (int i = 0; i < num_players; ++i) {
        aiJeeps[i]->release();
	}
    
}

void JeepManager::freezeAt(btVector3 const & pos)
{
    human->freezeAt(pos);
    int num_players = LoadInt("config/ai.xml","num_players");
    for (int i = 0; i < num_players; ++i) {
        aiJeeps[i]->freezeAt(pos);
	}
}

void JeepManager::hitDetection(btDynamicsWorld * world)
{
    	int numManifolds = world->getDispatcher()->getNumManifolds();
    	for (int i=0;i<numManifolds;i++)
    	{
    		btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
    		btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
    		btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

    		int numContacts = contactManifold->getNumContacts();
    		for (int j=0;j<numContacts;j++)
    		{
    			btManifoldPoint& pt = contactManifold->getContactPoint(j);
                btScalar distance = pt.getDistance();
    			if (distance<0.f)
    			{
                    void * a = obA->getUserPointer();
                    void * b = obB->getUserPointer();
                                            
                    const btVector3& ptA = pt.getPositionWorldOnA();
    				const btVector3& ptB = pt.getPositionWorldOnB();
    				const btVector3& normalOnB = pt.m_normalWorldOnB;
    				
                    btScalar impulse = pt.getAppliedImpulse();
                        				
    				if(a != NULL)
                        static_cast<JeepActor*>(a)->hitObject( impulse, ptA );
                    
                    if(b != NULL)
                        static_cast<JeepActor*>(b)->hitObject( impulse, ptB );
                    LOG("impulse " << pt.getAppliedImpulse(), "hit");
    				
    			}
    		}
    	}
}

void JeepManager::tickCallback(btDynamicsWorld *world, btScalar timeStep)
{
	JeepManager * m = static_cast<JeepManager *>(world->getWorldUserInfo());
    m->hitDetection(world);
    
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
		Jeeps allJeeps = aiJeeps;
		allJeeps.push_back(human);
		aiInputs[c]->step((*itr), allJeeps, pathDir1, pathDir2, trackVector, segmentVec1, segmentVec2);
	}
	
    
}

void JeepManager::startEngines()
{
	human->startEngine();
	for (Jeeps::iterator itr = aiJeeps.begin(); itr != aiJeeps.end(); ++itr) {
		(*itr)->startEngine();
	}
}


JeepActor* JeepManager::getHuman() {
	return human;
}

JeepManager::Jeeps JeepManager::getAIs() {
	return aiJeeps;
}

btVector3 JeepManager::getPlayerPos(int p) const {
	if (p < aiJeeps.size()) return aiJeeps[p]->pos;
	else return human->pos;
}
