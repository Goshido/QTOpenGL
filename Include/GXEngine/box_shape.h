#ifndef BOX_SHAPE_H
#define BOX_SHAPE_H

#include "physics_shape.h"

namespace gx_engine {

class BoxShape : public PhysicsShape {
public:
    BoxShape();
    ~BoxShape() override;

    void getExtremePoint(gx_common::GXVec3& point, const gx_common::GXMat4& transform,
                         const gx_common::GXVec3& direction) const override;

    void setWidth(float width);     // m
    void setHeight(float height);   // m
    void setDepth(float depth);     // m

protected:
    void updateMassAndInertiaTensor() override;

private:
    float _width;
    float _height;
    float _depth;
};

} // namespace gx_engine

#endif BOX_SHAPE_H
