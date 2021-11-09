#ifndef BLACKHOLEFORCEGENERATOR_H_
#define BLACKHOLEFORCEGENERATOR_H_

#include "../../utils/checkML.h"
#include "../../utils/core.hpp"

#include "SphericalForceField.h"

class BlackHoleForceGenerator : public SphericalForceField
{
	// BHole attraction force
	double holeForce;

public:
	BlackHoleForceGenerator(const Vector3& pos, double force, double size)
		: SphericalForceField(pos, { 0, 0, 0, 0 }, size), holeForce(force) {};
	virtual ~BlackHoleForceGenerator() {};

	virtual void updateForce(Particle* particle, float t);
};
#endif
