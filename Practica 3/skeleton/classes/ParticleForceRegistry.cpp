#include "ParticleForceRegistry.h"

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg) {
	registrations.push_back(ParticleForceRegistration(particle, fg));
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg) {
	registrations.remove(ParticleForceRegistration(particle, fg));
}

void ParticleForceRegistry::clear() {
	for (auto it = registrations.begin(); it != registrations.end(); ++it)
		registrations.erase(it);
}

void ParticleForceRegistry::clearParticleLinks(Particle* pt) {
	for (auto it = registrations.begin(); it != registrations.end(); ++it) 
		if (it->particle == pt)
			registrations.erase(it);
}

void ParticleForceRegistry::clearForceLinks(ParticleForceGenerator* fg) {
	for (auto it = registrations.begin(); it != registrations.end(); ++it)
		if (it->fg == fg)
			registrations.erase(it);
}

void ParticleForceRegistry::updateForces(float t) {
	for (auto it = registrations.begin(); it != registrations.end(); ++it)
		it->fg->updateForce(it->particle, t);
}
