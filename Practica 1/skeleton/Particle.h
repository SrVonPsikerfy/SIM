#ifndef PARTICLE_H
#define PARTICLE_H

#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3 position, Vector3 velocity, Vector3 acceleration, double damp = 0.999);
	~Particle();

private:
	// Position in world space
	Vector3 pos;
	
	// Linear velocity in world space
	Vector3 vel;

	// Linear acceleration in world space
	Vector3 acc;

	// How much linear velocity is lost.
	// To improve stability of the engine
	double damping;

	physx::PxTransform pose;

	RenderItem* renderItem;
};

#endif