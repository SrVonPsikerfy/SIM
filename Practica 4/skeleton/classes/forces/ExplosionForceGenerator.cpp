#include "ExplosionForceGenerator.h"

void ExplosionForceGenerator::updateForce(Particle* particle, float t) {
	if (particle->hasFiniteMass() || !onField(particle->getPos())) 
		return;

	Vector3 diff = particle->getPos() - fieldPos.p;
	double linealForce = exForce * (1 - (diff.magnitude() / radius));
	auto x = diff.getNormalized() * linealForce;

	particle->addForce(x);
}

