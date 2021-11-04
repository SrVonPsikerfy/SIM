#ifndef BLACKHOLEFORCEGENERATOR_H_
#define BLACKHOLEFORCEGENERATOR_H_

#include "../utils/checkML.h"
#include "../utils/core.hpp"

#include "ParticleForceGenerator.h"

class BlackHoleForceGenerator : public ParticleForceGenerator
{
	// Wind push force
	double holeForce;
	double radius;

	physx::PxTransform pos;
	RenderItem* field;

	bool onField(const Vector3& p);

public:
	BlackHoleForceGenerator(const Vector3& pos_, double force, double size);
	~BlackHoleForceGenerator();

	virtual void updateForce(Particle* particle, float t);
};
#endif
