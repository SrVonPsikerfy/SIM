#ifndef FIREWORK_H_
#define FIREWORK_H_

#include "Particle.h"
#include "../utils/core.hpp"
#include "../utils/checkML.h"

enum class FireworkLoadType { NONE, FLOWER, FRACTAL, RANDOM };

class Firework : public Particle
{
public:
	Firework() : Particle(), type(FireworkLoadType::NONE) {};
	Firework(FireworkLoadType lType, Vector3 position, Vector3 velocity, Vector3 acceleration, Vector4 color,
		double inverse_mass, double size, double time = 1, float damp = 0.999, bool prog = false) :
		Particle(position, velocity, acceleration, color, inverse_mass, size, time, damp, prog), type(lType) {};
	Firework(FireworkLoadType lType, Vector3 sysPos, ParticleData data) : Particle(sysPos, data), type(lType) {};

	virtual ~Firework() {};

	FireworkLoadType getFireworkLoadType() { return type; }

private:
	FireworkLoadType type;
};

#endif