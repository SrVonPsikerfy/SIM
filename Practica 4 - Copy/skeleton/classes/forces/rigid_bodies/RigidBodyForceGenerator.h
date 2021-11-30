#ifndef RIGIDBODYFORCEGENERATOR_H
#define RIGIDBODYFORCEGENERATOR_H

#include "../../utils/checkML.h"

#include "../../rigid_body_system/RigidBody.h"

class RigidBodyForceGenerator
{
public:
	// Overload to provide functionality
	virtual void updateForce(RigidBody* particle, float t) = 0;
	virtual void update(double time) {};

	void setActive(bool act) { active = act; };
	bool isActive() { return active; };

protected:
	bool active = true;
};
#endif