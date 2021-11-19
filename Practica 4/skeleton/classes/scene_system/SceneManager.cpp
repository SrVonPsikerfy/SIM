#include "SceneManager.h"

#include "../../utils/maths.h"

SceneManager::SceneManager(Camera* cam) {
	fReg = new ParticleForceRegistry();

	camera = cam;

	axisPos = physx::PxTransform(0, 0, 0);
	axis = new RenderItem(CreateShape(physx::PxSphereGeometry(0.5)), &axisPos, { 1, 0, 0, 1 });
	RegisterRenderItem(axis);

	currScene = Scenes::DEFAULT;
	defaultScene();
}

SceneManager::~SceneManager() {
	free();

	if (axis != nullptr)
		axis->release();
}

void SceneManager::handleInput(unsigned char key)
{
	ParticleData pData;

	switch (toupper(key)) {
	case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': {
		changeScene((Scenes)((int)key - '0' - 1));
		break;
	}
	case '+': {
		if (currScene != Scenes::ANCHORED_SPRING || currScene != Scenes::SPRING)
			return;

		((SpringForceGenerator*)forces[2])->addElasticity(0.1f);
		if (currScene == Scenes::SPRING)
			((SpringForceGenerator*)forces[3])->addElasticity(0.1f);
		break;
	}
	case '-': {
		if (currScene != Scenes::ANCHORED_SPRING || currScene != Scenes::SPRING)
			return;

		((SpringForceGenerator*)forces[2])->addElasticity(-0.1f);
		if (currScene == Scenes::SPRING)
			((SpringForceGenerator*)forces[3])->addElasticity(0.1f);
		break;
	}
	case ' ': {
		if (currScene < Scenes::ANCHORED_SPRING) {
			//offset iniSpeed acceleration damp invmass size deathTime prog color
			pData = { { 0, 0, 0 }, camera->getDir() * 200, { 0, 0, 0 }, 0.999, 1, 3, 6, true, { 1, 1, 1, 1 } };
			pSys[0]->generateBullet(camera->getTransform().p, pData);
		}
		else if (currScene == Scenes::ANCHORED_SPRING || currScene == Scenes::SPRING)
			((ExplosionForceGenerator*)forces[1])->activateExplosion();
		break;
	}
	case 'X': {
		if (currScene != Scenes::PARTICLE_SYSTEM)
			break;

		double time = (((float)rand()) / RAND_MAX) / 16;
		pSys[1]->spawnFountain(time);
		break;
	}
	case 'F': {
		if (currScene != Scenes::FIREWORK_SYSTEM)
			break;

		pData = { { 0, 0, 0 }, { 0, 30, 0 } , { 0, 0, 0 }, 0.999, 1, 2, 3, true, { 1, 1, 1, 1 } };
		((FireworkSystem*)pSys[1])->generateFirework(FireworkLoadType::FLOWER, 360, 1, pData);
		break;
	}
	case 'P': {
		if (currScene != Scenes::FIREWORK_SYSTEM)
			break;

		pData = { { 0, 0, 0 }, { 0, 30, 0 } , { 0, 0, 0 }, 0.999, 1, 2, 3, true, { 0, 1, 0, 1 } };
		((FireworkSystem*)pSys[1])->generateFirework(FireworkLoadType::SPHERE, 40, 1, pData);
		break;
	}
	case 'O': {
		if (currScene != Scenes::FIREWORK_SYSTEM)
			break;

		pData = { { 0, 0, 0 }, { 0, 30, 0 } , { 0, 0, 0 }, 0.999, 1, 2, 3, true, { 0, 0, 1, 1 } };
		((FireworkSystem*)pSys[1])->generateFirework(FireworkLoadType::RANDOM, 30, 2, pData);
		break;
	}
	case 'R': {
		for (auto pS : pSys)
			pS->reset();
		break;
	}
	case 'C': {
		if (currScene != Scenes::FORCES)
			break;

		((WindFieldForceGenerator*)forces[3])->switchWindType(3);
		break;
	}
	case 'E': {
		if (currScene != Scenes::FORCES)
			break;

		((ExplosionForceGenerator*)forces[1])->activateExplosion();
		pSys[1]->applyForceGenerator(forces[0]);
		break;
	}
	case 'G': {
		if (currScene != Scenes::FORCES)
			break;

		pSys[0]->spawnOnSphere(((SphericalForceField*)forces[1])->getPos(), 55, 50);
		if (forces.size() > 2) // forcesScene()
			pSys[0]->spawnOnSphere(((SphericalForceField*)forces[2])->getPos(), 55, 50);
		break;
	}
	case 'Z': {
		if (currScene != Scenes::BUOYANCY)
			break;

		ParticleData data = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, 0.7, 1 / maths::random<float>(5000.0, 8000.0), 2, 10000, false, { 1, 0, 1, 0 } };
		Particle* newP = new Particle({ maths::random<float>(-15, 15), maths::random<float>(10, 40), maths::random<float>(-15, 15) },
			data, Shape::CUBE);

		pSys[0]->addParticle(newP);
		break;
	}
	case 'H': {
		if (currScene != Scenes::BUNGEE)
			break;

		Vector3 vel = pSys[0]->getParticles()[0]->getVelocity();
		pSys[0]->getParticles()[0]->setVelocity(Vector3(-vel.x, vel.y, vel.z));
		break;
	}
	default:
		break;
	}
}

