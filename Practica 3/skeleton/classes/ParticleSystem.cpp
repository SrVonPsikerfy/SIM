#include "ParticleSystem.h"

ParticleSystem::~ParticleSystem() {
	reset();
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

void ParticleSystem::reset() {
	for (int i = 0; i < particles.size(); i++)
		delete particles[i];
	particles.clear();

	spawnTime = -1;
}

void ParticleSystem::generateBullet(Vector3 pos, ParticleData data) {
	particles.push_back(new Particle(pos, data));
	for (auto fg : fgs)
		fReg->add(particles[particles.size() - 1], fg);
}

void ParticleSystem::spawnFountain(double spawn) {
	spType = SpawnType::FOUNTAIN;
	spawnTime = spawn;
}

void ParticleSystem::applyForceGenerator(ParticleForceGenerator* fg) {
	fgs.push_back(fg);
}

void ParticleSystem::onParticleDeath(int particle) {
	releaseParticle(particle);
}

void ParticleSystem::spawnParticle(double t) {
	nextSpawn += t;
	if (nextSpawn >= spawnTime) {
		switch (spType) {
		case SpawnType::FOUNTAIN:
			generateFountainParticle();
			break;
		case SpawnType::NONE:
			nextSpawn = 0;
			break;
		}
	}
}

void ParticleSystem::generateFountainParticle() {
	ParticleData pData;
	int vel = 10;
	float x = (-2 + (rand() % 7)), y = (rand() % 8), z = (-2 + (rand() % 7)), blue = (((float)rand()) / RAND_MAX);

	pData.offset = { 0, 0, 0 }; pData.initialSpeed = { vel * x, vel * y, vel * z };
	pData.acceleration = { 0, -30, 0 };

	pData.damp = 1;	pData.inv_mass = 1;
	pData.size = 2;	pData.lifeTime = rand() % 2 + 2;
	pData.progThroughTime = true; pData.color = { 0, 0, blue, 1 };

	particles.push_back(new Particle(posSystem, pData));

	spawnTime = ((float)rand() / RAND_MAX) / 16;
	nextSpawn = 0;
}

void ParticleSystem::releaseParticle(int numParticle) {
	fReg->clearParticleLinks(particles[numParticle]);

	delete particles[numParticle];
	particles.erase(particles.begin() + numParticle);
}