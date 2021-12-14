#include "RigidBodySystem.h"

#include "../../utils/maths.h"

void RigidBodySystem::update(double t)
{
	integrate(t);

	if (spawnTime != -1)
		spawnRigidBody(t);
}

RigidBody* RigidBodySystem::addBody(Vector3 offset, float massSett, Vector3 velSet, Vector3 rotVelSet, 
	float dampingSet, float dampAngSet, Vector3 tensorSet, float sizeSet, float lifeSet, Vector4 colorSet, RBType type)
{
	if (rBodies.size() == maxParticles)
		return nullptr;

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
	rigid->setLinearVelocity(velSet);				rigid->setLinearDamping(dampingSet);
	rigid->setAngularVelocity(rotVelSet);			rigid->setAngularDamping(dampAngSet);

	//Dinámica 
	//PxRigidBodyExt::updateMassAndInertia(*rigid, 1);
	rigid->setMass(massSett);
	rigid->setMassSpaceInertiaTensor(tensorSet);

	gScene->addActor(*rigid);

	// complete body struct 
	rb->rigidData = rigid;
	rb->deathTime = lifeSet; rb->lifetime = 0;
	rb->force = { 0.0f, 0.0f, 0.0f };
	rb->torque = { 0.0f, 0.0f, 0.0f };
	rb->rItem = new RenderItem(shape, rigid, colorSet);

	rBodies.push_back(rb);

	addForceLinks();

	return rb;
}

void RigidBodySystem::reset() {
	for (int i = 0; i < rBodies.size(); i++) {
		releaseRigidBody(i);
		--i;
	}
	rBodies.clear();

	spType = SpawnRBType::NONE;
	spawnTime = -1;
}

void RigidBodySystem::setSpawn(double spawn, SpawnRBType type)
{
	spType = type;
	spawnTime = spawn;
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
		spawn(t);
		nextSpawn = 0;
	}
}

void RigidBodySystem::spawn(double t) {
	nextSpawn += t;
	if (nextSpawn >= spawnTime) {
		switch (spType) {
		case SpawnRBType::SNOW: {
			Vector3 pos = { maths::random<float>(-10, 10), -1, maths::random<float>(-10, 10) };
			double size = maths::random<double>(1, 3);
			double mass = maths::random<double>(5, 10);
			Vector3 vel = { 0, 0, 0 }, rotVel = { 0, 0, 0 };
			float damp = 0.8, dampAng = 0.01, life = maths::random<double>(3, 5);
			Vector3 tensor = { 0, 1, 1 };
			Vector4 color = { 1, 1, 1, 1.0 }; RBType type = RBType::CUBE;

			addBody(pos, mass, vel, rotVel, damp, dampAng, tensor, size, life, color, type);
			break;
		}
		case SpawnRBType::NONE:
			addBody();
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