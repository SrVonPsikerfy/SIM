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

//Particle::Particle(const Particle& p) noexcept
//{
//	pos = p.pos; vel = p.vel;
//	acc = p.acc; damping = p.damping;
//	pose = p.pose; renderItem = p.renderItem;
//}

Particle::~Particle() {
	DeregisterRenderItem(renderItem);
	delete renderItem;
}

bool Particle::update(double t) {
	if (inverse_mass == 0.0) // infinite mass --> do nothing
		throw std::exception("Mass is infinite, unable to calculate physics");

	pose.p = pose.p + vel * t; // movement
	vel += acc * t; // acceleration
	vel *= powf(damping, t); // impose drag

	lifetime += t; // life time increase
	return lifetime > deathTime;
}

void Particle::setPosition(Vector3 p) {
	pos = p;
	pose.p = p;
}

void Particle::setVelocity(Vector3 v) {
	vel = v;
}

void Particle::setAcceleration(Vector3 a) {
	acc = a;
}

void Particle::setDamping(float damp) {
	damping = damp;
}

void Particle::setMass(double mass) {
	if (mass == 0)
		throw std::exception("Can't have mass equal to 0");

	inverse_mass = 1 / mass;
}

void Particle::setColor(Vector4 c) {
	renderItem->color = c;
}

void Particle::setDeathTime(double t) {
	deathTime = t;
}

void Particle::resetLifeTime() {
	lifetime = 0;
}