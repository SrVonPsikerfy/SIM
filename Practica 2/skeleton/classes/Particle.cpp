#include "Particle.h"

Particle::Particle() noexcept {
	pos = Vector3(0, 0, 0);
	vel = Vector3(0, 0, 0);
	acc = Vector3(0, 0, 0);

	damping = 1;

	pose = physx::PxTransform(pos.x, pos.y, pos.z);
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(3)), &pose, { 1, 1, 1, 1 });

	inverse_mass = 1;

	deathTime = 0;
}

Particle::Particle(Vector3 position, Vector3 velocity, Vector3 acceleration,
	Vector4 color, double mass, double time, float damp) {
	if (mass == 0)
		throw std::exception("Can't have mass equal to 0");

	pos = position;
	vel = velocity;
	acc = acceleration;

	damping = damp;

	pose = physx::PxTransform(pos.x, pos.y, pos.z);
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(3)), &pose, color);

	inverse_mass = 1 / mass;

	deathTime = time;
}

Particle::~Particle() {
	renderItem->release();
}

bool Particle::update(double t) {
	integrate(t);
	return checkDeath(t);
}

void Particle::setMass(double mass) {
	if (mass == 0)
		throw std::exception("Can't have mass equal to 0");

	inverse_mass = 1 / mass;
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
	return lifetime > deathTime;
}