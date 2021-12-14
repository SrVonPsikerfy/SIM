#include "ParticleSystem.h"

#include "../../utils/maths.h"

ParticleSystem::~ParticleSystem() {
	reset();
}

void ParticleSystem::update(double t) {
	for (int i = particles.size() - 1; i >= 0; i--) {
		particles[i]->update(t);
		if (particles[i]->dead())
			onParticleDeath(i);
	}

	if (spawnTime != -1)
		spawnParticle(t);
}

void ParticleSystem::reset() {
	for (int i = 0; i < particles.size(); i++) {
		fReg->clearParticleLinks(particles[i]);
		delete particles[i];
	}
	particles.clear();

	spType = SpawnType::NONE;
	spawnTime = -1;
}

void ParticleSystem::generateBullet(Vector3 pos, ParticleData data) {
	particles.push_back(new Particle(pos, data));
	addForceLinks();
}

void ParticleSystem::spawn(double spawn,SpawnType type) {
	spType = type;
	spawnTime = spawn;
}

void ParticleSystem::spawnOnSphere(Vector3 pos, double radius, int num) {
	for (size_t i = 0; i < num; i++) {
		Particle* p = new Particle();
		p->setDeathTime(20);

		Vector3 dir = Vector3(maths::random<double>(-1, 1), 
			maths::random<double>(-1, 1), maths::random<double>(-1, 1)).getNormalized();
		double amount = maths::random<double>(-radius, radius);
		p->setPosition(pos + dir * amount);

		particles.push_back(p);
		for (size_t i = 1; i < fgs.size(); i++)
			fReg->add(particles[particles.size() - 1], fgs[i]);
	}
}

void ParticleSystem::addParticle(Particle* newP) {
	particles.push_back(newP);
	addForceLinks();
}

void ParticleSystem::addForceGenerator(ParticleForceGenerator* fg) {
	fgs.push_back(fg);
}

void ParticleSystem::applyForceGenerator(ParticleForceGenerator* fg) {
	for (int i = 0; i < particles.size(); i++)
		fReg->add(particles[i], fg);
}

void ParticleSystem::onParticleDeath(int particle) {
	releaseParticle(particle);
}

void ParticleSystem::spawnParticle(double t) {
	nextSpawn += t;
	if (nextSpawn >= spawnTime) {
		switch (spType) {
		case SpawnType::FOUNTAIN:
			generateFountainParticle();
			break;
		case SpawnType::ERUPTION:
			generateEruptionParticle();
			break;
		case SpawnType::RIVER:
			generateRiverParticle();
			break;
		case SpawnType::NONE:
			nextSpawn = 0;
			break;
		}
	}
}

void ParticleSystem::generateFountainParticle() {
	ParticleData pData;
	int vel = 10;
	float x = (-2 + (rand() % 7)), y = (rand() % 8), z = (-2 + (rand() % 7)), blue = (((float)rand()) / RAND_MAX);

	pData.offset = { 0, 0, 0 }; pData.initialSpeed = { vel * x, vel * y, vel * z };
	pData.acceleration = { 0, 0, 0 };

	pData.damp = 1;	pData.inv_mass = 1;
	pData.size = 2;	pData.lifeTime = rand() % 2 + 2;
	pData.progThroughTime = true; pData.color = { 0, 0, blue, 1 };

	particles.push_back(new Particle(posSystem, pData));
	addForceLinks();

	spawnTime = ((float)rand() / RAND_MAX) / 16;
	nextSpawn = 0;
}

void ParticleSystem::generateEruptionParticle()
{
	ParticleData pData;
	int vel = 10;
	float x = (-2 + (rand() % 7)), y = 2 + (rand() % 8), z = (-2 + (rand() % 7)), red = (((float)rand()) / RAND_MAX);

	pData.offset = { 0, 0, 0 }; pData.initialSpeed = { vel * x, vel * y, vel * z };
	pData.acceleration = { 0, 0, 0 };

	pData.damp = 1;	pData.inv_mass = 1;
	pData.size = 2;	pData.lifeTime = rand() % 2 + 2;
	pData.progThroughTime = true; pData.color = { red, 0, 0, 1 };

	particles.push_back(new Particle(posSystem, pData));
	addForceLinks();

	spawnTime = ((float)rand() / RAND_MAX) / 16;
	nextSpawn = 0;
}

void ParticleSystem::generateRiverParticle()
{
	ParticleData pData;
	float x = maths::random<double>(-15, 15), z = maths::random<double>(-90, 90);

	pData.offset = { x, 0, z }; pData.initialSpeed = { 0, 0, 0 };
	pData.acceleration = { 0, 0, 0 };

	pData.damp = 1;	pData.inv_mass = 1 / (maths::random<double>(400, 600));
	pData.size = 1;	pData.lifeTime = maths::random<double>(6, 10);
	pData.progThroughTime = false; pData.color = { 1, 1, 1, 1 };

	particles.push_back(new Particle(posSystem, pData, Shape::CUBE));
	addForceLinks();
	
	nextSpawn = 0;
}

void ParticleSystem::addForceLinks() {
	for (auto fg : fgs)
		fReg->add(particles[particles.size() - 1], fg);
}

void ParticleSystem::releaseParticle(int numParticle) {
	fReg->clearParticleLinks(particles[numParticle]);

	delete particles[numParticle];
	particles.erase(particles.begin() + numParticle);
}