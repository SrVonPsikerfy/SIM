#ifndef FIREWORKSYSTEM_H
#define FIREWORKSYSTEM_H

#include <vector>

#include "../../utils/checkML.h"
#include "../../utils/core.hpp"

#include "ParticleSystem.h"
#include "Firework.h"

using namespace std;

class FireworkSystem : public ParticleSystem
{
public:
	FireworkSystem(ForceRegistry* fR, Vector3 pos = { 0, 0, 0 }, double spawn = -1) 
		: ParticleSystem(fR, pos, spawn) {};
	virtual ~FireworkSystem() {};

	void generateFirework(FireworkLoadType type, int payload, int lifes, ParticleData data);

private:
	void ignite(Vector3 particlePos, FireworkLoadType loadType, int payload, int life);

	virtual void onParticleDeath(int particle);
};
#endif