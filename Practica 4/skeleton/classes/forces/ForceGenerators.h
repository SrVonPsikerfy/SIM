#ifndef FORCES_H
#define FORCES_H

#include "ParticleForceGenerator.h"

#include "GravityForceGenerator.h"
#include "DragForceGenerator.h"
#include "WindFieldForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "BlackHoleForceGenerator.h"

#include "../../render/Camera.h"

class ParticleForceRegistry;

class ForceGenerators {
public:
	ForceGenerators(Camera* cam);
	~ForceGenerators();

	void addTime(ParticleForceRegistry* fReg, double t);
	void startTime() { if (expT == -1) expT = 0; }

	GravityForceGenerator* gravity;
	DragForceGenerator* drag;
	WindFieldForceGenerator* wind;
	ExplosionForceGenerator* explosion;
	BlackHoleForceGenerator* bHole;

private:
	double expT, expD;
};
#endif