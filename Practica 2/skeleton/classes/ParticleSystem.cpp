#include "ParticleSystem.h"

ParticleSystem::~ParticleSystem() {
	for (int i = 0; i < particles.size(); i++)
		delete particles[i];
}

void ParticleSystem::update(double t) {
	for (int i = particles.size() - 1; i >= 0; i--) {
		particles[i]->update(t);
		if (particles[i]->dead())
			onParticleDeath(i);
	}

	if (spawnTime != -1)
		spawnParticle(t);
}

void ParticleSystem::generateBullet(Vector3 pos, ParticleData data) {
	particles.push_back(new Particle(pos, data));
}

void ParticleSystem::generateFountain(ParticleData data, double spawn){

}

void ParticleSystem::generateFirework(FireworkLoadType type, ParticleData data) {
	particles.push_back(new Firework(type, posSystem, data));
}

void ParticleSystem::onParticleDeath(int particle) {
	Firework* fr = dynamic_cast<Firework*>(particles[particle]);
	if (fr != nullptr) 
		ignite(fr->getPos(), fr->getFireworkLoadType());

	releaseParticle(particle);
}

void ParticleSystem::spawnParticle(double t) {
	nextSpawn += t;
	if (nextSpawn >= spawnTime) {
		particles.push_back(new Particle(posSystem, pData));
		nextSpawn = 0;
	}
}

void ParticleSystem::releaseParticle(int numParticle) {
	delete particles[numParticle];
	particles.erase(particles.begin() + numParticle);
}

void ParticleSystem::ignite(Vector3 particlePos, FireworkLoadType loadType) {
	switch (loadType) {
	case FireworkLoadType::FLOWER:

		break;
	}
}
