#include "ExplosionRigidForceGenerator.h"

void ExplosionRigidForceGenerator::updateForce(RigidBody* rb, float t) {
	if (rb->rigidData->getInvMass() == 0 || !onField(rb->rigidData->getGlobalPose().p))
		return;

	Vector3 diff = rb->rigidData->getGlobalPose().p - fieldPos.p;
	double linealForce = exForce * (1 - (diff.magnitude() / radius));
	auto x = diff.getNormalized() * linealForce;

	rb->force += x;
}

void ExplosionRigidForceGenerator::update(double time) {
	exTime -= time;

	if (exTime <= 0) {
		setActive(false);
		exTime = exMaxTime;
	}
}

void ExplosionRigidForceGenerator::activateExplosion() {
	if (exTime == exMaxTime)
		setActive(true);
}
