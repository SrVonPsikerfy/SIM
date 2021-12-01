#include "SceneManager.h"

#include "../../utils/maths.h"

SceneManager::SceneManager(PxPhysics* gPhys, PxScene* gSc, PxMaterial* gMat, Camera* cam) {

	gPhysics = gPhys;
	gScene = gSc;
	gMaterial = gMat;

	camera = cam;

	// ------------------------------------------------------------

	gScene->setGravity(PxVec3(0.0f, -9.8f, 0.0f));

	// ------------------------------------------------------------

	fReg = new ForceRegistry();

	axisPos = physx::PxTransform(0, 0, 0);
	axis = new RenderItem(CreateShape(physx::PxSphereGeometry(2)), &axisPos, { 1, 0, 0, 1 });

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
		if (currScene != Scenes::SPRING && currScene != Scenes::RIGID_SOLID)
			return;

		if (currScene == Scenes::SPRING) {
			((SpringForceGenerator*)pForces[2])->addElasticity(0.1f);
			((SpringForceGenerator*)pForces[3])->addElasticity(0.1f);
		}
		else {
			auto newRes = gMaterial->getRestitution() + 0.01 > 1 ? 1 : gMaterial->getRestitution() + 0.01;
			gMaterial->setRestitution(newRes);
		}

		break;
	}
	case '-': {
		if (currScene != Scenes::SPRING && currScene != Scenes::RIGID_SOLID)
			return;

		if (currScene == Scenes::SPRING) {
			((SpringForceGenerator*)pForces[2])->addElasticity(-0.1f);
			((SpringForceGenerator*)pForces[3])->addElasticity(0.1f);
		}
		else {
			auto newRes = gMaterial->getRestitution() - 0.01 < 0 ? 0 : gMaterial->getRestitution() - 0.01;
			gMaterial->setRestitution(newRes);
		}

		break;
	}
	case ' ': {
		if (currScene != Scenes::SPRING && currScene != Scenes::RIGID_SOLID) {
			//offset iniSpeed acceleration damp invmass size deathTime prog color
			pData = { { 0, 0, 0 }, camera->getDir() * 200, { 0, 0, 0 }, 0.999, 1, 3, 6, true, { 1, 1, 1, 1 } };
			pSys[0]->generateBullet(camera->getTransform().p, pData);
		}
		else if (currScene == Scenes::SPRING)
			((ExplosionForceGenerator*)pForces[1])->activateExplosion();
		else
			((ExplosionRigidForceGenerator*)rbForces[rbForces.size() - 1])->activateExplosion();
		break;
	}
	case 'X': {
		if (currScene != Scenes::PARTICLE_SYSTEM && currScene != Scenes::RIGID_SOLID)
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

		((WindFieldForceGenerator*)pForces[3])->switchWindType(3);
		break;
	}
	case 'E': {
		if (currScene != Scenes::FORCES)
			break;

		((ExplosionForceGenerator*)pForces[1])->activateExplosion();
		pSys[1]->applyForceGenerator(pForces[0]);
		break;
	}
	case 'G': {
		if (currScene != Scenes::FORCES)
			break;

		pSys[0]->spawnOnSphere(((SphericalForceField*)pForces[1])->getPos(), 55, 50);
		if (pForces.size() > 2) // forcesScene()
			pSys[0]->spawnOnSphere(((SphericalForceField*)pForces[2])->getPos(), 55, 50);
		break;
	}
	case 'Z': {
		if (currScene != Scenes::BUOYANCY)
			break;

		ParticleData data = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, 0.7, 1 / maths::random<float>(5000.0, 7000.0), 2, 10000, false, { 1, 0, 1, 0 } };
		Particle* newP = new Particle({ maths::random<float>(-15, 15), maths::random<float>(10, 40), maths::random<float>(-15, 15) },
			data, Shape::CUBE);

		pSys[0]->addParticle(newP);
		break;
	}
	case 'H': {
		if (currScene != Scenes::SPRING)
			break;

		Vector3 vel = pSys[0]->getParticles()[0]->getVelocity();
		pSys[0]->getParticles()[0]->setVelocity(Vector3(-vel.x, vel.y, vel.z));
		break;
	}
	case 'M': {
		if (currScene != Scenes::RIGID_SOLID)
			break;

		pData = { { 0, 0, 0 }, camera->getDir() * 200, { 0, 0, 0 }, 0.999, 1, 3, 6, true, { 1, 1, 1, 1 } };
		pSys[0]->generateBullet(camera->getTransform().p, pData);
		break;
	}
	default:
		break;
	}
}

void SceneManager::update(double time) {
	for (ParticleForceGenerator* fg : pForces)
		if (fg->isActive())
			fg->update(time);

	for (RigidBodyForceGenerator* fg : rbForces)
		if (fg->isActive())
			fg->update(time);

	fReg->updateForces(time);

	for (ParticleSystem* ps : pSys)
		ps->update(time);

	for (RigidBodySystem* rbs : rbSys)
		rbs->update(time);
}

