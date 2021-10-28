#ifndef WINDFIELDFORCEGENERATOR_H_
#define WINDFIELDFORCEGENERATOR_H_

#include "../utils/checkML.h"
#include "../utils/core.hpp"

#include "ParticleForceGenerator.h"

class WindFieldForceGenerator : public ParticleForceGenerator
{
	// Wind push force
	Vector3 windForce;

	double radius;
	physx::PxTransform pos;
	RenderItem* field;

	bool onField(const Vector3& p);

public:
	WindFieldForceGenerator(const Vector3& force, const Vector3& pos, double size);
	~WindFieldForceGenerator();

	virtual void updateForce(Particle* particle, float t);
};
#endif