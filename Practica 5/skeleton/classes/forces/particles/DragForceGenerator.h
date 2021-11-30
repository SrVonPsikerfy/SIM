#ifndef DRAGFORCEGENERATOR_H_
#define DRAGFORCEGENERATOR_H_

#include "../../utils/checkML.h"
#include "../../utils/core.hpp"

#include "ParticleForceGenerator.h"

#include "../../particle_system/Particle.h"

class DragForceGenerator : public ParticleForceGenerator
{
private:
	float k1, k2;

public:
	DragForceGenerator(float _k1, float _k2) : k1(_k1), k2(_k2) {}
	virtual ~DragForceGenerator() {};

	virtual void updateForce(Particle* particle, float t);
};
#endif