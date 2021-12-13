#ifndef DOWNRIGIDFORCEGENERATOR_H_
#define DOWNRIGIDFORCEGENERATOR_H_

#include "../../../utils/checkML.h"
#include "../../../utils/core.hpp"

#include "RigidBodyForceGenerator.h"

#include "../../rigid_body_system/RigidBody.h"

class DownRigidForceGenerator : public RigidBodyForceGenerator
{
private:
	// Explosion push force
	double exForce, exTime, exMaxTime;

public:
	DownRigidForceGenerator(double force, double t)
		:  exForce(force), exMaxTime(t), exTime(t) {};
	virtual ~DownRigidForceGenerator() {};

	virtual void updateForce(RigidBody* rb, float t);
	virtual void update(double time);

	void activateDown();
};
#endif
