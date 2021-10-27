#ifndef DRAGFORCEGENERATOR_H_
#define DRAGFORCEGENERATOR_H_

#include "../utils/checkML.h"
#include "../utils/core.hpp"

#include "ParticleForceGenerator.h"

class DragForceGenerator : public ParticleForceGenerator
{
	// Coefficient for velocity
	float k1;
	// Coefficient for squared velocity
	float k2;
public:
	DragForceGenerator(float _k1, float _k2) : k1(_k1), k2(_k2) {}
	virtual void updateForce(Particle* particle, float t);
};
#endif