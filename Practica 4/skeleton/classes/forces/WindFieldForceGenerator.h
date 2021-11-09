#ifndef WINDFIELDFORCEGENERATOR_H_
#define WINDFIELDFORCEGENERATOR_H_

#include "../../utils/checkML.h"
#include "../../utils/core.hpp"

#include "SphericalForceField.h"

class WindFieldForceGenerator : public SphericalForceField
{
	// Wind push force
	Vector3 windForce;

public:
	WindFieldForceGenerator(const Vector3& pos, const Vector3& force, double size)
		: SphericalForceField(pos, { 0, 0, 1, 0 }, size), windForce(force) {};
	virtual ~WindFieldForceGenerator() {};

	virtual void updateForce(Particle* particle, float t);
};
#endif