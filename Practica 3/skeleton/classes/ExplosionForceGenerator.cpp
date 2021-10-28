#include "ExplosionForceGenerator.h"

ExplosionForceGenerator::ExplosionForceGenerator(const Vector3& pos_, double force, double size) 
	: exForce(force), radius(size) {
	pos = physx::PxTransform(pos_);
	field = new RenderItem(CreateShape(physx::PxSphereGeometry(size)), &pos, { 1, 0, 1, 0 });
}

ExplosionForceGenerator::~ExplosionForceGenerator() {
	field->release();
}

void ExplosionForceGenerator::updateForce(Particle* particle, float t) {
	if (particle->hasFiniteMass() || !onField(particle->getPos())) return;
	Vector3 diff = particle->getPos() - pos.p;
	double linealForce = exForce * (1 - (diff.magnitude() / radius));
	auto x = diff.getNormalized() * linealForce * particle->getMass();
	particle->addForce(x);
}

bool ExplosionForceGenerator::onField(const Vector3& p) {
	return (pos.p - p).magnitude() < radius;
}