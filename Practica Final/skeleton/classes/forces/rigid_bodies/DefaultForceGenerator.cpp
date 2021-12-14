#include "DefaultForceGenerator.h"

#include "../../../utils/maths.h"
#include <iostream>
#include "DefaultForceGenerator.h"

void DefaultForceGenerator::addForce(Vector3 force)
{
	f = force;
	setActive(true);
}

void DefaultForceGenerator::updateForce(RigidBody* rb, float t)
{
	if (rb->rigidData->getInvMass() == 0)
		return;

	rb->rigidData->addForce(f);
	setActive(false);
}