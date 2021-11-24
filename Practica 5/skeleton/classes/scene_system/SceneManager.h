#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>

#include "../../render/RenderUtils.hpp"

#include "../particle_system/ParticleSystem.h"
#include "../particle_system/FireworkSystem.h"

#include "../forces/ParticleForceRegistry.h"
#include "../forces/GravityForceGenerator.h"
#include "../forces/DragForceGenerator.h"
#include "../forces/WindFieldForceGenerator.h"
#include "../forces/ExplosionForceGenerator.h"
#include "../forces/BlackHoleForceGenerator.h"

#include "../springs/ParticleAnchoredSpring.h"
#include "../springs/ParticleSpring.h"
#include "../springs/ParticleBungee.h"
#include "../springs/ParticleBuoyancy.h"

using namespace std;
using namespace physx;

using Forces = vector<ParticleForceGenerator*>;
using ParticleSystems = vector<ParticleSystem*>;

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

	ParticleForceRegistry* fReg;
	ParticleSystems pSys;
	Scenes currScene;
	Forces forces;

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