#include "TorqueRigidForceGenerator.h"

void TorqueRigidForceGenerator::updateForce(RigidBody* rb, float t) {
	if (rb->rigidData->getInvMass() == 0)
		return;

	rb->torque += torque;
}