#ifndef SPRINGFORCEGENERATOR_H
#define SPRINGFORCEGENERATOR_H

#include "../../utils/checkML.h"
#include "../../utils/core.hpp"

#include "../forces/ParticleForceGenerator.h"

class SpringForceGenerator : public ParticleForceGenerator
{
protected:
	// Elasticity constant
	float k;

	float restLength;

public:
	SpringForceGenerator(float kFactor, float rLength)
		: k(kFactor), restLength(rLength) {};

	void addElasticity(float kAdd) { k += kAdd; };
};
#endif