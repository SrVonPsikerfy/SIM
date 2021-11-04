#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <list>

#include "../utils/checkML.h"
#include "../utils/core.hpp"

#include "Particle.h"
#include "ParticleForceGenerator.h"
#include "ParticleForceRegistry.h"

enum class SpawnType { NONE, FOUNTAIN };

class ParticleSystem {
public:
	ParticleSystem(ParticleForceRegistry* fR, Vector3 pos = { 0, 0, 0 }, double spawn = -1) : 
		spawnTime(spawn), posSystem(pos), spType(SpawnType::NONE), fReg(fR) {};
	virtual ~ParticleSystem();

	void update(double t);
	void reset();

	void generateBullet(Vector3 pos, ParticleData data);
	void spawnFountain(double spawn);
	void spawnOnSphere(Vector3 pos, double radius, int num);

	void addForceGenerator(ParticleForceGenerator* fg);
	void applyForceGenerator(ParticleForceGenerator* fg, bool instantForce);

protected:
	virtual void onParticleDeath(int particle);
	void releaseParticle(int numParticle);

	void spawnParticle(double t);

	void generateFountainParticle();

	std::vector<Particle*> particles;
	std::list<ParticleForceGenerator*> fgs;
	ParticleForceRegistry* fReg;

	Vector3 posSystem;
	SpawnType spType;
	double spawnTime, nextSpawn = 0;
};

#endif