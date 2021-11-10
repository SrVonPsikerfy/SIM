#include "SceneManager.h"

SceneManager::SceneManager(Camera* cam) {
	fReg = new ParticleForceRegistry();

	camera = cam;

	currScene = Scenes::DEFAULT;
	defaultScene();
}

SceneManager::~SceneManager() {
	free();
}

void SceneManager::handleInput(unsigned char key)
{
	ParticleData pData;

	switch (toupper(key)) {
	case '1': case '2': case '3': case '4': case '5': case '6': {
		changeScene((Scenes)((int)key - '0' - 1));
		break;
	}
	case ' ': {
		//offset iniSpeed acceleration damp invmass size deathTime prog color
		pData = { { 0, 0, 0 }, camera->getDir() * 200, { 0, 0, 0 }, 0.999, 1, 3, 6, true, { 1, 1, 1, 1 } };
		pSys[0]->generateBullet(camera->getTransform().p, pData);
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
	case SceneManager::Scenes::SPRINGS:
		springScene();
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

void SceneManager::springScene() {
	//Particle p1, p2;
	//ParticleForceRegistry registry;

	//ParticleSpring ps1(&p2, 2.0f, 3.5f);
	//registry.add(&p1, ps1);

	//ParticleSpring ps2(&p1, 2.0f, 3.5f);
	//registry.add(&p2, ps2);
}
