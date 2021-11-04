#ifndef GRAVITYFORCEGENERATOR_H_
#define GRAVITYFORCEGENERATOR_H_

#include "../utils/checkML.h"
#include "../utils/core.hpp"

#include "ParticleForceGenerator.h"

class GravityForceGenerator : public ParticleForceGenerator
{
	// Acceleration for gravity
	Vector3 g;

public:
	GravityForceGenerator(const Vector3& gravity) : g(gravity) {}
	~GravityForceGenerator() {};

	virtual void updateForce(Particle* particle, float t);
};
#endif