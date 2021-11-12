#ifndef PARTICLEANCHOREDSPRING_H
#define PARTICLEANCHOREDSPRING_H

#include "../../utils/checkML.h"
#include "../../utils/core.hpp"

#include "../springs/SpringForceGenerator.h"

class ParticleAnchoredSpring : public SpringForceGenerator
{
	// Position of the other end of the spring 
	Vector3 anchor;

public:
	ParticleAnchoredSpring(Vector3 anch, float kFactor, float rLength)
		: SpringForceGenerator(kFactor, rLength), anchor(anch) {};

	virtual void updateForce(Particle* particle, float t);
};
#endif