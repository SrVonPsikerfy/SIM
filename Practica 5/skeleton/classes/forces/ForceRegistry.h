#pragma once

#include <vector>

#include "../../utils/checkML.h"

#include "ParticleForceGenerator.h"
#include "RigidBodyForceGenerator.h"

class ForceRegistry
{
protected:
	// Storage for generator-particle entry
	struct ParticleForceRegistration {
		ParticleForceRegistration(Particle* p, ParticleForceGenerator* fgen) :
			particle(p), fg(fgen) {};

		Particle* particle;
		ParticleForceGenerator* fg;
	};

	// Storage for generator-rigid body entry
	struct RigidBodyForceRegistration {
		RigidBodyForceRegistration(RigidBody* rb, RigidBodyForceGenerator* fgen) :
			rigidBody(rb), fg(fgen) {};

		RigidBody* rigidBody;
		RigidBodyForceGenerator* fg;
	};

	using ParticleForceRegistry = std::vector<ParticleForceRegistration>;
	ParticleForceRegistry registrations;

	using RigidBodyForceRegistry = std::vector<ParticleForceRegistration>;
	RigidBodyForceRegistry registrations;

public:
	ForceRegistry() {};
	~ForceRegistry() {};

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
