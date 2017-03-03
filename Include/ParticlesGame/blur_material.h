#ifndef BLUR_MATERIAL_H
#define BLUR_MATERIAL_H

#include <GXEngine/material.h>
#include <GXEngine/texture.h>

namespace particles_game {

class BlurMaterial : public gx_engine::Material {
public:
    explicit BlurMaterial();
    ~BlurMaterial() override;

    void bind(const gx_engine::Renderable& activeRenderable) override;
    void unbind() override;

    void setBlurStrength(float strength);
    void setBlurVelocityMinimum(float velocity);
    void setInverseViewerProjectionMatrix(const gx_common::GXMat4& matrix);
    void setControlTextures(gx_engine::Texture& image, gx_engine::Texture& velocity, gx_engine::Texture& depth);

private:
    GLint _modelViewProjectionMatrixLocation;
    GLint _paramsLocation;
    GLint _invViewerProjectionMatrixLocation;

    gx_engine::Texture* _image;
    gx_engine::Texture* _velocity;
    gx_engine::Texture* _depth;
    gx_common::GXVec4 _params;
    const gx_common::GXMat4* _invViewerProjectionMatrix;
};

} // namespace particles_game

#endif BLUR_MATERIAL_H
