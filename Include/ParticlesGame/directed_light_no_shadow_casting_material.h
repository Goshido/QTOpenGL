#ifndef DIRECTED_LIGHT_NO_SHADOW_CASTING_MATERIAL_H
#define DIRECTED_LIGHT_NO_SHADOW_CASTING_MATERIAL_H

#include <GXEngine/material.h>
#include <GXEngine/texture.h>

namespace particles_game {

class DirectedLightNoShadowCastingMaterial : public gx_engine::Material {
public:
    explicit DirectedLightNoShadowCastingMaterial();
    ~DirectedLightNoShadowCastingMaterial() override;

    void bind(const gx_engine::Renderable& activeRenderable) override;
    void unbind() override;

    void setLightDirectionView(const gx_common::GXVec3& directionView);
    void setColor(unsigned char red, unsigned char green, unsigned char blue, float intensity);
    void setViewerInverseProjectionMatrix(const gx_common::GXMat4& invProjectionMatrix);
    void setControlTextures(gx_engine::Texture& albedo, gx_engine::Texture& emission, gx_engine::Texture& normal,
                            gx_engine::Texture& cookTorrance, gx_engine::Texture& depth);

private:
    GLint _modelViewProjectionMatrixLocation;
    GLint _invScreenResolutionLocation;
    GLint _invProjectionMatrixLocation;
    GLint _hdrColorLocation;
    GLint _colorComponentsLocation;
    GLint _invLightDirectionViewLocation;

    gx_engine::Texture* _albedoTexture;
    gx_engine::Texture* _emissionTexture;
    gx_engine::Texture* _normalTexture;
    gx_engine::Texture* _cookTorranceTexture;
    gx_engine::Texture* _depthTexture;

    gx_common::GXVec2 _invScreenResolution;
    gx_common::GXVec3 _invLightDirectionView;
    const gx_common::GXMat4* _invProjectionMatrix;
    gx_common::GXVec3 _hdrColor;
    gx_common::GXVec4 _colorComponents;
};

} // namespace particles_game

#endif DIRECTED_LIGHT_NO_SHADOW_CASTING_MATERIAL_H