void SceneManager::free() {
	fReg->clear();

	for (size_t i = 0; i < pForces.size(); i++)
		delete pForces[i];
	pForces.clear();

	for (size_t j = 0; j < pSys.size(); j++)
		delete pSys[j];
	pSys.clear();

	for (size_t t = 0; t < rbSys.size(); t++)
		delete rbSys[t];
	rbSys.clear();

	for (size_t l = 0; l < stBodies.size(); l++) {
		gScene->removeActor(*stBodies[l]->staticData);
		stBodies[l]->rItem->release();
		delete stBodies[l];
	}
	stBodies.clear();
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
	case SceneManager::Scenes::SPRING:
		springScene();
		break;
	case SceneManager::Scenes::BUOYANCY:
		buoyancyScene();
		break;
	case SceneManager::Scenes::RIGID_SOLID:
		rigidBodyScene();
		break;
	default:
		break;
	}
}

void SceneManager::defaultScene() {
	pForces.push_back(new GravityForceGenerator({ 0, -10, 0 }));

	ParticleSystem* bulletPS = new ParticleSystem(fReg);
	bulletPS->addForceGenerator(pForces[0]);
	pSys.push_back(bulletPS);
}

void SceneManager::partSysScene() {
	pForces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	pForces.push_back(new GravityForceGenerator({ 0, -50, 0 }));

	ParticleSystem* bulletPS = new ParticleSystem(fReg);
	bulletPS->addForceGenerator(pForces[0]);
	pSys.push_back(bulletPS);

	ParticleSystem* fountainPS = new ParticleSystem(fReg);
	fountainPS->addForceGenerator(pForces[1]);
	pSys.push_back(fountainPS);
}

void SceneManager::fireSysScene() {
	pForces.push_back(new GravityForceGenerator({ 0, -10, 0 }));

	ParticleSystem* bulletPS = new ParticleSystem(fReg);
	bulletPS->addForceGenerator(pForces[0]);
	pSys.push_back(bulletPS);

	FireworkSystem* fireworkPS = new FireworkSystem(fReg);
	fireworkPS->addForceGenerator(pForces[0]);
	pSys.push_back(fireworkPS);
}

void SceneManager::forcesScene() {
	pForces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	pForces.push_back(new ExplosionForceGenerator(camera->getDir() * 100, 1000, 60, 0.5)); pForces[pForces.size() - 1]->setActive(false);
	pForces.push_back(new BlackHoleForceGenerator(camera->getDir() * 100 + Vector3(-150, 150, 150), 800, 120));
	pForces.push_back(new WindFieldForceGenerator(camera->getDir() * 100 + Vector3(-150, 150, -150), Vector3(300, 300, 0), 60));

	ParticleSystem* bulletPS = new ParticleSystem(fReg);
	bulletPS->addForceGenerator(pForces[0]); bulletPS->addForceGenerator(pForces[1]);
	bulletPS->addForceGenerator(pForces[2]); bulletPS->addForceGenerator(pForces[3]);
	pSys.push_back(bulletPS);

	ParticleSystem* spawnPS = new ParticleSystem(fReg);
	spawnPS->addForceGenerator(pForces[1]); spawnPS->addForceGenerator(pForces[2]);
	spawnPS->addForceGenerator(pForces[3]);
	pSys.push_back(spawnPS);
}

void SceneManager::windScene() {
	pForces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	pForces.push_back(new WindFieldForceGenerator(camera->getDir() * 100, Vector3(300, 300, 0), 60));

	ParticleSystem* bulletPS = new ParticleSystem(fReg);
	bulletPS->addForceGenerator(pForces[0]);
	bulletPS->addForceGenerator(pForces[1]);
	pSys.push_back(bulletPS);
}

void SceneManager::explosionScene() {
	pForces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	pForces.push_back(new ExplosionForceGenerator(camera->getDir() * 100, 1000, 60, 0.5));
	pForces[pForces.size() - 1]->setActive(false);

	ParticleSystem* bulletPS = new ParticleSystem(fReg);
	bulletPS->addForceGenerator(pForces[0]);
	bulletPS->addForceGenerator(pForces[1]);
	pSys.push_back(bulletPS);

	ParticleSystem* spawnPS = new ParticleSystem(fReg);
	spawnPS->addForceGenerator(pForces[1]);
	pSys.push_back(spawnPS);
}

void SceneManager::blackHoleScene() {
	pForces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	pForces.push_back(new BlackHoleForceGenerator(camera->getDir() * 100, 800, 120));

	ParticleSystem* bulletPS = new ParticleSystem(fReg);
	bulletPS->addForceGenerator(pForces[0]);
	bulletPS->addForceGenerator(pForces[1]);
	pSys.push_back(bulletPS);

	ParticleSystem* spawnPS = new ParticleSystem(fReg);
	spawnPS->addForceGenerator(pForces[0]);
	spawnPS->addForceGenerator(pForces[1]);
	pSys.push_back(spawnPS);
}

