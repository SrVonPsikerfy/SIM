#ifndef EXPLOSIONRIGIDFORCEGENERATOR_H_
#define EXPLOSIONRIGIDFORCEGENERATOR_H_

#include "../../utils/checkML.h"
#include "../../utils/core.hpp"

#include "RigidBodyForceGenerator.h"
#include "../SphericalForceField.h"

#include "../../rigid_body_system/RigidBody.h"

class ExplosionRigidForceGenerator : public RigidBodyForceGenerator, SphericalForceField
{
private:
	// Explosion push force
	double exForce, exTime, exMaxTime;
	
public:
	ExplosionRigidForceGenerator(const Vector3& pos, double force, double size, double t)
		: SphericalForceField(pos, { 1, 0, 1, 0 }, size), exForce(force), exMaxTime(t), exTime(t) {};
	virtual ~ExplosionRigidForceGenerator() {};

	virtual void updateForce(RigidBody* rb, float t);
	virtual void update(double time);

	void activateExplosion();
};
#endif