#ifndef SPHERICALFORCEFIELD_H
#define SPHERICALFORCEFIELD_H

#include "../../utils/checkML.h"
#include "../../utils/core.hpp"

#include "../../render/RenderUtils.hpp"

class SphericalForceField
{
public:
	Vector3 getPos() { return fieldPos.p; };

protected: 
	SphericalForceField(const Vector3& pos, const Vector4& color, double size);
	virtual ~SphericalForceField();

	virtual bool onField(const Vector3& p);

	physx::PxTransform fieldPos;
	RenderItem* field;
	double radius;
};
#endif