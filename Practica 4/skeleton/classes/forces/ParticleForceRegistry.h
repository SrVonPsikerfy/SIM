#pragma once

#include <vector>

#include "../../utils/checkML.h"

#include "ParticleForceGenerator.h"

class ParticleForceRegistry
{
protected:
	// Storage for generator-particle entry
	struct ParticleForceRegistration {
		ParticleForceRegistration(Particle* p, ParticleForceGenerator* fgen, bool app) :
			particle(p), fg(fgen), instantApplication(app) {};

		Particle* particle;
		ParticleForceGenerator* fg;

		bool instantApplication;
	};

	using Registry = std::vector<ParticleForceRegistration>;
	Registry registrations;

public:
	ParticleForceRegistry() {};
	~ParticleForceRegistry() {};

	// Associate generator with a particle
	void add(Particle* particle, ParticleForceGenerator* fg, bool app = false);
	// Remove association
	void remove(Particle* particle, ParticleForceGenerator* fg);
	// Removes all associations. Particle and Generators won't be deleted
	void clear();
	// Removes all associations between a particle and all its forces
	void clearParticleLinks(Particle* pt);
	// Removes all associations between a force and all its particles
	void clearForceLinks(ParticleForceGenerator* fg);
	// Update all the generators in the registry
	void updateForces(float t);
};
