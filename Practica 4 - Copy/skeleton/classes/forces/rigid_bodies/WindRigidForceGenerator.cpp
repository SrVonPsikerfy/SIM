#include "WindRigidForceGenerator.h"

#include "../../utils/maths.h"
#include <iostream>

void WindRigidForceGenerator::updateForce(RigidBody* rb, float t) {
	if (rb->rigidData->getInvMass() == 0|| !onField(rb->rigidData->getGlobalPose().p))
		return;

	rb->force += windForce;
}