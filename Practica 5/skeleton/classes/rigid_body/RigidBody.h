#pragma once

#include "../../render/RenderUtils.hpp"

using namespace physx;
using namespace std;


struct RigidBody {
	PxRigidDynamic* rigidData;

	RenderItem* rItem;

	double deathTime, lifetime;

	PxVec3 force, torque;
}; 