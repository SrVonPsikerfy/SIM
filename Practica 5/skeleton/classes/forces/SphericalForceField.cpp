#include "SphericalForceField.h"

SphericalForceField::SphericalForceField(const Vector3& pos, const Vector4& color, double size) {
	radius = size;
	fieldPos = physx::PxTransform(pos);
	field = new RenderItem(CreateShape(physx::PxSphereGeometry(size)), &fieldPos, color);
}

SphericalForceField::~SphericalForceField() {
	field->release();
}

bool SphericalForceField::onField(const Vector3& p) {
	return (fieldPos.p - p).magnitude() < radius;
}