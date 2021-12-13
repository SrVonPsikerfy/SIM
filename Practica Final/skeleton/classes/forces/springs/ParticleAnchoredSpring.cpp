#include "ParticleAnchoredSpring.h"

void ParticleAnchoredSpring::updateForce(Particle* particle, float t)
{
	Vector3 f = particle->getPos();
	f -= anchor;

	// Length
	float length = f.normalize();

	// Resulting force
	f *= -((length - restLength) * k);

	particle->addForce(f);
}

void RigidBodyAnchoredSpring::updateForce(RigidBody* rb, float t)
{
	Vector3 f = rb->rigidData->getGlobalPose().p;
	f -= anchor;

	// Length
	float length = f.normalize();

	// Resulting force
	f *= -((length - restLength) * k);

	rb->force += f;
}
