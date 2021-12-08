#ifndef EXPLOSIONFORCEGENERATOR_H_
#define EXPLOSIONFORCEGENERATOR_H_

#include "../../../utils/checkML.h"
#include "../../../utils/core.hpp"

#include "ParticleForceGenerator.h"
#include "../SphericalForceField.h"

#include "../../particle_system/Particle.h"

class ExplosionForceGenerator : public ParticleForceGenerator, SphericalForceField
{
private:
	// Explosion push force
	double exForce, exTime, exMaxTime;
	
public:
	ExplosionForceGenerator(const Vector3& pos, double force, double size, double t)
		: SphericalForceField(pos, { 1, 0, 1, 0 }, size), exForce(force), exMaxTime(t), exTime(t) {};
	virtual ~ExplosionForceGenerator() {};

	virtual void updateForce(Particle* particle, float t);
	virtual void update(double time);

	void activateExplosion();
};
#endif