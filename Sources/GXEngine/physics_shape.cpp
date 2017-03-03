#include <GXEngine/physics_shape.h>

#define DEFAULT_DENSITY 7800        // steel
#define INVALID_MASS -1.0f
#define DEFAULT_FRICTION 0.5f
#define DEFAULT_COEFFICIENT_OF_RESTRICTION 1.0f
#define DEFAULT_COLLISION_FLAGS 0xFFFFFFFF

namespace gx_engine {

PhysicsShape::PhysicsShape(PhysicsShapeType type)
    : _type(type)
    , _density(DEFAULT_DENSITY)
    , _mass(INVALID_MASS)
{
    setFriction(DEFAULT_FRICTION);
    setCoefficientOfRestitution(DEFAULT_COEFFICIENT_OF_RESTRICTION);
    gx_common::gxSetMat3Identity(_inertiaTensor);
    setCollisionFlags(DEFAULT_COLLISION_FLAGS);
}

PhysicsShape::~PhysicsShape()
{
    // NOTHING
}

PhysicsShapeType PhysicsShape::getType() const
{
    return _type;
}

const gx_common::GXAABB& PhysicsShape::getBoundsLocal() const
{
    return _boundsLocal;
}

void PhysicsShape::setDensity(float density)
{
    _density = density;
    updateMassAndInertiaTensor();
}

float PhysicsShape::getDensity() const
{
    return _density;
}

float PhysicsShape::getMass() const
{
    return _mass;
}

void PhysicsShape::setFriction(float friction)
{
    _friction = friction;
}

float PhysicsShape::getFriction() const
{
    return _friction;
}

void PhysicsShape::setCoefficientOfRestitution(float coefficient)
{
    _coefficientOfRestitution = coefficient;
}

float PhysicsShape::getCoefficientOfRestitution() const
{
    return _coefficientOfRestitution;
}

const gx_common::GXMat3 PhysicsShape::getInertiaTensor() const
{
    return _inertiaTensor;
}

void PhysicsShape::setCollisionFlags(unsigned int flags)
{
    _collisionFlags = flags;
}

unsigned int PhysicsShape::getCollisionFlags()
{
    return _collisionFlags;
}

} // namespace gx_engine
