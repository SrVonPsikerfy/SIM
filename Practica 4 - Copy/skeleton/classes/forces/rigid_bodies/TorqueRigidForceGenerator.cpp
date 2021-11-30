#include "TorqueRigidForceGenerator.h"

void TorqueRigidForceGenerator::updateForce(RigidBody* rb, float t) {
	if (rb->rigidData->getInvMass() == 0 || !onField(rb->rigidData->getGlobalPose().p))
		return;

	rb->torque += torque;
}