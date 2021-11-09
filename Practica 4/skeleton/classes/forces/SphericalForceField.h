#ifndef SPHERICALFORCEFIELD_H
#define SPHERICALFORCEFIELD_H

#include "../../utils/checkML.h"

#include "ParticleForceGenerator.h"

class SphericalForceField : public ParticleForceGenerator
{
protected: 
	SphericalForceField(const Vector3& pos, const Vector4& color, double size);
	virtual ~SphericalForceField();

	virtual bool onField(const Vector3& p);

	physx::PxTransform fieldPos;
	RenderItem* field;
	double radius;
};
#endif