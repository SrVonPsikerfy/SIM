#include "BlackHoleForceGenerator.h"

void BlackHoleForceGenerator::updateForce(Particle* particle, float t) {
	if (particle->hasFiniteMass() || !onField(particle->getPos())) 
		return;

	Vector3 diff = fieldPos.p - particle->getPos();
	double linealForce = holeForce * (1 - (diff.magnitude() / radius));
	auto x = diff.getNormalized() * linealForce;

	particle->addForce(x);
}