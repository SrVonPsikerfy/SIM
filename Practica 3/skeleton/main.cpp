#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "./utils/core.hpp"
#include "./utils/RenderUtils.hpp"
#include "./utils/callbacks.hpp"
#include "./utils/checkML.h"

#include "./classes/ParticleSystem.h"
#include "./classes/FireworkSystem.h"

#include "./classes/ParticleForceRegistry.h"

#include "./classes/ForceGenerators.h"

using namespace physx;

PxDefaultAllocator gAllocator;
PxDefaultErrorCallback gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;

PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;

RenderItem* axis;

ParticleSystem* inputParticleSystem = nullptr/*, * automaticParticleSystem = nullptr*/;
//FireworkSystem* frSystem = nullptr;

ParticleForceRegistry* fReg = nullptr;
ForceGenerators* forces = nullptr;

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	// ------------------------------------------------------

	fReg = new ParticleForceRegistry();
	forces = new ForceGenerators();

	inputParticleSystem = new ParticleSystem(fReg);
	inputParticleSystem->applyForceGenerator(forces->gravity);
	inputParticleSystem->applyForceGenerator(forces->gravity);
	/*automaticParticleSystem = new ParticleSystem();
	frSystem = new FireworkSystem();*/
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	fReg->updateForces(t);

	inputParticleSystem->update(t);
	/*automaticParticleSystem->update(t);
	frSystem->update(t);*/

	gScene->simulate(t);
	gScene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	ParticleData pData;
	FireworkLoadType type;
	int randVel = (std::rand() % 30 + 20);

	switch (toupper(key)) {
	case ' ':
		//offset iniSpeed acceleration damp invmass size deathTime prog color
		pData = { { 0, 0, 0 }, GetCamera()->getDir() * 200, { 0, 0, 0 }, 0.999, 1, 3, 6, true, { 1, 1, 1, 1 } };
		inputParticleSystem->generateBullet(GetCamera()->getTransform().p, pData);
		break;
	case 'X': {
		double time = (((float)rand()) / RAND_MAX) / 16;
		//automaticParticleSystem->spawnFountain(time);
		break;
	}
	case 'F':
		pData = { { 0, 0, 0 }, { 0, 30, 0 } , { 0, -10, 0 }, 0.999, 1, 2, 3, true, { 1, 1, 1, 1 } };
		//frSystem->generateFirework(FireworkLoadType::FLOWER, 360, 1, pData);
		break;
	case 'P':
		pData = { { 0, 0, 0 }, { 0, 30, 0 } , { 0, -10, 0 }, 0.999, 1, 2, 3, true, { 0, 1, 0, 1 } };
		//frSystem->generateFirework(FireworkLoadType::SPHERE, 40, 1, pData);
		break;
	case 'O':
		pData = { { 0, 0, 0 }, { 0, 30, 0 } , { 0, -10, 0 }, 0.999, 1, 2, 3, true, { 0, 0, 1, 1 } };
		//frSystem->generateFirework(FireworkLoadType::RANDOM, 30, 2, pData);
		break;
	case 'R':
		inputParticleSystem->reset();
		/*automaticParticleSystem->reset();
		frSystem->reset();*/
		break;
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}

int main(int, const char* const*)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Check Memory Leaks

#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for (PxU32 i = 0; i < frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}