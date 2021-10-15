#ifndef PARTICLE_H
#define PARTICLE_H

#include "../utils/core.hpp"
#include "../utils/RenderUtils.hpp"
#include "../utils/checkML.h"

// data structure to compress info
struct ParticleData {
	Vector3 offset;
	Vector3 initialSpeed;
	Vector3 acceleration;

	double damp;
	double inv_mass;
	double size;
	double lifeTime;

	bool progThroughTime;

	Vector4 color;
};

class Particle
{
public:
	Particle();
	Particle(Vector3 position, Vector3 velocity, Vector3 acceleration, Vector4 color,
		double inverse_mass, double size, double time = 1, float damp = 0.999, bool prog = false);
	Particle(Vector3 sysPos, ParticleData data);
	virtual ~Particle();

	virtual void update(double t);

	inline bool dead() noexcept { return death; };
	inline Vector3 getPos() { return pose.p; };

	void clearForce(); // Clears accumulated force
	void addForce(const Vector3& f); // Add force to apply in next integration only

	inline void setPosition(Vector3 p) { pos = p; pose.p = p; };
	inline void setVelocity(Vector3 v) { vel = v; };
	inline void setAcceleration(Vector3 a) { acc = a; };
	inline void setDamping(float damp) noexcept { damping = damp; };
	inline void setMass(double inv_mass) noexcept {	inverse_mass = inv_mass; };
	inline void setColor(Vector4 c) { renderItem->color = c; };
	inline void resetLifeTime() noexcept{ lifetime = 0; };

protected:
	void integrate(double t);
	bool checkDeath(double t);

	// in case we want to do something with the particle through time
	virtual void changeParticleThroughTime(double t);

	Vector3 pos; // Position in world space
	Vector3 vel; // Linear velocity in world space
	Vector3 acc; // Linear acceleration in world space
	Vector3 force; // Accumulated force

	// How much linear velocity is lost.
	// To improve stability of the engine
	float damping;

	// rendering elements
	physx::PxTransform pose;
	RenderItem* renderItem;
	double size;

	// Inverse mass, so 0 isn't a viable option
	double inverse_mass;

	// Lifetime of the particle (-1 if not supposed to die)
	double deathTime, lifetime = 0;
	bool death; // to know if the particle is dead

	// bool in case we want an action to occur while lifetime is reduced
	bool progThroughTime;
};

#endif