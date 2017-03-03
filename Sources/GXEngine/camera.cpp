#include <GXEngine/camera.h>
#include <string.h>

namespace gx_engine {

Camera* Camera::_activeCamera = nullptr;

Camera::Camera()
{
    gx_common::gxSetMat4Identity(_modelMatrix);
    gx_common::gxSetMat4Identity(_currentViewMatrix);
    gx_common::gxSetMat4Identity(_lastFrameViewMatrix);
    gx_common::gxSetMat4Identity(_projectionMatrix);
    gx_common::gxSetMat4Identity(_invProjectionMatrix);
    gx_common::gxSetMat4Identity(_viewProjectionMatrix);
    updatePlanes();
}

Camera::~Camera()
{
    // NOTHING
}

void Camera::setLocation(float x, float y, float z)
{
    gx_common::gxSetMat4TranslateTo(_modelMatrix, x, y, z);
    gx_common::gxSetMat4Inverse(_currentViewMatrix, _modelMatrix);
    gx_common::gxMulMat4Mat4(_viewProjectionMatrix, _currentViewMatrix, _projectionMatrix);
    updatePlanes();
}

void Camera::setLocation(const gx_common::GXVec3& location)
{
    gx_common::gxSetMat4TranslateTo(_modelMatrix, location);
    gx_common::gxSetMat4Inverse(_currentViewMatrix, _modelMatrix);
    gx_common::gxMulMat4Mat4(_viewProjectionMatrix, _currentViewMatrix, _projectionMatrix);
    updatePlanes();
}

void Camera::getLocation(gx_common::GXVec3 location) const
{
    memcpy(location.arr, _modelMatrix.wv.arr, sizeof(gx_common::GXVec3));
}

void Camera::setRotation(float pitch_rad, float yaw_rad, float roll_rad)
{
    gx_common::GXVec3 location;
    memcpy(location.arr, _modelMatrix.wv.arr, sizeof(gx_common::GXVec3));

    gx_common::gxSetMat4RotationXYZ(_modelMatrix, pitch_rad, yaw_rad, roll_rad);
    memcpy(_modelMatrix.wv.arr, location.arr, sizeof(gx_common::GXVec3));

    gx_common::gxSetMat4Inverse(_currentViewMatrix, _modelMatrix);
    gx_common::gxMulMat4Mat4(_viewProjectionMatrix, _currentViewMatrix, _projectionMatrix);
    updatePlanes();
}

void Camera::setRotation(const gx_common::GXVec3& rotation)
{
    gx_common::GXVec3 location;
    memcpy(location.arr, _modelMatrix.wv.arr, sizeof(gx_common::GXVec3));

    gx_common::gxSetMat4RotationXYZ(_modelMatrix, rotation.pitch_rad, rotation.yaw_rad, rotation.roll_rad);
    memcpy(_modelMatrix.wv.arr, location.arr, sizeof(gx_common::GXVec3));

    gx_common::gxSetMat4Inverse(_currentViewMatrix, _modelMatrix);
    gx_common::gxMulMat4Mat4(_viewProjectionMatrix, _currentViewMatrix, _projectionMatrix);
    updatePlanes();
}

void Camera::setRotation(const gx_common::GXMat4& rotation)
{
    gx_common::GXVec3 location;
    memcpy(location.arr, _modelMatrix.wv.arr, sizeof(gx_common::GXVec3));

    _modelMatrix = rotation;
    memcpy(_modelMatrix.wv.arr, location.arr, sizeof(gx_common::GXVec3));

    gx_common::gxSetMat4Inverse(_currentViewMatrix, _modelMatrix);
    gx_common::gxMulMat4Mat4(_viewProjectionMatrix, _currentViewMatrix, _projectionMatrix);
    updatePlanes();
}

void Camera::setRotation(const gx_common::GXQuat& quaternion)
{
    gx_common::GXVec3 location;
    memcpy(location.arr, _modelMatrix.wv.arr, sizeof(gx_common::GXVec3));

    _modelMatrix.from(quaternion, location);

    gx_common::gxSetMat4Inverse(_currentViewMatrix, _modelMatrix);
    gx_common::gxMulMat4Mat4(_viewProjectionMatrix, _currentViewMatrix, _projectionMatrix);
    updatePlanes();
}

void Camera::getRotation(gx_common::GXMat4 rotation) const
{
    rotation = _modelMatrix;
    memset(rotation.wv.arr, 0, sizeof(gx_common::GXVec3));
}

void Camera::getRotation(gx_common::GXQuat quaternion)
{
    quaternion = gx_common::gxCreateQuat(_modelMatrix);
}

const gx_common::GXMat4& Camera::getModelMatrix() const
{
    return _modelMatrix;
}

const gx_common::GXMat4& Camera::getCurrentViewMatrix() const
{
    return _currentViewMatrix;
}

const gx_common::GXMat4& Camera::getLastFrameViewMatrix() const
{
    return _lastFrameViewMatrix;
}

const gx_common::GXMat4& Camera::getProjectionMatrix() const
{
    return _projectionMatrix;
}

const gx_common::GXMat4& Camera::getInverseProjectionMatrix() const
{
    return _invProjectionMatrix;
}

const gx_common::GXMat4& Camera::getViewProjectionMatrix() const
{
    return _viewProjectionMatrix;
}

bool Camera::isObjectVisible(const gx_common::GXAABB& objectBoundsWorld)
{
    return _clipPlanesWorld.isVisible(objectBoundsWorld);
}

void Camera::updateLastFrameViewMatrix()
{
    _lastFrameViewMatrix = _currentViewMatrix;
}

void Camera::setActiveCamera(Camera* camera)
{
    _activeCamera = camera;
}

Camera* Camera::getActiveCamera()
{
    return _activeCamera;
}

void Camera::updatePlanes()
{
    _clipPlanesWorld.from(_viewProjectionMatrix);
}

} // namespace gx_engine
