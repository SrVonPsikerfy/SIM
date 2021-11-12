#include "ParticleForceRegistry.h"
#include <iostream>

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg) {
	registrations.push_back(ParticleForceRegistration(particle, fg));
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg) {
	for (size_t i = 0; i < registrations.size(); i++) 
		if (registrations[i].particle == particle && registrations[i].fg == fg) {
			registrations.erase(registrations.begin() + i); return;
		}
}

void ParticleForceRegistry::clear() {
	registrations.clear();
}

void ParticleForceRegistry::clearParticleLinks(Particle* pt) {
	for (size_t i = 0; i < registrations.size(); i++) 
		if (registrations[i].particle == pt) 
			registrations.erase(registrations.begin() + i);
}

void ParticleForceRegistry::clearForceLinks(ParticleForceGenerator* fg) {
	for (size_t i = 0; i < registrations.size(); i++)
		if (registrations[i].fg == fg) 
			registrations.erase(registrations.begin() + i);
}

void ParticleForceRegistry::updateForces(float t) {
	for (size_t i = 0; i < registrations.size(); i++)
		if (registrations[i].fg->isActive())
			registrations[i].fg->updateForce(registrations[i].particle, t);
}
