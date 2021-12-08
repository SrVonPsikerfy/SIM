#ifndef PARTICLEBUOYANCY_H
#define PARTICLEBUOYANCY_H

#include "../../../utils/checkML.h"
#include "../../../utils/core.hpp"

#include "../particles/ParticleForceGenerator.h"

class ParticleBuoyancy : public ParticleForceGenerator
{
private:
	float maxDepth;

	float volume;

	// Considering a water plane parallel to XZ
	float waterHeight;

	// Density of the liquid the object is submerged in
	float liquidDensity;

	float gFactor = 9.8;

	physx::PxTransform waterCeilingPos;
	RenderItem* waterCeiling;

public:
	ParticleBuoyancy(Vector3 pos, float _maxDepth, float _volume, float _waterHeight, float _liquidDensity = 1000.0f);

	virtual void updateForce(Particle* particle, float t);
};
#endif