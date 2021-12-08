#ifndef TORQUERIGIDFORCEGENERATOR_H_
#define TORQUERIGIDFORCEGENERATOR_H_

#include "../../../utils/checkML.h"
#include "../../../utils/core.hpp"

#include "RigidBodyForceGenerator.h"
#include "../SphericalForceField.h"

#include "../../rigid_body_system/RigidBody.h"

class TorqueRigidForceGenerator : public RigidBodyForceGenerator, SphericalForceField
{
private:
	Vector3 torque;

public:
	TorqueRigidForceGenerator(const Vector3& pos, const Vector3& force, double size, double time = -1)
		: SphericalForceField(pos, { 0, 1, 1, 0 }, size), torque(force) {};
	virtual ~TorqueRigidForceGenerator() {};

	virtual void updateForce(RigidBody* rb, float t);
};
#endif