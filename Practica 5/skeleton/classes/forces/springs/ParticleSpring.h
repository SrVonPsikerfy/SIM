#ifndef PARTICLESPRING_H
#define PARTICLESPRING_H

#include "../../../utils/checkML.h"
#include "../../../utils/core.hpp"

#include "SpringForceGenerator.h"

class ParticleSpring : public SpringForceGenerator
{
private:
	// The other particle the spring is attached to
	Particle* oppParticle;

public:
	ParticleSpring(Particle* opp, float kFactor, float rLength) 
		: SpringForceGenerator(kFactor, rLength), oppParticle(opp) {};

	virtual void updateForce(Particle* particle, float t);
};
#endif