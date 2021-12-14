#include "SceneManager.h"

#include "../../utils/maths.h"

SceneManager::SceneManager(PxPhysics* gPhys, PxScene* gSc, PxMaterial* gMat, Camera* cam) {

	gPhysics = gPhys;
	gScene = gSc;
	gMaterial = gMat;
	gScene->setGravity(PxVec3(0.0f, -9.8f, 0.0f));

	camera = cam;

	fReg = new ForceRegistry();

	rainSpawn = 1; spawnT = 0;

	currScene = Scenes::DEFAULT;
	finalScene();
}

SceneManager::~SceneManager() {
	free();
}

void SceneManager::handleInput(unsigned char key)
{
	ParticleData pData;
	float factor = 40;

	switch (toupper(key)) {
	case '1': {
		changeScene((Scenes)((int)key - '0' - 1));
		break;
	}
	case 'W': {
		Vector3 force = { camera->getDir().x * factor, 0, camera->getDir().z * factor };
		pc->rigidData->setLinearVelocity(force);
		break;
	}
	case 'S': {
		Vector3 force = { -camera->getDir().x * factor, 0, -camera->getDir().z * factor };
		pc->rigidData->setLinearVelocity(force);
		break;
	}
	case 'A': {
		Vector3 force = { -camera->getLateralDirection().x * factor, 0, -camera->getLateralDirection().z * factor };
		pc->rigidData->setLinearVelocity(force);
		break;
	}
	case 'D': {
		Vector3 force = { camera->getLateralDirection().x * factor, 0, camera->getLateralDirection().z * factor };
		pc->rigidData->setLinearVelocity(force);
		break;
	}
	case ' ': {
		Vector3 force = { 0, factor, 0 };
		pc->rigidData->setLinearVelocity(force);
		break;
	}
	case 'X': {
		double time = (((float)rand()) / RAND_MAX) / 16;
		pSys[0]->spawn(time, SpawnType::ERUPTION);
		break;
	}
	case 'P': {
		if (spawnT > rainSpawn) {
			pData = { { 0, 10, 0 }, { 0, -1, 0 }, { 0, 0, 0 }, 0.7, 1, 1, 0.5, true, { 0, 0.2, 1, 1 } };
			((FireworkSystem*)pSys[pSys.size() - 1])->generateFirework(FireworkLoadType::RAIN, 20, 2, pData);
			pData.offset = { 70, 10, 70 };
			((FireworkSystem*)pSys[pSys.size() - 1])->generateFirework(FireworkLoadType::RAIN, 20, 2, pData);
			pData.offset = { 70, 10, -70 };
			((FireworkSystem*)pSys[pSys.size() - 1])->generateFirework(FireworkLoadType::RAIN, 20, 2, pData);
			pData.offset = { -70, 10, -70 };
			((FireworkSystem*)pSys[pSys.size() - 1])->generateFirework(FireworkLoadType::RAIN, 20, 2, pData);
			pData.offset = { -70, 10, 70 };
			((FireworkSystem*)pSys[pSys.size() - 1])->generateFirework(FireworkLoadType::RAIN, 20, 2, pData);
			spawnT = 0;
		}
		break;
	}
	case 'R': {
		for (auto pS : pSys)
			pS->reset();
		for (int i = 3; i < rbSys.size() - 1; ++i)
			rbSys[i]->reset();
		break;
	}
	case 'E': {
		((DownRigidForceGenerator*)rbForces[0])->activateDown();
		break;
	}
	case 'G': {
		double time = 0.2;
		pSys[1]->spawn(time, SpawnType::RIVER);
		break;
	}
	case 'H': {
		float time = maths::random<double>(0.2, 1.5);
		rbSys[3]->setSpawn(time, SpawnRBType::SNOW);
		rbSys[4]->setSpawn(time, SpawnRBType::SNOW);
		rbSys[5]->setSpawn(time, SpawnRBType::SNOW);
		break;
	}
	default:
		break;
	}
}

