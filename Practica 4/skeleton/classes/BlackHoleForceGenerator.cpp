#include "BlackHoleForceGenerator.h"

BlackHoleForceGenerator::BlackHoleForceGenerator(const Vector3& pos_, double force, double size)
	: holeForce(force), radius(size) {
	pos = physx::PxTransform(pos_);
	field = new RenderItem(CreateShape(physx::PxSphereGeometry(size)), &pos, { 0, 0, 0, 0 });
}

BlackHoleForceGenerator::~BlackHoleForceGenerator() {
	field->release();
}

void BlackHoleForceGenerator::updateForce(Particle* particle, float t) {
	if (particle->hasFiniteMass() || !onField(particle->getPos())) return;
	Vector3 diff = pos.p - particle->getPos();
	double linealForce = holeForce * (1 - (diff.magnitude() / radius));
	auto x = diff.getNormalized() * linealForce * particle->getMass();
	particle->addForce(x);
}

bool BlackHoleForceGenerator::onField(const Vector3& p) {
	return (pos.p - p).magnitude() < radius;
}