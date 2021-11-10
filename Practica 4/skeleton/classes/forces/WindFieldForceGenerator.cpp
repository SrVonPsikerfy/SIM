#include "WindFieldForceGenerator.h"

#include "../../utils/maths.h"
#include <iostream>

void WindFieldForceGenerator::updateForce(Particle* particle, float t) {
	if (particle->hasFiniteMass() || !onField(particle->getPos()))
		return;

	particle->addForce(windForce);
}

void WindFieldForceGenerator::update(double t) {
	if (timeThreshold != -1) {
		changeWindDir -= t;
		if (changeWindDir <= 0) {
			windForce = Vector3(maths::random<double>(-10, 10) * 30, maths::random<double>(-10, 10) * 30, 0);
			changeWindDir = timeThreshold;
		}
	}
}

void WindFieldForceGenerator::switchWindType(double t) {
	if (timeThreshold != -1)
		changeWindDir = timeThreshold = -1;
	else
		changeWindDir = timeThreshold = t;
}
