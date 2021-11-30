#include "ForceRegistry.h"
#include <iostream>

void ForceRegistry::add(Particle* particle, ParticleForceGenerator* fg) {
	registrations.push_back(ParticleForceRegistration(particle, fg));
}

void ForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg) {
	for (size_t i = 0; i < registrations.size(); i++) 
		if (registrations[i].particle == particle && registrations[i].fg == fg) {
			registrations.erase(registrations.begin() + i); return;
		}
}

void ForceRegistry::clear() {
	registrations.clear();
}

void ForceRegistry::clearParticleLinks(Particle* pt) {
	for (size_t i = 0; i < registrations.size(); i++) 
		if (registrations[i].particle == pt) 
			registrations.erase(registrations.begin() + i);
}

void ForceRegistry::clearForceLinks(ParticleForceGenerator* fg) {
	for (size_t i = 0; i < registrations.size(); i++)
		if (registrations[i].fg == fg) 
			registrations.erase(registrations.begin() + i);
}

void ForceRegistry::updateForces(float t) {
	for (size_t i = 0; i < registrations.size(); i++)
		if (registrations[i].fg->isActive())
			registrations[i].fg->updateForce(registrations[i].particle, t);
}
