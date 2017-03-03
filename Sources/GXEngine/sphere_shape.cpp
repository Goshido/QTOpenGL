#include <GXEngine/sphere_shape.h>

#define DEFAULT_RADIUS 0.2f

namespace gx_engine {

SphereShape::SphereShape()
    : PhysicsShape(PhysicsShapeType::Sphere)
{
    setRadius(DEFAULT_RADIUS);
}

SphereShape::~SphereShape()
{
    // NOTHING
}

void SphereShape::getExtremePoint(gx_common::GXVec3& point, const gx_common::GXMat4& transform,
                                  const gx_common::GXVec3& direction) const
{
    gx_common::GXVec3 d(direction.x, direction.y, direction.z);
    gx_common::gxNormalizeVec3(d);
    gx_common::GXVec3 offset;
    gx_common::gxMulVec3Scalar(offset, d, _radius);
    gx_common::gxSumVec3Vec3(point, transform.wv, offset);
}

void SphereShape::setRadius(float radius)
{
    _radius = radius;
    gx_common::gxSetAABBEmpty(_boundsLocal);
    gx_common::gxAddVertexToAABB(_boundsLocal, -_radius, -_radius, -_radius);
    gx_common::gxAddVertexToAABB(_boundsLocal, _radius, _radius, _radius);
    updateMassAndInertiaTensor();
}

float SphereShape::getRadius() const
{
    return _radius;
}

void SphereShape::updateMassAndInertiaTensor()
{
    float alpha = _radius * _radius;

    _mass = 1.3333333f * GX_MATH_PI * alpha * _radius * _density;

    _inertiaTensor.m11 = _inertiaTensor.m22 = _inertiaTensor.m33 = 0.4f * _mass * alpha;
    _inertiaTensor.m12 = _inertiaTensor.m13 = 0.0f;
    _inertiaTensor.m21 = _inertiaTensor.m23 = 0.0f;
    _inertiaTensor.m31 = _inertiaTensor.m32 = 0.0f;
}

} // namespace gx_engine
