#ifndef PHYSICS_SHAPE_H
#define PHYSICS_SHAPE_H

#include <GXCommon/gx_math.h>

namespace gx_engine {

enum class PhysicsShapeType : unsigned char {
    Sphere,
    Box,
    Capsule,
    Convex
};

class PhysicsShape {
public:
    explicit PhysicsShape(PhysicsShapeType type);
    virtual ~PhysicsShape();

    virtual void getExtremePoint(gx_common::GXVec3& point, const gx_common::GXMat4& transform,
                                 const gx_common::GXVec3& direction) const = 0;

    PhysicsShapeType getType() const;
    const gx_common::GXAABB& getBoundsLocal() const;

    void setDensity(float density);                         // kg / m^3
    float getDensity() const;

    float getMass() const;                                  // kg

    void setFriction(float friction);                       //[0.0f 1.0f]
    float getFriction() const;

    void setCoefficientOfRestitution(float coefficient);    //[0.0f 1.0f]
    float getCoefficientOfRestitution() const;

    const gx_common::GXMat3 getInertiaTensor() const;

    void setCollisionFlags(unsigned int flags);
    unsigned int getCollisionFlags();

protected:
    virtual void updateMassAndInertiaTensor() = 0;

    PhysicsShapeType _type;
    float _density;
    float _mass;
    float _friction;
    float _coefficientOfRestitution;
    gx_common::GXMat3 _inertiaTensor;
    gx_common::GXAABB _boundsLocal;
    unsigned int _collisionFlags;
};

} // namespace gx_engine

#endif PHYSICS_SHAPE_H
