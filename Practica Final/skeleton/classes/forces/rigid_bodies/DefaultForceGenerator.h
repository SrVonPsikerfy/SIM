#ifndef DEFAULTFORCEGENERATOR_H_
#define DEFAULTFORCEGENERATOR_H_

#include "../../../utils/checkML.h"
#include "../../../utils/core.hpp"

#include "RigidBodyForceGenerator.h"

#include "../../rigid_body_system/RigidBody.h"

class DefaultForceGenerator : public RigidBodyForceGenerator
{
private:
	// Wind push force
	Vector3 f;
	double coolDown, timeThreshold;

public:
	DefaultForceGenerator(double time = -1)
		: coolDown(time), timeThreshold(0), f(Vector3(0, 0, 0)) {};
	virtual ~DefaultForceGenerator() {};

	virtual void updateForce(RigidBody* rb, float t);

	void addForce(Vector3 force);
};
#endif