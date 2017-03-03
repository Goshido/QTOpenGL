#include <GXEngine/orthographic_camera.h>

namespace gx_engine {

OrthographicCamera::OrthographicCamera(float width, float height, float zNear, float zFar)
    : _width(width)
    , _height(height)
    , _zNear(zNear)
    , _zFar(zFar)
{
    gx_common::gxSetMat4Ortho(_projectionMatrix, _width, _height, _zNear, _zFar);
    gx_common::gxSetMat4Inverse(_invProjectionMatrix, _projectionMatrix);
    gx_common::gxMulMat4Mat4(_viewProjectionMatrix, _currentViewMatrix, _projectionMatrix);
    updatePlanes();
}

OrthographicCamera::~OrthographicCamera()
{
    // NOTHING
}

void OrthographicCamera::setResolution(float width, float height)
{
    _width = width;
    _height = height;
    gx_common::gxSetMat4Ortho(_projectionMatrix, _width, _height, _zNear, _zFar);
    gx_common::gxSetMat4Inverse(_invProjectionMatrix, _projectionMatrix);
    gx_common::gxMulMat4Mat4(_viewProjectionMatrix, _currentViewMatrix, _projectionMatrix);
    updatePlanes();
}

void OrthographicCamera::setZLimits(float zNear, float zFar)
{
    _zNear = zNear;
    _zFar = zFar;
    gx_common::gxSetMat4Ortho(_projectionMatrix, _width, _height, _zNear, _zFar);
    gx_common::gxSetMat4Inverse(_invProjectionMatrix, _projectionMatrix);
    gx_common::gxMulMat4Mat4(_viewProjectionMatrix, _currentViewMatrix, _projectionMatrix);
    updatePlanes();
}

float OrthographicCamera::getWidth() const
{
    return _width;
}

float OrthographicCamera::getHeight() const
{
    return _height;
}

float OrthographicCamera::getZNear() const
{
    return _zNear;
}

float OrthographicCamera::getZFar() const
{
    return _zFar;
}

} // namespace gx_engine
