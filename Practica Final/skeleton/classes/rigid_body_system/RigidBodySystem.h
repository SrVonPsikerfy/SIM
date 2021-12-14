#ifndef RIGIDBODYSYSTEM_H
#define RIGIDBODYSYSTEM_H

#include <vector>

#include "RigidBody.h"

#include "../forces/ForceRegistry.h"

#include "../forces/rigid_bodies/RigidBodyForceGenerator.h"

enum class SpawnRBType { NONE, SNOW };

class RigidBodySystem
{
public:
	RigidBodySystem(ForceRegistry* fR, PxPhysics* gPhys, PxScene* gSc, PxTransform pxSet, int maxP = 10, double spT = -1,
			SpawnRBType type = SpawnRBType::NONE)
		: fReg(fR), gPhysics(gPhys), gScene(gSc), origin(pxSet), maxParticles(maxP), spawnTime(spT) {};
	~RigidBodySystem() { reset(); };

	void update(double t);
	RigidBody* addBody(Vector3 offset = { 0, 0, 0 }, float massSett = 5, Vector3 velSet = { 0, 0, 0 }, Vector3 rotVelSet = { 0, 0, 0 },
		float dampingSet = 0.1, float dampAngSet = 0.1, Vector3 tensorSet = { 1, 1, 1 }, float sizeSet = 1, float lifeSet = 10,
		Vector4 colorSet = { 1, 0, 0, 1.0 }, RBType type = RBType::CUBE);
	void reset();

	void setSpawn(double spawn, SpawnRBType type);

	void addForceGenerator(RigidBodyForceGenerator* fg);
	void applyForceGenerator(RigidBodyForceGenerator* fg);
	 
private:
	void integrate(double t);
	void spawnRigidBody(double t);
	void spawn(double t);

	void free();
	void releaseRigidBody(int numRB);

	void addForceLinks();


	vector<RigidBody*> rBodies;
	vector<RigidBodyForceGenerator*> fgs;
	ForceRegistry* fReg;

	PxPhysics* gPhysics;
	PxScene* gScene;

	PxTransform origin;
	SpawnRBType spType;

	int maxParticles;
	double spawnTime, nextSpawn = 0;
};
#endif