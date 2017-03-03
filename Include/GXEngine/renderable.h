#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <GXCommon/gx_math.h>

namespace gx_engine {

class Renderable {
public:
    Renderable();
    virtual ~Renderable();

    virtual void onDraw() = 0;

    void setLocation(float x, float y, float z);
    void setLocation(const gx_common::GXVec3& location);
    void getLocation(gx_common::GXVec3& location) const;

    void setRotation(float pitch_rad, float yaw_rad, float roll_rad);
    void setRotation(const gx_common::GXVec3& rotation);
    void setRotation(const gx_common::GXQuat& quaternion);
    void setRotation(const gx_common::GXMat4& rotation);
    void getRotation(gx_common::GXMat4& rotation) const;
    void getRotation(gx_common::GXQuat& quaternion) const;

    void setScale(float x, float y, float z);
    void setScale(const gx_common::GXVec3& scale);
    void getScale(gx_common::GXVec3& scale) const;

    const gx_common::GXMat4& getCurrentModelMatrix() const;
    const gx_common::GXMat4& getLastFrameModelMatrix() const;

protected:
    virtual void onTransformChanged();

public:
    void updateLastFrameModelMatrix();

    gx_common::GXMat4 _currentModelMatrix;
    gx_common::GXMat4 _lastFrameModelMatrix;
    gx_common::GXMat4 _scaleMatrix;
    gx_common::GXMat4 _rotationMatrix;
};

} // namespace gx_engine

#endif RENDERABLE_H
