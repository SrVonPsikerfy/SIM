#ifndef FORCES_H
#define FORCES_H

#include "ParticleForceGenerator.h"
#include "GravityForceGenerator.h"
#include "DragForceGenerator.h"

class ForceGenerators {
public:
	static GravityForceGenerator* gravity;

	static DragForceGenerator* drag;
};

#endif