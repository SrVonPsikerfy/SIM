#pragma once

#include <list>

#include "../utils/checkML.h"

#include "ParticleForceGenerator.h"

class ParticleForceRegistry
{
protected:
	// Storage for generator-particle entry
	struct ParticleForceRegistration {
		ParticleForceRegistration(Particle* p, ParticleForceGenerator* fgen) :
			particle(p), fg(fgen) {};

		Particle* particle;
		ParticleForceGenerator* fg;

		bool operator==(ParticleForceRegistration const& a) const {
			return a.fg == fg && a.particle == particle;
		}
	};

	using Registry = std::list<ParticleForceRegistration>;
	Registry registrations;

public:
	ParticleForceRegistry() {};
	~ParticleForceRegistry() {};

	// Associate generator with a particle
	void add(Particle* particle, ParticleForceGenerator* fg);
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
