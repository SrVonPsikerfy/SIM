#include "DownRigidForceGenerator.h"

void DownRigidForceGenerator::updateForce(RigidBody* rb, float t)
{
	if (rb->rigidData->getInvMass() == 0)
		return;
	
	auto x = Vector3(0, -1, 0) * exForce;

	rb->force += x;
}

void DownRigidForceGenerator::update(double time) {
	exTime -= time;

	if (exTime <= 0) {
		setActive(false);
		exTime = exMaxTime;
	}
}

void DownRigidForceGenerator::activateDown() {
	if (exTime == exMaxTime)
		setActive(true);
}