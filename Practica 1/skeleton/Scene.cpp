#include "Scene.h"

Scene::Scene() {
	init();
}

Scene::~Scene() {
	shutdown();
}

void Scene::addParticle(Vector3 position, Vector3 velocity, Vector3 acceleration, Vector4 color) {
	particles.push_back(new Particle(position, velocity, acceleration, color));
}

void Scene::update(double t) {
	for (int i = 0; i < particles.size(); i++) {
		if (particles[i]->update(t)) {
			delete particles[i];
			particles.erase(particles.begin() + i);
		}
	}
}

void Scene::restart() {
	shutdown();
	init();
}

void Scene::init() {
	// here comes more objects if I wanted to
}

void Scene::shutdown() {
	for (Particle* p : particles)
		delete p;
}
