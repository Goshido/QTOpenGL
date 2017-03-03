#include <GXEngine/perspective_camera.h>
#include <GXCommon/gx_math.h>

namespace gx_engine {

PerspectiveCamera::PerspectiveCamera(float fovy_rad, float aspect, float zNear, float zFar)
    : _fovy(fovy_rad)
    , _aspect(aspect)
    , _zNear(zNear)
    , _zFar(zFar)
{
    gx_common::gxSetMat4Perspective(_projectionMatrix, _fovy, _aspect, _zNear, _zFar);
    gx_common::gxSetMat4Inverse(_invProjectionMatrix, _projectionMatrix);
    gx_common::gxMulMat4Mat4(_viewProjectionMatrix, _currentViewMatrix, _projectionMatrix);
    updatePlanes();
}

PerspectiveCamera::~PerspectiveCamera()
{
    // NOTHING
}

void PerspectiveCamera::setFOV(float fovy_rad)
{
    _fovy = fovy_rad;
    gx_common::gxSetMat4Perspective(_projectionMatrix, _fovy, _aspect, _zNear, _zFar);
    gx_common::gxSetMat4Inverse(_invProjectionMatrix, _projectionMatrix);
    gx_common::gxMulMat4Mat4(_viewProjectionMatrix, _currentViewMatrix, _projectionMatrix);
    updatePlanes();
}

void PerspectiveCamera::setAspect(float aspect)
{
    _aspect = aspect;
    gx_common::gxSetMat4Perspective(_projectionMatrix, _fovy, _aspect, _zNear, _zFar);
    gx_common::gxSetMat4Inverse(_invProjectionMatrix, _projectionMatrix);
    gx_common::gxMulMat4Mat4(_viewProjectionMatrix, _currentViewMatrix, _projectionMatrix);
    updatePlanes();
}

void PerspectiveCamera::setZLimits(float zNear, float zFar)
{
    _zNear = zNear;
    _zFar = zFar;
    gx_common::gxSetMat4Perspective(_projectionMatrix, _fovy, _aspect, _zNear, _zFar);
    gx_common::gxSetMat4Inverse(_invProjectionMatrix, _projectionMatrix);
    gx_common::gxMulMat4Mat4(_viewProjectionMatrix, _currentViewMatrix, _projectionMatrix);
    updatePlanes();
}

float PerspectiveCamera::getFOV() const
{
    return _fovy;
}

float PerspectiveCamera::getAspect() const
{
    return _aspect;
}

float PerspectiveCamera::getZNear() const
{
    return _zNear;
}

float PerspectiveCamera::getZFar() const
{
    return _zFar;
}

} // namespace gx_engine
