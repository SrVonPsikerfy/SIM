#include "RigidBodySystem.h"

void RigidBodySystem::update(double t)
{
	integrate(t);

	if (spawnTime != -1)
		spawnRigidBody(t);
}

void RigidBodySystem::addBody(Vector3 offset, float massSett, float sizeSet, float lifeSet, bool colorR, Vector4 colorSet, RBType type)
{
	if (rBodies.size() == maxParticles)
		return;

	RigidBody* rb = new RigidBody;

	//rigid 
	PxTransform originRB(origin.p + offset);
	PxRigidDynamic* rigid = gPhysics->createRigidDynamic(originRB);

	// shape 
	PxShape* shape;
	if (type == RBType::CUBE) shape = CreateShape(PxBoxGeometry(sizeSet, sizeSet, sizeSet));
	else if (type == RBType::SPHERE) shape = CreateShape(PxSphereGeometry(sizeSet));
	else shape = CreateShape(PxCapsuleGeometry(sizeSet, sizeSet));
	rigid->attachShape(*shape);

	//Cinetica 
	Vector3 vel = { -5.0f + rand() / (RAND_MAX / (10.0f)), -5.0f + rand() / (RAND_MAX / (10.0f)),
		-5.0f + rand() / (RAND_MAX / (10.0f)) };
	rigid->setLinearVelocity(vel);				rigid->setLinearDamping(0.1);
	rigid->setAngularVelocity({ 0,2,0 });		rigid->setAngularDamping(0.05);

	//Dinámica 
	//PxRigidBodyExt::updateMassAndInertia(*rigid, 1);
	rigid->setMass(massSett);
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

	addForceLinks();
}

void RigidBodySystem::addForceGenerator(RigidBodyForceGenerator* fg) {
	fgs.push_back(fg);
}

void RigidBodySystem::applyForceGenerator(RigidBodyForceGenerator* fg) {
	for (int i = 0; i < rBodies.size(); i++)
		fReg->add(rBodies[i], fg);
}

void RigidBodySystem::integrate(double t)
{
	for (size_t i = 0; i < rBodies.size(); i++) {
		rBodies[i]->rigidData->addForce(rBodies[i]->force, PxForceMode::eFORCE);
		rBodies[i]->force = { 0.0f, 0.0f, 0.0f };

		rBodies[i]->rigidData->addTorque(rBodies[i]->torque, PxForceMode::eFORCE);
		rBodies[i]->torque = { 0.0f, 0.0f, 0.0f };

		rBodies[i]->lifetime += t;

		if (rBodies[i]->deathTime != -1 && rBodies[i]->lifetime > rBodies[i]->deathTime)
			releaseRigidBody(i);
	}
}

void RigidBodySystem::spawnRigidBody(double t)
{
	nextSpawn += t;
	if (nextSpawn > spawnTime) {
		spawn();
		nextSpawn = 0;
	}
}

void RigidBodySystem::spawn(double t) {
	nextSpawn += t;
	if (nextSpawn >= spawnTime) {
		switch (spType) {
		case SpawnType::FOUNTAIN:
			generateFountainParticle();
			break;
		case SpawnType::ERUPTION:
			generateEruptionParticle();
			break;
		case SpawnType::RIVER:
			generateRiverParticle();
			break;
		case SpawnType::NONE:
			nextSpawn = 0;
			break;
		}
	}
}

void RigidBodySystem::free()
{
	for (size_t i = 0; i < rBodies.size(); i++)
		releaseRigidBody(i);
}

void RigidBodySystem::releaseRigidBody(int numRB) {
	fReg->clearRigidBodyLinks(rBodies[numRB]);

	gScene->removeActor(*(rBodies[numRB]->rigidData));
	rBodies[numRB]->rItem->release();

	delete rBodies[numRB];
	rBodies.erase(rBodies.begin() + numRB);
}

void RigidBodySystem::addForceLinks() {
	for (auto fg : fgs)
		fReg->add(rBodies[rBodies.size() - 1], fg);
}