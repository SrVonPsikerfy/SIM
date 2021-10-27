#include "ForceGenerators.h"

GravityForceGenerator* ForceGenerators::gravity = new GravityForceGenerator({ 0,-9.8,0 });

DragForceGenerator* ForceGenerators::drag = new DragForceGenerator(0.5, 1.5);