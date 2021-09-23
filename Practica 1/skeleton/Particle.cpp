#include "Particle.h"

Particle::Particle(Vector3 position, Vector3 velocity, Vector3 acceleration,
	Vector4 color, double damp) {
	pos = position;
	vel = velocity;
	acc = acceleration;

	damping = damp;

	pose = physx::PxTransform(pos.x, pos.y, pos.z);
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(3)), &pose, color);

	deathTime = (std::rand() % 6 + 0.5);
}

Particle::~Particle() {
	DeregisterRenderItem(renderItem);
	delete renderItem;
}

bool Particle::update(double t) {
	pose.p = pose.p + vel * t; // movement
	vel += acc * t; // acceleration
	vel *= powf(damping, t); // impose drag

	time += t;
	if (time > deathTime)
		return true;
	return false;
}