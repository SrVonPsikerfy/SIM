#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>

#include "../../render/RenderUtils.hpp"

#include "../particle_system/ParticleSystem.h"
#include "../particle_system/FireworkSystem.h"

#include "../forces/ForceRegistry.h"

#include "../forces/GravityForceGenerator.h"
#include "../forces/DragForceGenerator.h"
#include "../forces/WindFieldForceGenerator.h"
#include "../forces/ExplosionForceGenerator.h"
#include "../forces/BlackHoleForceGenerator.h"

#include "../springs/ParticleAnchoredSpring.h"
#include "../springs/ParticleSpring.h"
#include "../springs/ParticleBungee.h"
#include "../springs/ParticleBuoyancy.h"

#include "../rigid_body/RigidBodySystem.h"

using namespace std;
using namespace physx;

using ParticleSystems = vector<ParticleSystem*>;
using RigidBodySystems = vector<RigidBodySystem*>;
using Forces = vector<ParticleForceGenerator*>;

class SceneManager
{
	enum class Scenes { DEFAULT, PARTICLE_SYSTEM, FIREWORK_SYSTEM, FORCE, FORCES, SPRING, BUOYANCY, RIGID_SOLID };

public:
	SceneManager(PxPhysics* gPhys, PxScene* gSc, Camera* cam);
	~SceneManager();

	void update(double time);
	void handleInput(unsigned char key);

private:
	PxPhysics* gPhysics;
	PxScene* gScene;

	Camera* camera;

	ForceRegistry* fReg;
	ParticleSystems pSys;
	RigidBodySystems rbSys;
	Forces forces;

	Scenes currScene;

	physx::PxTransform axisPos;
	RenderItem* axis;

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
};
#endif