void SceneManager::update(double time) {
	fReg->updateForces(time);

	for (ParticleForceGenerator* fg : forces)
		if (fg->isActive())
			fg->update(time);

	for (ParticleSystem* ps : pSys)
		ps->update(time);
}

void SceneManager::free() {
	fReg->clear();

	for (size_t i = 0; i < forces.size(); i++)
		delete forces[i];
	forces.clear();

	for (size_t j = 0; j < pSys.size(); j++)
		delete pSys[j];
	pSys.clear();
}

void SceneManager::changeScene(Scenes newScene)
{
	free();
	currScene = newScene;

	switch (currScene)
	{
	case SceneManager::Scenes::DEFAULT:
		defaultScene();
		break;
	case SceneManager::Scenes::PARTICLE_SYSTEM:
		partSysScene();
		break;
	case SceneManager::Scenes::FIREWORK_SYSTEM:
		fireSysScene();
		break;
	case SceneManager::Scenes::FORCE:
		windScene();
		break;
	case SceneManager::Scenes::FORCES:
		forcesScene();
		break;
	case SceneManager::Scenes::ANCHORED_SPRING:
		anchoredSpringScene();
		break;
	case SceneManager::Scenes::SPRING:
		springScene();
		break;
	case SceneManager::Scenes::BUOYANCY:
		buoyancyScene();
		break;
	case SceneManager::Scenes::BUNGEE:
		bungeeScene();
		break;
	default:
		break;
	}
}

void SceneManager::defaultScene() {
	forces.push_back(new GravityForceGenerator({ 0, -10, 0 }));

	ParticleSystem* bulletPS = new ParticleSystem(fReg);
	bulletPS->addForceGenerator(forces[0]);
	pSys.push_back(bulletPS);
}

void SceneManager::partSysScene() {
	forces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	forces.push_back(new GravityForceGenerator({ 0, -50, 0 }));

	ParticleSystem* bulletPS = new ParticleSystem(fReg);
	bulletPS->addForceGenerator(forces[0]);
	pSys.push_back(bulletPS);

	ParticleSystem* fountainPS = new ParticleSystem(fReg);
	fountainPS->addForceGenerator(forces[1]);
	pSys.push_back(fountainPS);
}

void SceneManager::fireSysScene() {
	forces.push_back(new GravityForceGenerator({ 0, -10, 0 }));

	ParticleSystem* bulletPS = new ParticleSystem(fReg);
	bulletPS->addForceGenerator(forces[0]);
	pSys.push_back(bulletPS);

	FireworkSystem* fireworkPS = new FireworkSystem(fReg);
	fireworkPS->addForceGenerator(forces[0]);
	pSys.push_back(fireworkPS);
}

void SceneManager::forcesScene() {
	forces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	forces.push_back(new ExplosionForceGenerator(camera->getDir() * 100, 1000, 60, 0.5)); forces[forces.size() - 1]->setActive(false);
	forces.push_back(new BlackHoleForceGenerator(camera->getDir() * 100 + Vector3(-150, 150, 150), 800, 120));
	forces.push_back(new WindFieldForceGenerator(camera->getDir() * 100 + Vector3(-150, 150, -150), Vector3(300, 300, 0), 60));

	ParticleSystem* bulletPS = new ParticleSystem(fReg);
	bulletPS->addForceGenerator(forces[0]); bulletPS->addForceGenerator(forces[1]);
	bulletPS->addForceGenerator(forces[2]); bulletPS->addForceGenerator(forces[3]);
	pSys.push_back(bulletPS);

	ParticleSystem* spawnPS = new ParticleSystem(fReg);
	spawnPS->addForceGenerator(forces[1]); spawnPS->addForceGenerator(forces[2]);
	spawnPS->addForceGenerator(forces[3]);
	pSys.push_back(spawnPS);
}

void SceneManager::windScene() {
	forces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	forces.push_back(new WindFieldForceGenerator(camera->getDir() * 100, Vector3(300, 300, 0), 60));

	ParticleSystem* bulletPS = new ParticleSystem(fReg);
	bulletPS->addForceGenerator(forces[0]);
	bulletPS->addForceGenerator(forces[1]);
	pSys.push_back(bulletPS);
}

