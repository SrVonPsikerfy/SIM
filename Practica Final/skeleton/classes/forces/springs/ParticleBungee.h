#ifndef PARTICLEBUNGEE_H
#define PARTICLEBUNGEE_H

#include "../../../utils/checkML.h"
#include "../../../utils/core.hpp"

#include "SpringForceGenerator.h"

class ParticleBungee : public SpringForceGenerator
{
private:
	// The other particle the spring is attached to
	Particle* oppParticle;

public:
	ParticleBungee(Particle* opp, float kFactor, float rLength)
		: SpringForceGenerator(kFactor, rLength), oppParticle(opp) {};

	virtual void updateForce(Particle* particle, float t);
};
#endif