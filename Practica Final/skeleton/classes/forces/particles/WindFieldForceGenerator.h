#ifndef WINDFIELDFORCEGENERATOR_H_
#define WINDFIELDFORCEGENERATOR_H_

#include "../../../utils/checkML.h"
#include "../../../utils/core.hpp"

#include "ParticleForceGenerator.h"
#include "../SphericalForceField.h"

#include "../../particle_system/Particle.h"

class WindFieldForceGenerator : public ParticleForceGenerator, SphericalForceField
{
private:
	// Wind push force
	Vector3 windForce;
	double changeWindDir, timeThreshold;

public:
	WindFieldForceGenerator(const Vector3& pos, const Vector3& force, double size, double time = -1)
		: SphericalForceField(pos, { 0, 0, 1, 0 }, size), windForce(force), changeWindDir(time), timeThreshold(time) {};
	virtual ~WindFieldForceGenerator() {};

	virtual void updateForce(Particle* particle, float t);
	virtual void update(double t);

	void switchWindType(double t);
};
#endif