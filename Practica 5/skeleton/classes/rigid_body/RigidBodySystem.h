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
	RigidBodySystem(PxPhysics* gPhys, PxScene* gSc, double spT = -1) : gPhysics(gPhys), gScene(gSc), spawnTime(spT) {};
	~RigidBodySystem();

	void addBody();
	void integrate(double t);

private:
	void releaseRigidBody(int numRB);

	vector<RigidBody*> rBodies;

	PxPhysics* gPhysics;
	PxScene* gScene;

	double spawnTime, nextSpawn = 0;
};

