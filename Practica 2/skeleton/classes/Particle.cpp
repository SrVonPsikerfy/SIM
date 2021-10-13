#include "Particle.h"

#include <iostream>

Particle::Particle() : death(false) {
	pos = Vector3(0, 0, 0);
	vel = Vector3(0, 0, 0);
	acc = Vector3(0, 0, 0);

	damping = 1;

	size = 3;
	pose = physx::PxTransform(pos.x, pos.y, pos.z);
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(size)), &pose, { 1, 1, 1, 1 });

	inverse_mass = 1;

	deathTime = 0;
}

Particle::Particle(Vector3 position, Vector3 velocity, Vector3 acceleration,
	Vector4 color, double sizeSh, double inv_mass, double time, float damp, bool prog) : death(false) {

	pos = position;
	vel = velocity;
	acc = acceleration;

	damping = damp;

	size = sizeSh;
	pose = physx::PxTransform(pos.x, pos.y, pos.z);
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(size)), &pose, color);

	inverse_mass = inv_mass;

	deathTime = time;

	progThroughTime = prog;
}

Particle::Particle(Vector3 sysPos, ParticleData data) : death(false) {

	pos = sysPos + data.offset;
	vel = data.initialSpeed;
	acc = data.acceleration;

	damping = data.damp;

	size = data.size;
	pose = physx::PxTransform(pos.x, pos.y, pos.z);
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(size)), &pose, data.color);

	inverse_mass = data.inv_mass;

	deathTime = data.lifeTime;
}

Particle::~Particle() {
	renderItem->release();
}

void Particle::update(double t) {
	integrate(t);
	death = checkDeath(t);
}

void Particle::integrate(double t) {
	if (inverse_mass == 0.0) // infinite mass --> do nothing
		throw std::exception("Mass is infinite, unable to calculate physics");

	pose.p = pose.p + vel * t; // movement
	vel += acc * t; // acceleration
	vel *= powf(damping, t); // impose drag
}

bool Particle::checkDeath(double t) {
	if (lifetime != -1) lifetime += t; // life time increase
	//if (progThroughTime && !(lifetime > deathTime))
	//	changeParticleThroughTime(lifetime);
	return lifetime > deathTime;
}
//
//void Particle::changeParticleThroughTime(double t) {
//	Vector4 color = renderItem->color;
//	double size = renderItem->shape->getGeometry().sphere().radius;
//    renderItem->release();
//
//	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(size * (t / deathTime))), &pose, color);
//}