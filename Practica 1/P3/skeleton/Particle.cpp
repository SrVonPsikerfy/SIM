#include "Particle.h"

Particle::Particle(Vector3 position, Vector3 velocity, Vector3 acceleration, double damp)
{
	pos = position;
	vel = velocity;
	acc = acceleration;

	damping = damp;

	pose = physx::PxTransform(pos.x, pos.y, pos.z);
	renderItem = new RenderItem(CreateShape(physx::PxBoxGeometry(3, 3, 3)), &pose, { 0,1,0.5,1 });
}

Particle::~Particle() {
	DeregisterRenderItem(renderItem);
}

void Particle::update(double t)
{
	// movement
	pose.p = pose.p + vel * t;

	// acceleration
	vel += acc * t;

	// impose drag
	vel *= powf(damping, t);
}