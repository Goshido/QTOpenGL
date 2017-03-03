#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <GXCommon/gx_math.h>
#include "physics_shape.h"
#include <QVector>

namespace gx_engine {

class RigidBody;

struct PhysicsShapeLink {
    RigidBody* rigidBody;
    PhysicsShape* shape;

    gx_common::GXVec3 shapeCenterOfMassOffset;
    gx_common::GXVec3 shapePivotOffset;
    gx_common::GXMat3 shapeRotation;

    PhysicsShapeLink* next;
    PhysicsShapeLink* prev;
};

class RigidBody {
public:
    explicit RigidBody();
    ~RigidBody();

    void addShape(PhysicsShape* shape, const gx_common::GXVec3& shapePivotOffset,
                  const gx_common::GXMat3& shapeRotation);
    PhysicsShapeLink* getShapes();

    void setLocationWorld(float x, float y, float z);            // m
    void setVelocityWorld(float x, float y, float z);            // m / sec
    void setRotation(const gx_common::GXMat4& rotation);
    void setAngularMomentumWorld(const gx_common::GXVec3& momentum);
    void setInertiaTensorWorld(const gx_common::GXMat3& inertiaTensor);
    void setAngularVelocity(const gx_common::GXVec3& angularVelocity);

    const gx_common::GXVec3& getLocationWorld() const;
    const gx_common::GXVec3& getVelocityWorld() const;
    const gx_common::GXMat3& getRotation() const;
    const gx_common::GXVec3& getAngularMomentumWorld() const;
    const gx_common::GXMat3& getInverseInertiaTensorLocal() const;
    const gx_common::GXMat3& getInertiaTensorWorld() const;
    const gx_common::GXVec3& getAngularVelocity() const;
    const gx_common::GXAABB& getBoundsWorld() const;

    void onShapeUpdated();
    void updateBoundsWorld();

private:
    gx_common::GXMat3 _inverseInertiaTensorLocal;
    gx_common::GXMat3 _inertiaTensorWorld;

    gx_common::GXVec3 _centerOfMassWorld;
    gx_common::GXVec3 _velocityWorld;
    gx_common::GXMat3 _rotation;
    gx_common::GXVec3 _angularMomentumWorld;
    gx_common::GXVec3 _angularVelocityWorld;
    float _mass;

    PhysicsShapeLink* _shapeLinks;
    gx_common::GXAABB _boundsWorld;
};

} // namespace gx_engine


#endif RIGID_BODY_H
