#ifndef PARTICLEBUNGEE_H
#define PARTICLEBUNGEE_H

#include "../../utils/checkML.h"
#include "../../utils/core.hpp"

#include "../forces/ParticleForceGenerator.h"

class ParticleBuoyancy : public ParticleForceGenerator
{
	float maxDepth;

	float volume;

	// Considering a water plane parallel to XZ
	float waterHeight;

	// Density of the liquid the object is submerged in
	float liquidDensity;

public:
	ParticleBuoyancy(float _maxDepth, float _volume, float _waterHeight, float _liquidDensity = 1000.0f) :
		maxDepth(_maxDepth), volume(_volume), waterHeight(_waterHeight), liquidDensity(_liquidDensity) {};

	virtual void updateForce(Particle* particle, float t);
};
#endif