#ifndef WINDRIGIDBODYFORCEGENERATOR_H_
#define WINDRIGIDBODYFORCEGENERATOR_H_

#include "../../../utils/checkML.h"
#include "../../../utils/core.hpp"

#include "RigidBodyForceGenerator.h"
#include "../SphericalForceField.h"

#include "../../rigid_body_system/RigidBody.h"

class WindRigidForceGenerator : public RigidBodyForceGenerator, SphericalForceField
{
private:
	// Wind push force
	Vector3 windForce;

public:
	WindRigidForceGenerator(const Vector3& pos, const Vector3& force, double size, double time = -1)
		: SphericalForceField(pos, { 0, 0, 1, 0 }, size), windForce(force) {};
	virtual ~WindRigidForceGenerator() {};

	virtual void updateForce(RigidBody* particle, float t);
};
#endif