void SceneManager::anchoredSpringScene() {
	pForces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	pForces.push_back(new ExplosionForceGenerator({ 0, 0, 0 }, 1000, 30, 0.5));
	pForces[1]->setActive(false);

	ParticleSystem* springsPS = new ParticleSystem(fReg);
	springsPS->addForceGenerator(pForces[0]);
	springsPS->addForceGenerator(pForces[1]);
	pSys.push_back(springsPS);

	ParticleData data = { { 0, 0, 0 }, { 0, 0, 0 } , { 0, 0, 0 }, 0.7, 1, 2, 10000, false, { 1, 1, 1, 1 } };
	Particle* newP = new Particle({ 0, 1, 0 }, data);
	pForces.push_back(new ParticleAnchoredSpring(Vector3(0, 0, 0), 2.0f, 3.5f));

	fReg->add(newP, pForces[2]);
	springsPS->addParticle(newP);
}

void SceneManager::springScene() {
	pForces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	pForces.push_back(new ExplosionForceGenerator({ 0, 0, 0 }, 1000, 30, 0.5));
	pForces[1]->setActive(false);

	ParticleSystem* springsPS = new ParticleSystem(fReg);
	springsPS->addForceGenerator(pForces[0]);
	springsPS->addForceGenerator(pForces[1]);
	pSys.push_back(springsPS);

	ParticleData data = { { 0, 0, 0 }, { 0, 0, 0 } , { 0, 0, 0 }, 0.7, 1, 2, 10000, false, { 0, 1, 0, 1 } };
	Particle* newP = new Particle({ 10, 30, 0 }, data);
	ParticleData data2 = { { 0, 0, 0 }, { 0, 0, 0 } , { 0, 0, 0 }, 0.7, 1, 2, 10000, false, { 0, 0, 1, 1 } };
	Particle* newP2 = new Particle({ -10, 30, 0 }, data2);

	pForces.push_back(new ParticleSpring(newP2, 2.0f, 3.5f));
	pForces.push_back(new ParticleSpring(newP, 3.0f, 3.5f));

	fReg->add(newP, pForces[2]);
	fReg->add(newP2, pForces[3]);
	springsPS->addParticle(newP);
	springsPS->addParticle(newP2);
}

void SceneManager::buoyancyScene() {
	pForces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	pForces.push_back(new ParticleBuoyancy(Vector3(0, 0, 0), 1.0f, 8.0, 40.0f));

	ParticleData data = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, 0.7, 1 / 300.0, 2, 10000, false, { 1, 0, 1, 0 } };
	Particle* newP = new Particle({ 0, 35, 0 }, data, Shape::CUBE);

	ParticleSystem* buoyancyPS = new ParticleSystem(fReg);
	buoyancyPS->addForceGenerator(pForces[0]);
	buoyancyPS->addForceGenerator(pForces[1]);
	pSys.push_back(buoyancyPS);

	buoyancyPS->addParticle(newP);
}

void SceneManager::bungeeScene() {
	ParticleData data = { { 0, 0, 0 }, { 10, 0, 0 } , { 0, 0, 0 }, 1, 1, 2, 10000, false, { 1, 1, 0, 1 } };
	Particle* newP = new Particle({ 0, 0, 0 }, data);
	ParticleData data2 = { { 0, 0, 0 }, { 0, 0, 0 } , { 0, 0, 0 }, 0.7, 1, 2, 10000, false, { 0, 1, 1, 1 } };
	Particle* newP2 = new Particle({ 30, 30, 0 }, data2);

	pForces.push_back(new ParticleBungee(newP, 2.0f, 15.0f));
	fReg->add(newP2, pForces[0]);

	ParticleSystem* bungeePS = new ParticleSystem(fReg);
	bungeePS->addForceGenerator(pForces[0]);
	pSys.push_back(bungeePS);

	bungeePS->addParticle(newP);
	bungeePS->addParticle(newP2);
}

void SceneManager::rigidBodyScene()
{
	partSysScene();

	// ---------------------- FORCES -----------------------------------
	//rbForces.push_back(new WindRigidForceGenerator({ 0, 0, 0 }, Vector3(300, 300, 0), 30));
	rbForces.push_back(new TorqueRigidForceGenerator({ 0, 0, 0 }, Vector3(300, 300, 0), 40));
	rbForces.push_back(new ExplosionRigidForceGenerator({ 0, 0, 0 }, 1000, 30, 0.5));
	rbForces[1]->setActive(false);

	// ---------------------- STATIC -----------------------------------
	StaticBody* floor = new StaticBody;
	PxShape* shape = CreateShape(PxBoxGeometry(100, 1, 100));
	floor->staticData = gPhysics->createRigidStatic({ 0, 0, 0 });
	floor->staticData->attachShape(*shape);
	gScene->addActor(*floor->staticData);
	floor->rItem = new RenderItem(shape, floor->staticData, { 0.6, 0.2, 1, 1 });

	stBodies.push_back(floor);

	// ---------------------- RIGID -----------------------------------

	RigidBodySystem* defaultRBS = new RigidBodySystem(fReg, gPhysics, gScene, PxTransform(Vector3(0, 60, 0)), 50, 0.7);
	defaultRBS->addForceGenerator(rbForces[0]);
	defaultRBS->addForceGenerator(rbForces[1]);
	rbSys.push_back(defaultRBS);
}
