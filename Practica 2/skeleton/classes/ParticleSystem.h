#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>

#include "../utils/core.hpp"
#include "../utils/checkML.h"

#include "Particle.h"

enum class SpawnType { NONE, FOUNTAIN };

class ParticleSystem {
public:
	ParticleSystem(Vector3 pos = { 0, 0, 0 }, double spawn = -1) : spawnTime(spawn), posSystem(pos), spType(SpawnType::NONE) {};
	virtual ~ParticleSystem();

	void update(double t);
	void reset();

	void generateBullet(Vector3 pos, ParticleData data);
	void spawnFountain(double spawn);

protected:
	virtual void onParticleDeath(int particle);
	void releaseParticle(int numParticle);

	void spawnParticle(double t);

	void generateFountainParticle();

	std::vector<Particle*> particles;

	Vector3 posSystem;
	SpawnType spType;
	double spawnTime, nextSpawn = 0;
};

#endif