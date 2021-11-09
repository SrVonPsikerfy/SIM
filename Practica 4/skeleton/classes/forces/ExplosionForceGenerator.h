#ifndef EXPLOSIONFORCEGENERATOR_H_
#define EXPLOSIONFORCEGENERATOR_H_

#include "../../utils/checkML.h"
#include "../../utils/core.hpp"

#include "SphericalForceField.h"

class ExplosionForceGenerator : public SphericalForceField
{
	// Explosion push force
	double exForce;
	
public:
	ExplosionForceGenerator(const Vector3& pos, double force, double size)
		: SphericalForceField(pos, { 1, 0, 1, 0 }, size), exForce(force) {};
	virtual ~ExplosionForceGenerator() {};

	virtual void updateForce(Particle* particle, float t);
};
#endif