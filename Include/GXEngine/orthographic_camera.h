#ifndef ORTHOGRAPHIC_CAMERA_H
#define ORTHOGRAPHIC_CAMERA_H

#include "camera.h"

namespace gx_engine {

class OrthographicCamera : public Camera {
public:
    explicit OrthographicCamera(float width, float height, float zNear, float zFar);
    ~OrthographicCamera() override;

    void setResolution(float width, float height);
    void setZLimits(float zNear, float zFar);

    float getWidth() const;
    float getHeight() const;
    float getZNear() const;
    float getZFar() const;

private:
    float _width;
    float _height;
    float _zNear;
    float _zFar;
};

} // namespace gx_engine

#endif ORTHOGRAPHIC_CAMERA_H
