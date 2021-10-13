#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>

#include "Particle.h"
#include "Firework.h"
#include "../utils/core.hpp"
#include "../utils/checkML.h"

class ParticleSystem {
public:
	ParticleSystem(Vector3 pos = { 0, 0, 0 }, double spawn = -1) : spawnTime(spawn), posSystem(pos) {};
	virtual ~ParticleSystem();

	void update(double t);

	void generateBullet(Vector3 pos, ParticleData data);
	void generateFountain(ParticleData data, double spawn);
	void generateFirework(FireworkLoadType type, ParticleData data);

protected:
	void onParticleDeath(int particle);
	void releaseParticle(int numParticle);

	void ignite(Vector3 particlePos, FireworkLoadType loadType);

	void spawnParticle(double t);

	std::vector<Particle*> particles;

	Vector3 posSystem;
	ParticleData pData;
	double spawnTime, nextSpawn = 0;
};

#endif