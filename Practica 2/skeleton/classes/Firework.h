#ifndef FIREWORK_H_
#define FIREWORK_H_

#include "../utils/core.hpp"
#include "../utils/checkML.h"

#include "Particle.h"

enum class FireworkLoadType { NONE, FLOWER, SPHERE, RANDOM };

class Firework : public Particle
{
public:
	Firework() : Particle(), type(FireworkLoadType::NONE), payload(0), lifes(0) {};
	Firework(FireworkLoadType lType, int lifes_, int pl, Vector3 position, Vector3 velocity, Vector3 acceleration, Vector4 color,
		double inverse_mass, double size, double time = 1, float damp = 0.999, bool prog = false) :
		Particle(position, velocity, acceleration, color, inverse_mass, size, time, damp, prog), type(lType), payload(pl), lifes(lifes_) {};
	Firework(FireworkLoadType lType, int pl, int lifes_, Vector3 sysPos, ParticleData data) : Particle(sysPos, data), type(lType), payload(pl), lifes(lifes_) {};

	virtual ~Firework() {};

	FireworkLoadType getFireworkLoadType() { return type; }
	int getPayload() { return payload; }
	int getLifes() { return payload; }

private:
	FireworkLoadType type;
	int payload, lifes;
};

#endif