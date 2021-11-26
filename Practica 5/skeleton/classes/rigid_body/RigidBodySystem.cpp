#include "RigidBodySystem.h"

RigidBodySystem::~RigidBodySystem()
{
	free();
}

void RigidBodySystem::update(double t)
{
	integrate(t);

	if (spawnTime != -1)
		spawnRigidBody(t);
}

void RigidBodySystem::addBody(Vector3 offset, float sizeSet, float lifeSet, bool colorR, Vector4 colorSet)
{
	if (rBodies.size() == maxParticles)
		return;

	RigidBody* rb = new RigidBody;

	//rigid 
	PxTransform originRB(origin.p + offset);
	PxRigidDynamic* rigid = gPhysics->createRigidDynamic(originRB);

	// shape 
	PxShape* shape = CreateShape(PxBoxGeometry(sizeSet, sizeSet, sizeSet));
	rigid->attachShape(*shape);

	//Cinetica 
	Vector3 vel = { -5.0f + rand() / (RAND_MAX / (10.0f)), -5.0f + rand() / (RAND_MAX / (10.0f)), -5.0f + rand() / (RAND_MAX / (10.0f)) };
	rigid->setLinearVelocity(vel);				rigid->setLinearDamping(0.0);
	rigid->setAngularVelocity({ 0,2,0 });		rigid->setAngularDamping(0.05);

	//Dinámica 
	//PxRigidBodyExt::updateMassAndInertia(*rigid, 1);
	rigid->setMass(4);
	rigid->setMassSpaceInertiaTensor(PxVec3(0.f, 0.f, 1.f));



	gScene->addActor(*rigid);

	// rColor
	if (colorR) colorSet = { float(rand() % 10 / 10.0f), float(rand() % 10 / 10.0f), float(rand() % 10 / 10.0f), 1.0 };

	// complete body struct 
	rb->rigidData = rigid;
	rb->deathTime = lifeSet; rb->lifetime = 0;
	rb->force = { 0.0f, 0.0f, 0.0f };
	rb->torque = { 0.0f, 0.0f, 0.0f };
	rb->rItem = new RenderItem(shape, rigid, colorSet);

	rBodies.push_back(rb);
}

void RigidBodySystem::integrate(double t)
{
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

void RigidBodySystem::spawnRigidBody(double t)
{
	nextSpawn += t;
	if (nextSpawn > spawnTime) {
		addBody();
		nextSpawn = 0;
	}
}

void RigidBodySystem::free()
{
	for (size_t i = 0; i < rBodies.size(); i++)
		releaseRigidBody(i);
}

void RigidBodySystem::releaseRigidBody(int numRB) {
	//fReg->clearParticleLinks(rBodies[numRB]);

	gScene->removeActor(*(rBodies[numRB]->rigidData));
	rBodies[numRB]->rItem->release();

	delete rBodies[numRB];
	rBodies.erase(rBodies.begin() + numRB);
}
