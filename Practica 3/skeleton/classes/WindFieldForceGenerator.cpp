#include "WindFieldForceGenerator.h"

WindFieldForceGenerator::WindFieldForceGenerator(const Vector3& force, const Vector3& pos_, double size) 
	: windForce(force), radius(size) {
	pos = physx::PxTransform(pos_);
	field = new RenderItem(CreateShape(physx::PxSphereGeometry(size)), &pos, { 0, 0, 1, 0 });
}

WindFieldForceGenerator::~WindFieldForceGenerator() {
	field->release();
}

void WindFieldForceGenerator::updateForce(Particle* particle, float t) {
	if (particle->hasFiniteMass() || !onField(particle->getPos())) return;
	particle->addForce(windForce * particle->getMass());
}

bool WindFieldForceGenerator::onField(const Vector3& p) {
	return (pos.p - p).magnitude() < radius;
}