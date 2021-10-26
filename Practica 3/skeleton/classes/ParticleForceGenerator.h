#ifndef PARTICLEFORCEGENERATOR_H
#define PARTICLEFORCEGENERATOR_H

#include "../utils/checkML.h"

#include "Particle.h"

class ParticleForceGenerator
{
public:
	// Overload to provide functionality
	virtual void updateForce(Particle* particle, float t) = 0;
};
#endif