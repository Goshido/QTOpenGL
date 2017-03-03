#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "camera.h"

namespace gx_engine {

class PerspectiveCamera : public Camera {
public:
    explicit PerspectiveCamera(float fovy_rad, float aspect, float zNear, float zFar);
    ~PerspectiveCamera() override;

    void setFOV(float fovy_rad);
    void setAspect(float aspect);
    void setZLimits(float zNear, float zFar);

    float getFOV() const;
    float getAspect() const;
    float getZNear() const;
    float getZFar() const;

private:
    float _fovy;
    float _aspect;
    float _zNear;
    float _zFar;
};

} // gx_engine

#endif PERSPECTIVE_CAMERA_H
