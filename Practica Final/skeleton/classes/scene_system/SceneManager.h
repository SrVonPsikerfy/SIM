#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>

#include "../../render/RenderUtils.hpp"

#include "../particle_system/ParticleSystem.h"
#include "../particle_system/FireworkSystem.h"

#include "../rigid_body_system/RigidBodySystem.h"

#include "../forces/ForceRegistry.h"

#include "../forces/particles/GravityForceGenerator.h"
#include "../forces/particles/DragForceGenerator.h"
#include "../forces/particles/WindFieldForceGenerator.h"
#include "../forces/particles/ExplosionForceGenerator.h"
#include "../forces/particles/BlackHoleForceGenerator.h"

#include "../forces/springs/ParticleAnchoredSpring.h"
#include "../forces/springs/ParticleSpring.h"
#include "../forces/springs/ParticleBungee.h"
#include "../forces/springs/ParticleBuoyancy.h"

#include "../forces/rigid_bodies/WindRigidForceGenerator.h"
#include "../forces/rigid_bodies/ExplosionRigidForceGenerator.h"
#include "../forces/rigid_bodies/TorqueRigidForceGenerator.h"
#include "../forces/rigid_bodies/DownRigidForceGenerator.h"
#include "../forces/rigid_bodies/DefaultForceGenerator.h"

using namespace std;
using namespace physx;

using ParticleSystems = vector<ParticleSystem*>;
using RigidBodySystems = vector<RigidBodySystem*>;

using ParticleForces = vector<ParticleForceGenerator*>;
using RigidBodyForces = vector<RigidBodyForceGenerator*>;

using StaticBodies = vector<StaticBody*>;

class SceneManager
{
	enum class Scenes { DEFAULT, PARTICLE_SYSTEM, FIREWORK_SYSTEM, FORCE, FORCES, SPRING, BUOYANCY, RIGID_SOLID };

public:
	SceneManager(PxPhysics* gPhys, PxScene* gSc, PxMaterial* gMat, Camera* cam);
	~SceneManager();

	void update(double time);
	void handleInput(unsigned char key);

private:
	PxPhysics* gPhysics;
	PxScene* gScene;
	PxMaterial* gMaterial;

	Camera* camera;

	ParticleSystems pSys;
	RigidBodySystems rbSys;

	StaticBodies stBodies;

	ForceRegistry* fReg;
	ParticleForces pForces;
	RigidBodyForces rbForces;

	Scenes currScene;

	RigidBody* pc;
	float rainSpawn, spawnT;
	bool deathScene;

	void free();
	void changeScene(Scenes newScene);

	void defaultScene();
	void partSysScene();
	void fireSysScene();
	void forcesScene();
	void windScene();
	void explosionScene();
	void blackHoleScene();
	void anchoredSpringScene();
	void springScene();
	void buoyancyScene();
	void bungeeScene();
	void rigidBodyScene();
	void finalScene();

	void createSkybox();
	void createVolcano();
	void createEruption();
	void createRiver();
	void createTree();
	void createBananas();
	void createSnow();
	void createRain();
	void createPC();
};
#endif