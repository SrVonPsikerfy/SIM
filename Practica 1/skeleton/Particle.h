#ifndef PARTICLE_H
#define PARTICLE_H

#include "core.hpp"
#include "RenderUtils.hpp"
#include "checkML.h"

class Particle
{
public:
	Particle() noexcept;
	Particle(Vector3 position, Vector3 velocity, Vector3 acceleration, Vector4 color,
		double mass, double time = std::rand() % 4 + 3.5, float damp = 0.999);
	//Particle(const Particle& p) noexcept;
	~Particle();

	bool update(double t);

	void setPosition(Vector3 p);
	void setVelocity(Vector3 v);
	void setAcceleration(Vector3 a);
	void setDamping(float damp);
	void setMass(double mass);
	void setColor(Vector4 c);
	void setDeathTime(double t);
	void resetLifeTime();

protected:
	Vector3 pos; // Position in world space
	Vector3 vel; // Linear velocity in world space
	Vector3 acc; // Linear acceleration in world space

	// How much linear velocity is lost.
	// To improve stability of the engine
	float damping;

	physx::PxTransform pose;
	RenderItem* renderItem;

	// Inverse mass, so 0 isn't a viable option
	double inverse_mass;

	// Lifetime of the particle
	double deathTime, lifetime = 0;
};

#endif