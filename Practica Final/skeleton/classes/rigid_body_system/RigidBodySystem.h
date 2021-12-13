#ifndef RIGIDBODYSYSTEM_H
#define RIGIDBODYSYSTEM_H

#include <vector>

#include "RigidBody.h"

#include "../forces/ForceRegistry.h"

#include "../forces/rigid_bodies/RigidBodyForceGenerator.h"

enum class SpawnType { NONE, SNOW };

class RigidBodySystem
{
public:
	RigidBodySystem(ForceRegistry* fR, PxPhysics* gPhys, PxScene* gSc, PxTransform pxSet, int maxP = 10, double spT = -1,
			SpawnType type = SpawnType::NONE)
		: fReg(fR), gPhysics(gPhys), gScene(gSc), origin(pxSet), maxParticles(maxP), spawnTime(spT) {};
	~RigidBodySystem() { free(); };

	void update(double t);
	void addBody(Vector3 offset = { 0, 0, 0 }, float massSett = 5, float sizeSet = 1, float lifeSet = 10,
		bool colorR = true, Vector4 colorset = { 1, 0, 0, 1.0 }, RBType type = RBType::CUBE);

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
	SpawnType spType;

	int maxParticles;
	double spawnTime, nextSpawn = 0;
};
#endif