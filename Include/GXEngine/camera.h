#ifndef CAMERA_H
#define CAMERA_H

#include <GXCommon/gx_math.h>

namespace gx_engine {

class Camera {
public:
    explicit Camera();
    virtual ~Camera();

    void setLocation(float x, float y, float z);
    void setLocation(const gx_common::GXVec3& location);
    void getLocation(gx_common::GXVec3 location) const;

    void setRotation(float pitch_rad, float yaw_rad, float roll_rad);
    void setRotation(const gx_common::GXVec3& rotation);
    void setRotation(const gx_common::GXMat4& rotation);
    void setRotation(const gx_common::GXQuat& quaternion);
    void getRotation(gx_common::GXMat4 rotation) const;
    void getRotation(gx_common::GXQuat quaternion);

    const gx_common::GXMat4& getModelMatrix() const;
    const gx_common::GXMat4& getCurrentViewMatrix() const;
    const gx_common::GXMat4& getLastFrameViewMatrix() const;
    const gx_common::GXMat4& getProjectionMatrix() const;
    const gx_common::GXMat4& getInverseProjectionMatrix() const;
    const gx_common::GXMat4& getViewProjectionMatrix() const;

    bool isObjectVisible(const gx_common::GXAABB& objectBoundsWorld);

    void updateLastFrameViewMatrix();

    static void setActiveCamera(Camera* camera);
    static Camera* getActiveCamera();

protected:
    void updatePlanes();

    gx_common::GXMat4 _modelMatrix;
    gx_common::GXMat4 _currentViewMatrix;
    gx_common::GXMat4 _lastFrameViewMatrix;
    gx_common::GXMat4 _projectionMatrix;
    gx_common::GXMat4 _invProjectionMatrix;
    gx_common::GXMat4 _viewProjectionMatrix;
    gx_common::GXProjectionClipPlanes _clipPlanesWorld;
    static Camera* _activeCamera;
};

} // namespace gx_engine

#endif CAMERA_H
