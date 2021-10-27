#include "GravityForceGenerator.h"

void GravityForceGenerator::updateForce(Particle* particle, float t)
{
	if (!particle->hasFiniteMass()) return;
	particle->addForce(g * particle->getMass());
}