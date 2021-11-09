#include "WindFieldForceGenerator.h"

void WindFieldForceGenerator::updateForce(Particle* particle, float t) {
	if (particle->hasFiniteMass() || !onField(particle->getPos()))
		return;

	particle->addForce(windForce);
}