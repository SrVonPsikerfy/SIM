#ifndef RIGIDBODYSYSTEM_H
#define RIGIDBODYSYSTEM_H

#include <vector>

#include "RigidBody.h"

#include "../forces/ForceRegistry.h"

#include "../forces/rigid_bodies/RigidBodyForceGenerator.h"

class RigidBodySystem
{
public:
	RigidBodySystem(ForceRegistry* fR, PxPhysics* gPhys, PxScene* gSc, PxTransform pxSet, int maxP = 10, double spT = -1)
		: fReg(fR), gPhysics(gPhys), gScene(gSc), origin(pxSet), maxParticles(maxP), spawnTime(spT) {};
	~RigidBodySystem() { free(); };

	void update(double t);
	void addBody(Vector3 offset = { 0, 0, 0 }, float sizeSet = 1, float lifeSet = 5,
		bool colorR = true, Vector4 colorset = { 1, 0, 0, 1.0 });

	void addForceGenerator(RigidBodyForceGenerator* fg);
	void applyForceGenerator(RigidBodyForceGenerator* fg);
	 
private:
	void integrate(double t);
	void spawnRigidBody(double t);

	void free();
	void releaseRigidBody(int numRB);

	void addForceLinks();

	vector<RigidBody*> rBodies;
	vector<RigidBodyForceGenerator*> fgs;
	ForceRegistry* fReg;

	PxPhysics* gPhysics;
	PxScene* gScene;

	PxTransform origin;

	int maxParticles;
	double spawnTime, nextSpawn = 0;
};
#endif