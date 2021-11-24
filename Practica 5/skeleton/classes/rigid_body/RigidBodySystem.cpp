#include "RigidBodySystem.h"

RigidBodySystem::~RigidBodySystem()
{
	for (size_t i = 0; i < rBodies.size(); i++)
		delete rBodies[i];
}

void RigidBodySystem::addBody()
{
}

void RigidBodySystem::integrate(double t)
{
	if (spawnTime != -1) {
		nextSpawn += t;
		if (nextSpawn > spawnTime)
			addBody();
	}

	for (size_t i = 0; i < rBodies.size(); i++) {
		rBodies[i]->rigidData->addForce(rBodies[i]->force, PxForceMode::eFORCE);
		rBodies[i]->force = { 0.0f, 0.0f, 0.0f };

		rBodies[i]->rigidData->addTorque(rBodies[i]->torque, PxForceMode::eFORCE);
		rBodies[i]->torque = { 0.0f, 0.0f, 0.0f };

		rBodies[i]->lifetime += t;

		if (rBodies[i]->lifetime > rBodies[i]->deathTime)
			releaseRigidBody(i);
	}

}

void RigidBodySystem::releaseRigidBody(int numRB) {
	//fReg->clearParticleLinks(rBodies[numRB]);

	gScene->removeActor(*(rBodies[numRB]->rigidData));
	rBodies[numRB]->rItem->release();

	delete rBodies[numRB];
	rBodies.erase(rBodies.begin() + numRB);
}
