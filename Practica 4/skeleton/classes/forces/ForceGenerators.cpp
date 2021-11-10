#include "ForceGenerators.h"

#include "ParticleForceRegistry.h"

ForceGenerators::ForceGenerators(Camera* cam) : expT(-1), expD(1.5) {
	gravity = new GravityForceGenerator({ 0, -9.8, 0 });

	drag = new DragForceGenerator(0.5, 1.5);

	wind = new WindFieldForceGenerator(cam->getDir() * 100 + Vector3(-150, 150, -150), Vector3(300, 300, 0), 60);

	explosion = new ExplosionForceGenerator(cam->getDir() * 100, 1000, 60, 5);

	bHole = new BlackHoleForceGenerator(cam->getDir() * 100 + Vector3(-150, 150, 150), 800, 120);
}

ForceGenerators::~ForceGenerators() {
	delete gravity;
	delete drag;
	delete wind;
	delete explosion;
	delete bHole;
}

void ForceGenerators::addTime(ParticleForceRegistry* fReg, double t) {
	if (expT == -1) return;

	expT += t;
	if (expT > expD) {
		fReg->clearForceLinks(explosion);
		expT = -1;
	}
}