#ifndef PARTICLE_H
#define PARTICLE_H

#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3 position, Vector3 velocity, Vector3 acceleration, Vector4 color, double damp = 0.999);
	~Particle();

	bool update(double t);

private:
	Vector3 pos; // Position in world space
	Vector3 vel; // Linear velocity in world space
	Vector3 acc; // Linear acceleration in world space

	// How much linear velocity is lost.
	// To improve stability of the engine
	double damping;

	physx::PxTransform pose;
	RenderItem* renderItem;

	double deathTime, time = 0;
};

#endif