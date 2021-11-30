#include "ParticleSpring.h"

void ParticleSpring::updateForce(Particle* particle, float t)
{
	// Calculate distance vector
	Vector3 f = particle->getPos();
	f -= oppParticle->getPos();

	// Length
	float length = f.normalize();

	// Resulting force
	float deltaL = (length - restLength);
	float forceMagnitude = -k * deltaL;
	f *= forceMagnitude;

	particle->addForce(f);
}