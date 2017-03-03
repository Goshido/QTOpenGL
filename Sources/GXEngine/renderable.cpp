#include <GXEngine/renderable.h>
#include <string.h>

namespace gx_engine {

Renderable::Renderable()
{
    gx_common::gxSetMat4Identity(_currentModelMatrix);
    gx_common::gxSetMat4Identity(_lastFrameModelMatrix);
    gx_common::gxSetMat4Identity(_scaleMatrix);
    gx_common::gxSetMat4Identity(_rotationMatrix);
}

Renderable::~Renderable()
{
    // NOTHING
}

void Renderable::setLocation(float x, float y, float z)
{
    _currentModelMatrix.m41 = x;
    _currentModelMatrix.m42 = y;
    _currentModelMatrix.m43 = z;
    onTransformChanged();
}

void Renderable::setLocation(const gx_common::GXVec3& location)
{
    memcpy(_currentModelMatrix.wv.arr, location.arr, sizeof(gx_common::GXVec3));
    onTransformChanged();
}

void Renderable::getLocation(gx_common::GXVec3& location) const
{
    memcpy(location.arr, _currentModelMatrix.wv.arr, sizeof(gx_common::GXVec3));
}

void Renderable::setRotation(float pitch_rad, float yaw_rad, float roll_rad)
{
    gx_common::GXVec3 location;
    memcpy(location.arr, _currentModelMatrix.wv.arr, sizeof(gx_common::GXVec3));

    gx_common::gxSetMat4RotationXYZ(_rotationMatrix, pitch_rad, yaw_rad, roll_rad);
    gx_common::gxMulMat4Mat4(_currentModelMatrix, _rotationMatrix, _scaleMatrix);

    memcpy(_currentModelMatrix.wv.arr, location.arr, sizeof(gx_common::GXVec3));
    onTransformChanged();
}

void Renderable::setRotation(const gx_common::GXVec3& rotation)
{
    gx_common::GXVec3 location;
    memcpy(location.arr, _currentModelMatrix.wv.arr, sizeof(gx_common::GXVec3));

    gx_common::gxSetMat4RotationXYZ(_rotationMatrix, rotation.pitch_rad, rotation.yaw_rad, rotation.roll_rad);
    gx_common::gxMulMat4Mat4(_currentModelMatrix, _rotationMatrix, _scaleMatrix);

    memcpy(_currentModelMatrix.wv.arr, location.arr, sizeof(gx_common::GXVec3));
    onTransformChanged();
}

void Renderable::setRotation(const gx_common::GXQuat& quaternion)
{
    static const gx_common::GXVec3 origin = gx_common::gxCreateVec3(0.0f, 0.0f, 0.0f);

    gx_common::GXVec3 location;
    memcpy(location.arr, _currentModelMatrix.wv.arr, sizeof(gx_common::GXVec3));

    _rotationMatrix.from(quaternion, origin);

    gx_common::gxMulMat4Mat4(_currentModelMatrix, _rotationMatrix, _scaleMatrix);

    memcpy(_currentModelMatrix.wv.arr, location.arr, sizeof(gx_common::GXVec3));
    onTransformChanged();
}

void Renderable::setRotation(const gx_common::GXMat4& rotation)
{
    gx_common::GXVec3 location;
    memcpy(location.arr, _currentModelMatrix.wv.arr, sizeof(gx_common::GXVec3));

    _rotationMatrix = rotation;
    gx_common::gxMulMat4Mat4(_currentModelMatrix, _rotationMatrix, _scaleMatrix);

    memcpy(_currentModelMatrix.wv.arr, location.arr, sizeof(gx_common::GXVec3));
    onTransformChanged();
}

void Renderable::getRotation(gx_common::GXMat4& rotation) const
{
    rotation = _rotationMatrix;
}

void Renderable::getRotation(gx_common::GXQuat& quaternion) const
{
    quaternion = gx_common::gxCreateQuat(_rotationMatrix);
}

void Renderable::setScale(float x, float y, float z)
{
    gx_common::GXVec3 location;
    memcpy(location.arr, _currentModelMatrix.wv.arr, sizeof(gx_common::GXVec3));

    gx_common::gxSetMat4Scale(_scaleMatrix, x, y, z);
    gx_common::gxMulMat4Mat4(_currentModelMatrix, _rotationMatrix, _scaleMatrix);

    memcpy(_currentModelMatrix.wv.arr, location.arr, sizeof(gx_common::GXVec3));
    onTransformChanged();
}

void Renderable::setScale(const gx_common::GXVec3& scale)
{
    gx_common::GXVec3 location;
    memcpy(location.arr, _currentModelMatrix.wv.arr, sizeof(gx_common::GXVec3));

    gx_common::gxSetMat4Scale(_scaleMatrix, scale.x, scale.y, scale.z);
    gx_common::gxMulMat4Mat4(_currentModelMatrix, _rotationMatrix, _scaleMatrix);

    memcpy(_currentModelMatrix.wv.arr, location.arr, sizeof(gx_common::GXVec3));
    onTransformChanged();
}

void Renderable::getScale(gx_common::GXVec3& scale) const
{
    scale.x = _scaleMatrix.m11;
    scale.y = _scaleMatrix.m22;
    scale.z = _scaleMatrix.m33;
}

const gx_common::GXMat4& Renderable::getCurrentModelMatrix() const
{
    return _currentModelMatrix;
}

const gx_common::GXMat4& Renderable::getLastFrameModelMatrix() const
{
    return _lastFrameModelMatrix;
}

void Renderable::onTransformChanged()
{
    // NOTHING
}

void Renderable::updateLastFrameModelMatrix()
{
    _lastFrameModelMatrix = _currentModelMatrix;
}

} // namespace gx_engine
