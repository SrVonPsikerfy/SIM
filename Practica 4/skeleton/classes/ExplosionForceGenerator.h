#ifndef EXPLOSIONFORCEGENERATOR_H_
#define EXPLOSIONFORCEGENERATOR_H_

#include "../utils/checkML.h"
#include "../utils/core.hpp"

#include "ParticleForceGenerator.h"

class ExplosionForceGenerator : public ParticleForceGenerator
{
	// Wind push force
	double exForce;
	double radius;
	
	physx::PxTransform pos;
	RenderItem* field;

	bool onField(const Vector3& p);

public:
	ExplosionForceGenerator(const Vector3& pos, double force, double size);
	~ExplosionForceGenerator();

	virtual void updateForce(Particle* particle, float t);
};
#endif