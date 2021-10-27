#ifndef FORCES_H
#define FORCES_H

#include "ParticleForceGenerator.h"
#include "GravityForceGenerator.h"
#include "DragForceGenerator.h"

class ForceGenerators {
public:
	ForceGenerators();
	~ForceGenerators();

	GravityForceGenerator* gravity;
	DragForceGenerator* drag;
};

#endif