#ifndef PARTICLEANCHOREDSPRING_H
#define PARTICLEANCHOREDSPRING_H

#include "../../utils/checkML.h"
#include "../../utils/core.hpp"

#include "../forces/ParticleForceGenerator.h"

class ParticleAnchoredSpring : public ParticleForceGenerator
{
	// Position of the other end of the spring 
	Vector3* anchor;

	// Elasticity constant 
	float k;

	float restLength;

public:
	ParticleAnchoredSpring(Vector3* anch, float kFactor, float rLength)
		: anchor(anch), k(kFactor), restLength(rLength) {};

	virtual void updateForce(Particle* particle, float t);
};
#endif