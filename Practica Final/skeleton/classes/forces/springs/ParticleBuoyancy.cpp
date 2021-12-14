#include "ParticleBuoyancy.h"

ParticleBuoyancy::ParticleBuoyancy(Vector3 pos, float _maxDepth, float _volume, float _waterHeight, float _liquidDensity)
	: maxDepth(_maxDepth), volume(_volume), waterHeight(_waterHeight), liquidDensity(_liquidDensity) {
	waterCeilingPos = physx::PxTransform(pos.x, waterHeight, pos.z);
	waterCeiling = new RenderItem(CreateShape(physx::PxBoxGeometry(20, 0.01, 150)), &waterCeilingPos, { 0, 1, 1, 1 });
}

void ParticleBuoyancy::updateForce(Particle* p, float t) {
	float depth = p->getPos().y;
	Vector3 f(0.0f, 0.0f, 0.0f);

	if (depth > waterHeight + maxDepth + 0.01) {
		p->setColor({ 0.5, 0.5, 0.5, 1 });
		return;
	}
	if (depth < waterHeight - maxDepth) {
		p->setColor({ 0, 0, 0, 1 });
		f.y = liquidDensity * volume * gFactor;
	}
	else {
		float depthExt = waterHeight + maxDepth;
		float volFactor = (depthExt - depth) / (2 * maxDepth);
		f.y = liquidDensity * volume * volFactor * gFactor;
		p->setColor({ 0.1, 0.1, 0.1, 1 });
	}

	p->addForce(f);
}