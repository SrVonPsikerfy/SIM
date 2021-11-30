#ifndef FORCEREGISTRY_H
#define FORCEREGISTRY_H

#include <vector>

#include "../../utils/checkML.h"

#include "./particles/ParticleForceGenerator.h"
#include "./rigid_bodies/RigidBodyForceGenerator.h"

class ForceRegistry
{
protected:
	// Storage for generator-particle entry
	struct ParticleForceRegistration {
		ParticleForceRegistration(Particle* p, ParticleForceGenerator* fgen) :
			particle(p), ptfg(fgen) {};

		Particle* particle;
		ParticleForceGenerator* ptfg;
	};

	// Storage for generator-rigid body entry
	struct RigidBodyForceRegistration {
		RigidBodyForceRegistration(RigidBody* rb, RigidBodyForceGenerator* fgen) :
			rigidBody(rb), rbfg(fgen) {};

		RigidBody* rigidBody;
		RigidBodyForceGenerator* rbfg;
	};

	using ParticleForceRegistry = std::vector<ParticleForceRegistration>;
	ParticleForceRegistry registry_pt;

	using RigidBodyForceRegistry = std::vector<RigidBodyForceRegistration>;
	RigidBodyForceRegistry registry_rb;

public:
	ForceRegistry() {};
	~ForceRegistry() {};

	// Associate generator with a particle/rigid body
	void add(Particle* particle, ParticleForceGenerator* fg);
	void add(RigidBody* rb, RigidBodyForceGenerator* fg);

	// Remove association
	void remove(Particle* particle, ParticleForceGenerator* fg);
	void remove(RigidBody* rb, RigidBodyForceGenerator* fg);

	// Removes all associations. Particle, RigidBody and Generators won't be deleted
	void clear();

	// Removes all associations between a particle/rigid body and all its forces
	void clearParticleLinks(Particle* pt);
	void clearRigidBodyLinks(RigidBody* rb);

	// Removes all associations between a force and all its particles/rigid bodies
	void clearParticleForceLinks(ParticleForceGenerator* fg);
	void clearRigidBodyForceLinks(RigidBodyForceGenerator* fg);

	// Update all the generators in the registries
	void updateForces(float t);
};
#endif