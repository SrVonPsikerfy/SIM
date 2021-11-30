#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../../render/RenderUtils.hpp"

using namespace physx;
using namespace std;

struct RigidBody {
	PxRigidDynamic* rigidData;

	RenderItem* rItem;

	double deathTime, lifetime;

	PxVec3 force, torque;
};

struct StaticBody {
	PxRigidStatic* staticData;

	RenderItem* rItem;
};
#endif