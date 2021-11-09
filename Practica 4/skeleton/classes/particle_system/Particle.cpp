#include "Particle.h"

Particle::Particle() : death(false) {
	pos = Vector3(0, 0, 0);
	vel = Vector3(0, 0, 0);
	acc = Vector3(0, 0, 0);
	force = Vector3(0, 0, 0);

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
	force = { 0, 0, 0 };

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
	force = { 0, 0, 0 };

	damping = data.damp;

	size = data.size;
	pose = physx::PxTransform(pos.x, pos.y, pos.z);
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(size)), &pose, data.color);

	inverse_mass = data.inv_mass;

	deathTime = data.lifeTime;

	progThroughTime = data.progThroughTime;
}

Particle::~Particle() {
	renderItem->release();
}

void Particle::update(double t) {
	integrate(t);
	death = checkDeath(t);
}

void Particle::integrate(double t) {
	if (hasFiniteMass()) // infinite mass --> do nothing
		throw std::exception("Mass is infinite, unable to calculate physics");

	pose.p = pose.p + vel * t; // update position
	vel += acc * t; // update velocity
	vel *= powf(damping, t); // impose drag
	// not cumulative since force does the work for the acceleration, and that one already is updated
	acc = force * inverse_mass; // update acceleration

	clearForce();
}

bool Particle::checkDeath(double t) {
	if (lifetime != -1) lifetime += t; // life time increase
	if (progThroughTime && !(lifetime > deathTime))
		changeParticleThroughTime(lifetime);
	return lifetime > deathTime;
}

void Particle::changeParticleThroughTime(double currentLifeTime) {
	Vector4 color = renderItem->color;
	renderItem->release();

	float nuevoSize = size * (deathTime - currentLifeTime) / deathTime;

	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(nuevoSize)), &pose, color);
}