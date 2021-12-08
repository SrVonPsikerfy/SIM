#include "ForceRegistry.h"

void ForceRegistry::add(Particle* particle, ParticleForceGenerator* fg) {
	registry_pt.push_back(ParticleForceRegistration(particle, fg));
}

void ForceRegistry::add(RigidBody* rb, RigidBodyForceGenerator* fg) {
	registry_rb.push_back(RigidBodyForceRegistration(rb, fg));
}

void ForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg) {
	for (size_t i = 0; i < registry_pt.size(); i++)
		if (registry_pt[i].particle == particle && registry_pt[i].ptfg == fg) {
			registry_pt.erase(registry_pt.begin() + i); return;
		}
}

void ForceRegistry::remove(RigidBody* rb, RigidBodyForceGenerator* fg) {
	for (size_t i = 0; i < registry_rb.size(); i++)
		if (registry_rb[i].rigidBody == rb && registry_rb[i].rbfg == fg) {
			registry_rb.erase(registry_rb.begin() + i); return;
		}
}

void ForceRegistry::clear() {
	registry_pt.clear();
	registry_rb.clear();
}

void ForceRegistry::clearParticleLinks(Particle* pt) {
	for (size_t i = 0; i < registry_pt.size(); i++)
		if (registry_pt[i].particle == pt) {
			registry_pt.erase(registry_pt.begin() + i); --i;
		}
}

void ForceRegistry::clearRigidBodyLinks(RigidBody* rb) {
	for (size_t i = 0; i < registry_rb.size(); i++)
		if (registry_rb[i].rigidBody == rb) {
			registry_rb.erase(registry_rb.begin() + i); --i;
		}
}

void ForceRegistry::clearParticleForceLinks(ParticleForceGenerator* fg) {
	for (size_t i = 0; i < registry_pt.size(); i++)
		if (registry_pt[i].ptfg == fg) {
			registry_pt.erase(registry_pt.begin() + i); --i;
		}
}

void ForceRegistry::clearRigidBodyForceLinks(RigidBodyForceGenerator* fg) {
	for (size_t i = 0; i < registry_rb.size(); i++)
		if (registry_rb[i].rbfg == fg) {
			registry_rb.erase(registry_rb.begin() + i); --i;
		}
}

void ForceRegistry::updateForces(float t) {
	for (size_t i = 0; i < registry_pt.size(); i++)
		if (registry_pt[i].ptfg->isActive())
			registry_pt[i].ptfg->updateForce(registry_pt[i].particle, t);

	for (size_t i = 0; i < registry_rb.size(); i++)
		if (registry_rb[i].rbfg->isActive())
			registry_rb[i].rbfg->updateForce(registry_rb[i].rigidBody, t);
}