void SceneManager::update(double time) {
	spawnT += time;

	if (pc)
		camera->setEye(pc->rigidData->getGlobalPose().p);

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

	for (size_t i = 0; i < rbForces.size(); i++)
		delete rbForces[i];
	rbForces.clear();

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
		finalScene();
		break;
	case SceneManager::Scenes::PARTICLE_SYSTEM:
		break;
	case SceneManager::Scenes::FIREWORK_SYSTEM:
		break;
	case SceneManager::Scenes::FORCE:
		break;
	case SceneManager::Scenes::FORCES:
		break;
	case SceneManager::Scenes::SPRING:
		break;
	case SceneManager::Scenes::BUOYANCY:
		break;
	case SceneManager::Scenes::RIGID_SOLID:
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
	pForces.push_back(new ParticleBuoyancy(Vector3(0, 0, 0), 1.0f, 1.0, 0.0f));

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
	rbForces.push_back(new TorqueRigidForceGenerator(Vector3(300, 300, 0)));
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

void SceneManager::finalScene()
{
	createSkybox();
	createVolcano();
	createEruption();
	createRiver();
	createTree();
	createBananas();
	createSnow();
	createRain();
	createPC();
}

void SceneManager::createSkybox()
{
	float width = 100, depth = 100, height = 60;

	// FLOOR
	StaticBody* floor = new StaticBody;
	PxShape* shape = CreateShape(PxBoxGeometry(width, 1, depth));
	floor->staticData = gPhysics->createRigidStatic({ 0, 0, 0 });
	floor->staticData->attachShape(*shape);
	gScene->addActor(*floor->staticData);
	floor->rItem = new RenderItem(shape, floor->staticData, { 0, 1, 0, 1 });
	stBodies.push_back(floor);

	// CEILING
	StaticBody* ceiling = new StaticBody;
	PxShape* cShape = CreateShape(PxBoxGeometry(width, 1, depth));
	ceiling->staticData = gPhysics->createRigidStatic({ 0, height * 2, 0 });
	ceiling->staticData->attachShape(*cShape);
	gScene->addActor(*ceiling->staticData);
	ceiling->rItem = new RenderItem(cShape, ceiling->staticData, { 1, 1, 1, 1 });
	stBodies.push_back(ceiling);

	// RIGHT
	StaticBody* wall = new StaticBody;
	PxShape* wShape = CreateShape(PxBoxGeometry(1, height, depth));
	wall->staticData = gPhysics->createRigidStatic({ width, height, 0 });
	wall->staticData->attachShape(*wShape);
	gScene->addActor(*wall->staticData);
	wall->rItem = new RenderItem(wShape, wall->staticData, { 0, 0, 0.5, 1 });
	stBodies.push_back(wall);

	// LEFT
	wall = new StaticBody;
	wShape = CreateShape(PxBoxGeometry(1, height, depth));
	wall->staticData = gPhysics->createRigidStatic({ -width, height, 0 });
	wall->staticData->attachShape(*wShape);
	gScene->addActor(*wall->staticData);
	wall->rItem = new RenderItem(wShape, wall->staticData, { 0, 0, 0.5, 1 });
	stBodies.push_back(wall);

	// FRONT
	wall = new StaticBody;
	wShape = CreateShape(PxBoxGeometry(width, height, 1));
	wall->staticData = gPhysics->createRigidStatic({ 0, height, depth });
	wall->staticData->attachShape(*wShape);
	gScene->addActor(*wall->staticData);
	wall->rItem = new RenderItem(wShape, wall->staticData, { 0, 0, 0.5, 1 });
	stBodies.push_back(wall);

	// BACK
	wall = new StaticBody;
	wShape = CreateShape(PxBoxGeometry(width, height, 1));
	wall->staticData = gPhysics->createRigidStatic({ 0, height, -depth });
	wall->staticData->attachShape(*wShape);
	gScene->addActor(*wall->staticData);
	wall->rItem = new RenderItem(wShape, wall->staticData, { 0, 0, 0.5, 1 });
	stBodies.push_back(wall);
}

void SceneManager::createVolcano()
{
	float width = 10, depth = 10;

	// FLOOR
	StaticBody* rock = new StaticBody;
	PxShape* shape = CreateShape(PxBoxGeometry(width, 40, depth));
	rock->staticData = gPhysics->createRigidStatic({ 90, 40, 90 });
	rock->staticData->attachShape(*shape);
	gScene->addActor(*rock->staticData);
	rock->rItem = new RenderItem(shape, rock->staticData, { 0.5, 0.25, 0, 1 });
	stBodies.push_back(rock);

	rock = new StaticBody;
	shape = CreateShape(PxBoxGeometry(width, 25, depth));
	rock->staticData = gPhysics->createRigidStatic({ 70, 25, 90 });
	rock->staticData->attachShape(*shape);
	gScene->addActor(*rock->staticData);
	rock->rItem = new RenderItem(shape, rock->staticData, { 0.5, 0.25, 0, 1 });
	stBodies.push_back(rock);

	rock = new StaticBody;
	shape = CreateShape(PxBoxGeometry(width, 25, depth));
	rock->staticData = gPhysics->createRigidStatic({ 90, 25, 70 });
	rock->staticData->attachShape(*shape);
	gScene->addActor(*rock->staticData);
	rock->rItem = new RenderItem(shape, rock->staticData, { 0.5, 0.25, 0, 1 });
	stBodies.push_back(rock);

	rock = new StaticBody;
	shape = CreateShape(PxBoxGeometry(width, 10, depth));
	rock->staticData = gPhysics->createRigidStatic({ 70, 10, 70 });
	rock->staticData->attachShape(*shape);
	gScene->addActor(*rock->staticData);
	rock->rItem = new RenderItem(shape, rock->staticData, { 0.5, 0.25, 0, 1 });
	stBodies.push_back(rock);

	rock = new StaticBody;
	shape = CreateShape(PxBoxGeometry(width, 10, depth));
	rock->staticData = gPhysics->createRigidStatic({ 50, 10, 90 });
	rock->staticData->attachShape(*shape);
	gScene->addActor(*rock->staticData);
	rock->rItem = new RenderItem(shape, rock->staticData, { 0.5, 0.25, 0, 1 });
	stBodies.push_back(rock);

	rock = new StaticBody;
	shape = CreateShape(PxBoxGeometry(width, 10, depth));
	rock->staticData = gPhysics->createRigidStatic({ 90, 10, 50 });
	rock->staticData->attachShape(*shape);
	gScene->addActor(*rock->staticData);
	rock->rItem = new RenderItem(shape, rock->staticData, { 0.5, 0.25, 0, 1 });
	stBodies.push_back(rock);
}

void SceneManager::createEruption()
{
	ParticleSystem* eruptionPS = new ParticleSystem(fReg, { 90, 60, 90 }, -1);

	pForces.push_back(new GravityForceGenerator({ 0, -10, 0 }));
	eruptionPS->addForceGenerator(pForces[pForces.size() - 1]);

	pForces.push_back(new WindFieldForceGenerator({ 90, 100, 90 }, Vector3(-300, -50, -300), 20));
	eruptionPS->addForceGenerator(pForces[pForces.size() - 1]);

	pSys.push_back(eruptionPS);
}

void SceneManager::createRiver()
{
	pForces.push_back(new ParticleBuoyancy(Vector3(-80, 0, 0), 0.5f, 1.0, 1.0f));

	ParticleSystem* buoyancyPS = new ParticleSystem(fReg, { -80, 0, 0 }, -1);
	buoyancyPS->addForceGenerator(pForces[0]);
	buoyancyPS->addForceGenerator(pForces[pForces.size() - 1]);
	pSys.push_back(buoyancyPS);
}

void SceneManager::createTree()
{
	StaticBody* trunk = new StaticBody;
	PxShape* shape = CreateShape(PxBoxGeometry(3, 20, 3));
	trunk->staticData = gPhysics->createRigidStatic({ 70, 20, -70 });
	trunk->staticData->attachShape(*shape);
	gScene->addActor(*trunk->staticData);
	trunk->rItem = new RenderItem(shape, trunk->staticData, { 0.5, 0.25, 0, 1 });
	stBodies.push_back(trunk);

	StaticBody* leaves = new StaticBody;
	shape = CreateShape(PxBoxGeometry(40, 20, 40));
	leaves->staticData = gPhysics->createRigidStatic({ 60, 60, -60 });
	leaves->staticData->attachShape(*shape);
	gScene->addActor(*leaves->staticData);
	leaves->rItem = new RenderItem(shape, leaves->staticData, { 0, 1, 0.25, 1 });
	stBodies.push_back(leaves);
}

void SceneManager::createBananas()
{
	rbForces.push_back(new DownRigidForceGenerator(200, 0.5));
	rbForces[rbForces.size() - 1]->setActive(false);
	rbForces.push_back(new RigidBodyAnchoredSpring(Vector3(45, 60, -45), 1.5f, 10.0f));
	rbForces.push_back(new RigidBodyAnchoredSpring(Vector3(75, 60, -45), 1.5f, 10.0f));
	rbForces.push_back(new RigidBodyAnchoredSpring(Vector3(45, 60, -75), 1.5f, 10.0f));

	RigidBodySystem* racimoRBS = new RigidBodySystem(fReg, gPhysics, gScene, PxTransform(Vector3(45, 40, -45)), 50, -1);
	racimoRBS->addForceGenerator(rbForces[0]);
	racimoRBS->addForceGenerator(rbForces[1]);
	rbSys.push_back(racimoRBS);

	RigidBodySystem* racimo2RBS = new RigidBodySystem(fReg, gPhysics, gScene, PxTransform(Vector3(75, 40, -45)), 50, -1);
	racimo2RBS->addForceGenerator(rbForces[0]);
	racimo2RBS->addForceGenerator(rbForces[2]);
	rbSys.push_back(racimo2RBS);

	RigidBodySystem* racimo3RBS = new RigidBodySystem(fReg, gPhysics, gScene, PxTransform(Vector3(45, 40, -75)), 50, -1);
	racimo3RBS->addForceGenerator(rbForces[0]);
	racimo3RBS->addForceGenerator(rbForces[3]);
	rbSys.push_back(racimo3RBS);

	racimoRBS->addBody({ 20, 0, 0 }, 5, { 0, 0, 0 }, { 0, 0, 0 }, 0.1, 0.1,
		{ 1, 1, 1 }, 2, -1, { 1, 1, 0, 1 }, RBType::CAPSULE);
	racimoRBS->addBody({ 0, 0, 0 }, 5, { 0, 0, 0 }, { 0, 0, 0 }, 0.1, 0.1,
		{ 1, 1, 1 }, 2, -1, { 1, 1, 0, 1 }, RBType::CAPSULE);
	racimoRBS->addBody({ 0, 0, 20 }, 5, { 0, 0, 0 }, { 0, 0, 0 }, 0.1, 0.1,
		{ 1, 1, 1 }, 2, -1, { 1, 1, 0, 1 }, RBType::CAPSULE);

	racimo2RBS->addBody({ -20, 0, 0 }, 5, { 0, 0, 0 }, { 0, 0, 0 }, 0.1, 0.1,
		{ 1, 1, 1 }, 2, -1, { 1, 1, 0, 1 }, RBType::CUBE);
	racimo2RBS->addBody({ 0, 0, 0 }, 5, { 0, 0, 0 }, { 0, 0, 0 }, 0.1, 0.1,
		{ 1, 1, 1 }, 2, -1, { 1, 1, 0, 1 }, RBType::CUBE);
	racimo2RBS->addBody({ 0, 0, 20 }, 5, { 0, 0, 0 }, { 0, 0, 0 }, 0.1, 0.1,
		{ 1, 1, 1 }, 2, -1, { 1, 1, 0, 1 }, RBType::CUBE);

	racimo3RBS->addBody({ 20, 0, 0 }, 5, { 0, 0, 0 }, { 0, 0, 0 }, 0.1, 0.1,
		{ 1, 1, 1 }, 2, -1, { 1, 1, 0, 1 }, RBType::SPHERE);
	racimo3RBS->addBody({ 0, 0, 0 }, 5, { 0, 0, 0 }, { 0, 0, 0 }, 0.1, 0.1,
		{ 1, 1, 1 }, 2, -1, { 1, 1, 0, 1 }, RBType::SPHERE);
	racimo3RBS->addBody({ 0, 0, 20 }, 5, { 0, 0, 0 }, { 0, 0, 0 }, 0.1, 0.1,
		{ 1, 1, 1 }, 2, -1, { 1, 1, 0, 1 }, RBType::SPHERE);
}

void SceneManager::createSnow()
{
	rbForces.push_back(new TorqueRigidForceGenerator(Vector3(100, 100, 100)));

	RigidBodySystem* snow1RBS = new RigidBodySystem(fReg, gPhysics, gScene, PxTransform(Vector3(70, 110, 70)), 60, -1);
	snow1RBS->addForceGenerator(rbForces[rbForces.size() - 1]);
	rbSys.push_back(snow1RBS);
	RigidBodySystem* snow2RBS = new RigidBodySystem(fReg, gPhysics, gScene, PxTransform(Vector3(75, 110, -75)), 60, -1);
	snow2RBS->addForceGenerator(rbForces[rbForces.size() - 1]);
	rbSys.push_back(snow2RBS);
	RigidBodySystem* snow3RBS = new RigidBodySystem(fReg, gPhysics, gScene, PxTransform(Vector3(0, 110, 0)), 60, -1);
	snow3RBS->addForceGenerator(rbForces[rbForces.size() - 1]);
	rbSys.push_back(snow3RBS);
}

void SceneManager::createRain()
{
	FireworkSystem* rainPS = new FireworkSystem(fReg, { 0, 120, 0 }, -1);
	rainPS->addForceGenerator(pForces[0]);
	pSys.push_back(rainPS);
}

void SceneManager::createPC()
{
	RigidBodySystem* pcRBS = new RigidBodySystem(fReg, gPhysics, gScene, PxTransform(Vector3(0, 0, 0)), 1, -1);
	// playable character
	pc = pcRBS->addBody({ 0, 10, 0 }, 5, { 0, 0, 0 }, { 0, 0, 0 }, 0.1, 0.1,
		{ 0, 0, 0 }, 10, -1, { 0, 1, 1, 1 }, RBType::SPHERE);
	pc->rItem->release();

	rbSys.push_back(pcRBS);
}
