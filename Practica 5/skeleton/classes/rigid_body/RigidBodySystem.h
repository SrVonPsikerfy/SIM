#pragma once

#include <vector>

#include "../../render/RenderUtils.hpp"

using namespace physx;
using namespace std;

struct RigidBody {
	PxRigidDynamic* rigidData;

	RenderItem* rItem;

	double deathTime, lifetime;

	PxVec3 force, torque;
};

class RigidBodySystem
{
public:
	RigidBodySystem(PxPhysics* gPhys, PxScene* gSc, PxTransform pxSet, int maxP = 10, double spT = -1)
		: gPhysics(gPhys), gScene(gSc), origin(pxSet), maxParticles(maxP), spawnTime(spT) {};
	~RigidBodySystem();

	void update(double t);
	void addBody(Vector3 offset = { 0, 0, 0 }, float sizeSet = 1, float lifeSet = 5, bool colorR = true, Vector4 colorset = { 1, 0, 0, 1.0 });
	 
private:
	void integrate(double t);
	void spawnRigidBody(double t);

	void free();
	void releaseRigidBody(int numRB);

	vector<RigidBody*> rBodies;

	PxPhysics* gPhysics;
	PxScene* gScene;

	PxTransform origin;

	int maxParticles;
	double spawnTime, nextSpawn = 0;
};

