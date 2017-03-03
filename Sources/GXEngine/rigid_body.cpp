#include <GXEngine/rigid_body.h>

#define DEFAULT_VELOCITY_X 0.0f
#define DEFAULT_VELOCITY_Y 0.0f
#define DEFAULT_VELOCITY_Z 0.0f

#define DEFAULT_LOCATION_X 0.0f
#define DEFAULT_LOCATION_Y 0.0f
#define DEFAULT_LOCATION_Z 0.0f

#define DEFAULT_ANGULAR_MOMENTUM_X 0.0f
#define DEFAULT_ANGULAR_MOMENTUM_Y 0.0f
#define DEFAULT_ANGULAR_MOMENTUM_Z 0.0f

#define DEFAULT_ANGULAR_VELOCITY_X 0.0f
#define DEFAULT_ANGULAR_VELOCITY_Y 0.0f
#define DEFAULT_ANGULAR_VELOCITY_Z 0.0f

#define DEFAULT_MASS 0.001f

namespace gx_engine {

RigidBody::RigidBody()
    : _shapeLinks(nullptr)
{
    setLocationWorld(DEFAULT_LOCATION_X, DEFAULT_LOCATION_Y, DEFAULT_LOCATION_Z);
    setVelocityWorld(DEFAULT_VELOCITY_X, DEFAULT_VELOCITY_Y, DEFAULT_VELOCITY_Z);
    gx_common::gxSetMat3Identity(_rotation);
    setAngularMomentumWorld(gx_common::gxCreateVec3(DEFAULT_ANGULAR_MOMENTUM_X, DEFAULT_ANGULAR_MOMENTUM_Y,
                                               DEFAULT_ANGULAR_MOMENTUM_Z));

    gx_common::gxSetMat3Identity(_inverseInertiaTensorLocal);
    gx_common::gxSetMat3Identity(_inertiaTensorWorld);

    _mass = DEFAULT_MASS;

    _angularVelocityWorld = gx_common::gxCreateVec3(DEFAULT_ANGULAR_VELOCITY_X, DEFAULT_ANGULAR_VELOCITY_Y,
                                               DEFAULT_ANGULAR_VELOCITY_Z);

    gx_common::gxSetAABBEmpty(_boundsWorld);
}

RigidBody::~RigidBody()
{
    while (_shapeLinks) {
        PhysicsShapeLink* link = _shapeLinks;
        _shapeLinks = _shapeLinks->next;
        delete link->shape;
        delete link;
    }
}

void RigidBody::addShape(PhysicsShape* shape, const gx_common::GXVec3& shapePivotOffset,
                         const gx_common::GXMat3& shapeRotation)
{
    PhysicsShapeLink* link = new PhysicsShapeLink();
    link->rigidBody = this;
    link->shape = shape;
    link->shapePivotOffset = shapePivotOffset;
    link->shapeRotation = shapeRotation;
    link->next = nullptr;
    link->prev = nullptr;

    if (!_shapeLinks) {
        _shapeLinks = link;
    } else {
        link->next = _shapeLinks;
        _shapeLinks->prev = link;
    }

    onShapeUpdated();
}

PhysicsShapeLink* RigidBody::getShapes()
{
    return _shapeLinks;
}

void RigidBody::setLocationWorld(float x, float y, float z)
{
    _centerOfMassWorld = gx_common::gxCreateVec3(x, y, z);
    updateBoundsWorld();
}

void RigidBody::setVelocityWorld(float x, float y, float z)
{
    _velocityWorld = gx_common::gxCreateVec3(x, y, z);
}

void RigidBody::setRotation(const gx_common::GXMat4& rotation)
{
    gx_common::gxSetMat3FromMat4(_rotation, rotation);
    updateBoundsWorld();
}

void RigidBody::setAngularMomentumWorld(const gx_common::GXVec3& momentum)
{
    _angularMomentumWorld = momentum;
}

void RigidBody::setInertiaTensorWorld(const gx_common::GXMat3& inertiaTensor)
{
    _inertiaTensorWorld = inertiaTensor;
}

void RigidBody::setAngularVelocity(const gx_common::GXVec3& angularVelocity)
{
    _angularVelocityWorld = angularVelocity;
}

const gx_common::GXVec3& RigidBody::getLocationWorld() const
{
    return _centerOfMassWorld;
}

const gx_common::GXVec3& RigidBody::getVelocityWorld() const
{
    return _velocityWorld;
}

const gx_common::GXMat3& RigidBody::getRotation() const
{
    return _rotation;
}

const gx_common::GXVec3& RigidBody::getAngularMomentumWorld() const
{
    return _angularMomentumWorld;
}

const gx_common::GXMat3& RigidBody::getInverseInertiaTensorLocal() const
{
    return _inverseInertiaTensorLocal;
}

const gx_common::GXMat3& RigidBody::getInertiaTensorWorld() const
{
    return _inertiaTensorWorld;
}

const gx_common::GXVec3& RigidBody::getAngularVelocity() const
{
    return _angularVelocityWorld;
}

const gx_common::GXAABB& RigidBody::getBoundsWorld() const
{
    return _boundsWorld;
}

void RigidBody::onShapeUpdated()
{
    _mass = 0.0f;
    gx_common::GXVec3 centerOfMassLocal(0.0f, 0.0f, 0.0f);

    for (PhysicsShapeLink* link = _shapeLinks; link; link = link->next) {
        float shapeMass = link->shape->getMass();

        gx_common::GXVec3 alpha;
        gx_common::gxMulVec3Scalar(alpha, link->shapePivotOffset, shapeMass);
        gx_common::gxSumVec3Vec3(centerOfMassLocal, centerOfMassLocal, alpha);

        _mass += shapeMass;
    }

    gx_common::gxMulVec3Scalar(centerOfMassLocal, centerOfMassLocal, 1.0f / _mass);

    for (PhysicsShapeLink* link = _shapeLinks; link; link = link->next) {
        gx_common::gxSubVec3Vec3(link->shapeCenterOfMassOffset, link->shapePivotOffset,
                                 centerOfMassLocal);
    }

    gx_common::GXMat3 inertiaTensorLocal;
    gx_common::gxSetMat3Zero(inertiaTensorLocal);

    for (PhysicsShapeLink* link = _shapeLinks; link; link = link->next) {
        gx_common::GXMat3 alpha;
        gx_common::gxSetMat3Transponse(alpha, link->shapeRotation);

        gx_common::GXMat3 betta;
        gx_common::gxMulMat3Mat3(betta, alpha, link->shape->getInertiaTensor());

        gx_common::GXMat3 shapeInertiaTensorRigidBodyLocal;
        gx_common::gxMulMat3Mat3(shapeInertiaTensorRigidBodyLocal, betta, link->shapeRotation);

        gx_common::GXVec3 zeta = link->shapeCenterOfMassOffset;

        float eta = zeta.y * zeta.y;
        float iota = zeta.z * zeta.z;
        float kappa = zeta.x * zeta.x;
        float lambda = -zeta.x * zeta.y;
        float mu = -zeta.x * zeta.z;
        float nu = -zeta.y * zeta.z;

        gx_common::GXMat3 xi;
        xi.m11 = eta + iota;
        xi.m12 = lambda;
        xi.m13 = mu;

        xi.m21 = lambda;
        xi.m22 = kappa + iota;
        xi.m23 = nu;

        xi.m31 = mu;
        xi.m32 = nu;
        xi.m33 = kappa + eta;

        gx_common::GXMat3 omicron;
        gx_common::gxMulMat3Scalar(omicron, xi, link->shape->getMass());

        gx_common::GXMat3 sigma;
        gx_common::gxSubMat3Mat3(sigma, shapeInertiaTensorRigidBodyLocal, omicron);

        gx_common::gxSumMat3Mat3(inertiaTensorLocal, inertiaTensorLocal, sigma);
    }

    gx_common::gxSetMat3Inverse(_inverseInertiaTensorLocal, inertiaTensorLocal);

    gx_common::GXMat3 alpha;
    gx_common::gxSetMat3Transponse(alpha, _rotation);

    gx_common::GXMat3 betta;
    gx_common::gxMulMat3Mat3(betta, alpha, _inverseInertiaTensorLocal);

    gx_common::gxMulMat3Mat3(_inertiaTensorWorld, betta, _rotation);

    gx_common::gxMulVec3Mat3(_angularVelocityWorld, _angularMomentumWorld, _inertiaTensorWorld);

    updateBoundsWorld();
}

void RigidBody::updateBoundsWorld()
{
    gx_common::gxSetAABBEmpty(_boundsWorld);

    gx_common::GXMat4 rigidBodyLocalToRigidBodyWorld;
    rigidBodyLocalToRigidBodyWorld.from(_rotation, _centerOfMassWorld);

    for (PhysicsShapeLink* link = _shapeLinks; link; link = link->next) {
        gx_common::GXMat4 shapeLocalToRigidBodyLocal;
        shapeLocalToRigidBodyLocal.from(link->shapeRotation, link->shapeCenterOfMassOffset);

        gx_common::GXMat4 shapeLocalToRigidBodyWorld;
        gx_common::gxMulMat4Mat4(shapeLocalToRigidBodyWorld, shapeLocalToRigidBodyLocal,
                                 rigidBodyLocalToRigidBodyWorld);

        gx_common::GXAABB shapeBoundsRigidBodyWorld;
        gx_common::gxSetAABBTransformed(shapeBoundsRigidBodyWorld, link->shape->getBoundsLocal(),
                                        shapeLocalToRigidBodyWorld);

        gx_common::gxAddVertexToAABB(_boundsWorld, shapeBoundsRigidBodyWorld.min);
        gx_common::gxAddVertexToAABB(_boundsWorld, shapeBoundsRigidBodyWorld.max);
    }
}

} // namespace gx_engine
