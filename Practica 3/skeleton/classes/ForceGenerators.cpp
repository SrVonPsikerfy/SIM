#include "ForceGenerators.h"

#include "ParticleForceRegistry.h"

ForceGenerators::ForceGenerators(Camera* cam) : expT(-1), expD(1.5) {
	gravity = new GravityForceGenerator({ 0, -9.8, 0 });
	drag = new DragForceGenerator(0.5, 1.5);
	wind = new WindFieldForceGenerator({ 300, 300, 0 }, cam->getDir() * 100 + Vector3(-150, 150, -150), 60);
	explosion = new ExplosionForceGenerator(cam->getDir() * 100, 1000, 60);
}

ForceGenerators::~ForceGenerators() {
	delete gravity;
	delete drag;
	delete wind;
	delete explosion;
}

void ForceGenerators::addTime(ParticleForceRegistry* fReg, double t) {
	if (expT == -1) return;

	expT += t;
	if (expT > expD) {
		fReg->clearForceLinks(explosion);
		expT = -1;
	}
}