#ifndef TORQUERIGIDFORCEGENERATOR_H_
#define TORQUERIGIDFORCEGENERATOR_H_

#include "../../../utils/checkML.h"
#include "../../../utils/core.hpp"

#include "RigidBodyForceGenerator.h"
#include "../SphericalForceField.h"

#include "../../rigid_body_system/RigidBody.h"

class TorqueRigidForceGenerator : public RigidBodyForceGenerator
{
private:
	Vector3 torque;

public:
	TorqueRigidForceGenerator(const Vector3& force)
		: torque(force) {};
	virtual ~TorqueRigidForceGenerator() {};

	virtual void updateForce(RigidBody* rb, float t);
};
#endif