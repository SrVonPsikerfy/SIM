#ifndef PARTICLESPRING_H
#define PARTICLESPRING_H

#include "../../utils/checkML.h"
#include "../../utils/core.hpp"

#include "../forces/ParticleForceGenerator.h"

class ParticleSpring : public ParticleForceGenerator
{
	// The other particle the spring is attached to
	Particle* oppParticle;

	// Elasticity constant
	float k;

	float restLength;

public:
	ParticleSpring(Particle* opp, float kFactor, float rLength) 
		: oppParticle(opp), k(kFactor), restLength(rLength) {};

	virtual void updateForce(Particle* particle, float t);
};
#endif