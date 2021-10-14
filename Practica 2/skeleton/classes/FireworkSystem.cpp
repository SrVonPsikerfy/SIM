#include "FireworkSystem.h"


void FireworkSystem::generateFirework(FireworkLoadType type, int payload, int lifes, ParticleData data) {
	particles.push_back(new Firework(type, payload, lifes, posSystem, data));
}

void FireworkSystem::onParticleDeath(int particle) {
	Firework* fr = (Firework*)particles[particle];
	if (fr != nullptr)
		ignite(fr->getPos(), fr->getFireworkLoadType(), fr->getPayload(), fr->getLifes());

	releaseParticle(particle);
}

void FireworkSystem::ignite(Vector3 particlePos, FireworkLoadType loadType, int payload, int life) {
	ParticleData pData;
	switch (loadType) {
	case FireworkLoadType::FLOWER: {
		vector<float> velX = { 0, 20, 30, 20, 0, -20, -30, -20, 0 };
		vector<float> velY = { 30, 20, 0, -20, -30, -20, 0, 20, 0 };
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < payload / 9; j++) {
				float ang = 360.0f / (payload / 9);
				float currentAng = angToRad(ang * j);

				pData.offset = { 0, 0, 0 }; pData.acceleration = { 0, -10, 0 };

				pData.initialSpeed = { (float)cos(currentAng) * ((i == 8) ? 20.0f : 15.0f) + velX[i],
					(float)sin(currentAng) * ((i == 8) ? 20.0f : 15.0f) + velY[i], 0 };

				pData.size = 1; pData.inv_mass = 1; pData.damp = 0.2; pData.lifeTime = rand() % 5 + 1;

				pData.progThroughTime = true;
				pData.color = { 1, 1, 1, 1 };
				if (i == 8) pData.color = { 1, 1, 0, 1 };

				particles.push_back(new Particle(particlePos, pData));
			}
		}
		break;
	}
	case FireworkLoadType::SPHERE: {
		int vel = 20;
		int meridians = payload / 2, parallels = payload - payload / 2;

		float parallelAng = angToRad(180.0) / parallels;
		float meridianAng = angToRad(360.0) / meridians;

		for (int i = 0; i < parallels; i++) {
			float parallelLatitude = angToRad(90.0);
			parallelLatitude += parallelAng * i;

			float parallelHeight = sin(parallelLatitude);
			float parallelRadius = cos(parallelLatitude);

			for (int j = 0; j < meridians; j++) {
				float currMeridianAng = meridianAng * j;

				pData.offset = { 0, 0, 0 }; pData.acceleration = { 0, -10, 0 };
				pData.initialSpeed = { (float)cos(currMeridianAng) * parallelRadius * vel, parallelHeight * vel,
					(float)sin(currMeridianAng) * parallelRadius * vel };

				pData.damp = 0.3; pData.inv_mass = 1; pData.size = 1.5; pData.lifeTime = rand() % 5 + 1;

				pData.progThroughTime = true;
				float r1 = rand() % 255 / 255.0, r2 = rand() % 255 / 255.0,
					r3 = rand() % 255 / 255.0;
				pData.color = { r1, r2, r3, 1 };

				particles.push_back(new Particle(particlePos, pData));
			}
		}
		break;
	}
	case FireworkLoadType::RANDOM: {
		for (int i = 0; i < payload; i++) {
			int vel = rand() % 5 + 3;
			pData.offset = { 0, 0, 0 }; pData.acceleration = { 0, -10, 0 };

			float v1 = rand() % 5 + 1, v2 = rand() % 5 + 1, v3 = rand() % 5 + 1;
			pData.initialSpeed = { v1 * vel, v2 * vel, v3 * vel };

			pData.size = 1; pData.inv_mass = 1; pData.damp = 0.2; pData.lifeTime = rand() % 5 + 2;

			pData.progThroughTime = true;
			float const r1 = rand() % 255 / 255.0, r2 = rand() % 255 / 255.0,
				r3 = rand() % 255 / 255.0;
			pData.color = { r1, r2, r3, 1 };

			int randType = rand() % 2 + 1;
			particles.push_back(new Firework((FireworkLoadType)randType, (int)rand() % 70, life--, particlePos, pData));
		}
		break;
	}
	}
}

float FireworkSystem::angToRad(float ang) {
	return ang * (atan(1) * 4) / 180;
}