void SceneManager::explosionScene() {
	forces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	forces.push_back(new ExplosionForceGenerator(camera->getDir() * 100, 1000, 60, 0.5));
	forces[forces.size() - 1]->setActive(false);

	ParticleSystem* bulletPS = new ParticleSystem(fReg);
	bulletPS->addForceGenerator(forces[0]);
	bulletPS->addForceGenerator(forces[1]);
	pSys.push_back(bulletPS);

	ParticleSystem* spawnPS = new ParticleSystem(fReg);
	spawnPS->addForceGenerator(forces[1]);
	pSys.push_back(spawnPS);
}

void SceneManager::blackHoleScene() {
	forces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	forces.push_back(new BlackHoleForceGenerator(camera->getDir() * 100, 800, 120));

	ParticleSystem* bulletPS = new ParticleSystem(fReg);
	bulletPS->addForceGenerator(forces[0]);
	bulletPS->addForceGenerator(forces[1]);
	pSys.push_back(bulletPS);

	ParticleSystem* spawnPS = new ParticleSystem(fReg);
	spawnPS->addForceGenerator(forces[0]);
	spawnPS->addForceGenerator(forces[1]);
	pSys.push_back(spawnPS);
}

void SceneManager::anchoredSpringScene() {
	forces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	forces.push_back(new ExplosionForceGenerator({ 0, 0, 0 }, 1000, 30, 0.5));
	forces[1]->setActive(false);

	ParticleSystem* springsPS = new ParticleSystem(fReg);
	springsPS->addForceGenerator(forces[0]);
	springsPS->addForceGenerator(forces[1]);
	pSys.push_back(springsPS);

	ParticleData data = { { 0, 0, 0 }, { 0, 0, 0 } , { 0, 0, 0 }, 0.7, 1, 2, 10000, false, { 1, 1, 1, 1 } };
	Particle* newP = new Particle({ 0, 1, 0 }, data);
	forces.push_back(new ParticleAnchoredSpring(Vector3(0, 0, 0), 2.0f, 3.5f));

	fReg->add(newP, forces[2]);
	springsPS->addParticle(newP);
}

void SceneManager::springScene() {
	forces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	forces.push_back(new ExplosionForceGenerator({ 0, 0, 0 }, 1000, 30, 0.5));
	forces[1]->setActive(false);

	ParticleSystem* springsPS = new ParticleSystem(fReg);
	springsPS->addForceGenerator(forces[0]);
	springsPS->addForceGenerator(forces[1]);
	pSys.push_back(springsPS);

	ParticleData data = { { 0, 0, 0 }, { 0, 0, 0 } , { 0, 0, 0 }, 0.7, 1, 2, 10000, false, { 0, 1, 0, 1 } };
	Particle* newP = new Particle({ 10, 30, 0 }, data);
	ParticleData data2 = { { 0, 0, 0 }, { 0, 0, 0 } , { 0, 0, 0 }, 0.7, 1, 2, 10000, false, { 0, 0, 1, 1 } };
	Particle* newP2 = new Particle({ -10, 30, 0 }, data2);

	forces.push_back(new ParticleSpring(newP2, 2.0f, 3.5f));
	forces.push_back(new ParticleSpring(newP, 3.0f, 3.5f));

	fReg->add(newP, forces[2]);
	fReg->add(newP2, forces[3]);
	springsPS->addParticle(newP);
	springsPS->addParticle(newP2);
}

void SceneManager::buoyancyScene() {
	forces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	forces.push_back(new ParticleBuoyancy(Vector3(0, 0, 0), 1.0f, 8.0, 40.0f));

	ParticleData data = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, 0.7, 1 / 6000.0, 2, 10000, false, { 1, 0, 1, 0 } };
	Particle* newP = new Particle({ 0, 35, 0 }, data, Shape::CUBE);

	ParticleSystem* buoyancyPS = new ParticleSystem(fReg);
	buoyancyPS->addForceGenerator(forces[0]);
	buoyancyPS->addForceGenerator(forces[1]);
	pSys.push_back(buoyancyPS);

	buoyancyPS->addParticle(newP);
}

void SceneManager::bungeeScene() {
	ParticleData data = { { 0, 0, 0 }, { 10, 0, 0 } , { 0, 0, 0 }, 1, 1, 2, 10000, false, { 1, 1, 0, 1 } };
	Particle* newP = new Particle({ 0, 0, 0 }, data);
	ParticleData data2 = { { 0, 0, 0 }, { 0, 0, 0 } , { 0, 0, 0 }, 0.7, 1, 2, 10000, false, { 0, 1, 1, 1 } };
	Particle* newP2 = new Particle({ 30, 30, 0 }, data2);

	forces.push_back(new ParticleBungee(newP, 2.0f, 15.0f));
	fReg->add(newP2, forces[0]);

	ParticleSystem* bungeePS = new ParticleSystem(fReg);
	bungeePS->addForceGenerator(forces[0]);
	pSys.push_back(bungeePS);

	bungeePS->addParticle(newP);
	bungeePS->addParticle(newP2);
}

