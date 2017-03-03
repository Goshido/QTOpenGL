#include <GXEngine/box_shape.h>

#define DEFAULT_WIDTH 1.0f
#define DEFAULT_HEIGHT 1.0f
#define DEFAULT_DEPTH 1.0f

namespace gx_engine {

BoxShape::BoxShape()
    : PhysicsShape(PhysicsShapeType::Box)
{
    _width = DEFAULT_WIDTH;
    _height = DEFAULT_HEIGHT;
    setDepth(DEFAULT_DEPTH);
}

BoxShape::~BoxShape()
{
    // NOTHING
}

void BoxShape::getExtremePoint(gx_common::GXVec3& point, const gx_common::GXMat4& transform,
                     const gx_common::GXVec3& direction) const
{
    float halfWidth = _width * 0.5f;
    float halfHeight = _height * 0.5f;
    float halfDepth = _depth * 0.5f;

    gx_common::GXVec3 local[8];
    local[0] = gx_common::gxCreateVec3(-halfWidth, -halfHeight, -halfDepth);
    local[1] = gx_common::gxCreateVec3(halfWidth, -halfHeight, -halfDepth);
    local[2] = gx_common::gxCreateVec3(halfWidth, halfHeight, -halfDepth);
    local[3] = gx_common::gxCreateVec3(-halfWidth, halfHeight, -halfDepth);
    local[4] = gx_common::gxCreateVec3(-halfWidth, -halfHeight, halfDepth);
    local[5] = gx_common::gxCreateVec3(halfWidth, -halfHeight, halfDepth);
    local[6] = gx_common::gxCreateVec3(halfWidth, halfHeight, halfDepth);
    local[7] = gx_common::gxCreateVec3(-halfWidth, halfHeight, halfDepth);

    gx_common::GXVec3 world[8];
    for (unsigned char i = 0; i < 8; i++) {
        gx_common::gxMulVec3Mat4AsPoint(world[i], local[i], transform);
    }

    unsigned char extremeIndex = 0;
    float maxDot = gx_common::gxDotVec3Fast(world[extremeIndex], direction);

    for (unsigned char i = 1; i < 8; i++) {
        float currentDot = gx_common::gxDotVec3Fast(world[i], direction);
        if (currentDot > maxDot) {
            extremeIndex = i;
            maxDot = currentDot;
        }
    }

    point = world[extremeIndex];
}

void BoxShape::setWidth(float width)
{
    _width = width;
    gx_common::gxSetAABBEmpty(_boundsLocal);
    gx_common::gxAddVertexToAABB(_boundsLocal, -0.5f * _width, -0.5f * _height, -0.5f * _depth);
    gx_common::gxAddVertexToAABB(_boundsLocal, 0.5f * _width, 0.5f * _height, 0.5f * _depth);
    updateMassAndInertiaTensor();
}

void BoxShape::setHeight(float height)
{
    _height = height;
    gx_common::gxSetAABBEmpty(_boundsLocal);
    gx_common::gxAddVertexToAABB(_boundsLocal, -0.5f * _width, -0.5f * _height, -0.5f * _depth);
    gx_common::gxAddVertexToAABB(_boundsLocal, 0.5f * _width, 0.5f * _height, 0.5f * _depth);
    updateMassAndInertiaTensor();
}

void BoxShape::setDepth(float depth)
{
    _depth = depth;
    gx_common::gxSetAABBEmpty(_boundsLocal);
    gx_common::gxAddVertexToAABB(_boundsLocal, -0.5f * _width, -0.5f * _height, -0.5f * _depth);
    gx_common::gxAddVertexToAABB(_boundsLocal, 0.5f * _width, 0.5f * _height, 0.5f * _depth);
    updateMassAndInertiaTensor();
}

void BoxShape::updateMassAndInertiaTensor()
{
    _mass = _width * _height * _depth * _density;

    float alpha = _width * _width;
    float betta = _height * _height;
    float gamma = _depth * _depth;
    float iota = 0.083333f * _mass;

    _inertiaTensor.m11 = iota * (betta + gamma);
    _inertiaTensor.m22 = iota * (alpha + gamma);
    _inertiaTensor.m33 = iota * (alpha + betta);

    _inertiaTensor.m12 = _inertiaTensor.m13 = 0.0f;
    _inertiaTensor.m21 = _inertiaTensor.m23 = 0.0f;
    _inertiaTensor.m31 = _inertiaTensor.m32 = 0.0f;
}

} // namespace gx_engine
