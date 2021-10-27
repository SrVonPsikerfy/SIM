#include "ForceGenerators.h"

ForceGenerators::ForceGenerators() {
	gravity = new GravityForceGenerator({ 0, -9.8, 0 });
	drag = new DragForceGenerator(0.5, 1.5);
}

ForceGenerators::~ForceGenerators() {
	delete gravity;
	delete drag;
}