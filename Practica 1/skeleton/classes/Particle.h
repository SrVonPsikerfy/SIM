#ifndef PARTICLE_H
#define PARTICLE_H

#include "../utils/core.hpp"
#include "../utils/RenderUtils.hpp"
#include "../utils/checkML.h"

class Particle
{
public:
	Particle() noexcept;
	Particle(Vector3 position, Vector3 velocity, Vector3 acceleration, Vector4 color,
		double mass, double time = std::rand() % 4 + 3.5, float damp = 0.999);
	~Particle();

	virtual bool update(double t);

	inline void setPosition(Vector3 p) { pos = p; pose.p = p; };
	inline void setVelocity(Vector3 v) { vel = v; };
	inline void setAcceleration(Vector3 a) { acc = a; };
	inline void setDamping(float damp) noexcept { damping = damp; };
	void setMass(double mass);
	inline void setColor(Vector4 c) { renderItem->color = c; };
	inline void setDeathTime(double t) { deathTime = t; };
	inline void resetLifeTime() { lifetime = 0; };

	inline Vector3 getPosition() { return pos; };
	inline Vector3 getVelocity() { return vel; };
	inline Vector3 getAcceleration() { return vel; };
	inline float getDamping() noexcept { return damping; };
	inline double getMass() noexcept { return 1 / inverse_mass; };
	inline Vector4 getColor() { return renderItem->color; };
	inline double getDeathTime() noexcept { return deathTime; };

protected:
	void integrate(double t);
	bool checkDeath(double t);

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

	// Lifetime of the particle (-1 if not supposed to die)
	double deathTime, lifetime = 0;
};

#endif