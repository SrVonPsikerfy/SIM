#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

#include "core.hpp"
#include "Particle.h"

class Scene
{
public:
	Scene();
	~Scene();
	void addParticle(Vector3 position, Vector3 velocity, Vector3 acceleration, Vector4 color);
	void update(double t);
	void restart();

private:
	std::vector<Particle*> particles;

	void init();
	void shutdown();
};

#endif