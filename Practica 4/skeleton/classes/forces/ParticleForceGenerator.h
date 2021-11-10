#ifndef PARTICLEFORCEGENERATOR_H
#define PARTICLEFORCEGENERATOR_H

#include "../../utils/checkML.h"

#include "../particle_system/Particle.h"

class ParticleForceGenerator
{
public:
	// Overload to provide functionality
	virtual void updateForce(Particle* particle, float t) = 0;
	virtual void update(double time) {};

	void setActive(bool act) { active = act; };
	bool isActive() { return active; };

protected:
	bool active = true;
};
#endif