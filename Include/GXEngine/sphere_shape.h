#ifndef SPHERE_SHAPE_H
#define SPHERE_SHAPE_H

#include "physics_shape.h"

namespace gx_engine {

class SphereShape : public PhysicsShape {
public:
    SphereShape();
    ~SphereShape() override;

    void getExtremePoint(gx_common::GXVec3& point, const gx_common::GXMat4& transform,
                         const gx_common::GXVec3& direction) const override;

    void setRadius(float radius);    // m
    float getRadius() const;

protected:
    void updateMassAndInertiaTensor() override;

private:
    float _radius;
};

} // namespace gx_engine

#endif SPHERE_SHAPE_H
