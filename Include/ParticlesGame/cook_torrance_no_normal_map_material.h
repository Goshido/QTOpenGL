#ifndef COOK_TORRANCE_NO_NORMAL_MAP_MATERIAL_H
#define COOK_TORRANCE_NO_NORMAL_MAP_MATERIAL_H

#include <GXEngine/material.h>
#include <GXEngine/texture.h>

namespace particles_game {

class CookTorranceNoNormalMapMaterial : public gx_engine::Material {
public:
    explicit CookTorranceNoNormalMapMaterial();
    ~CookTorranceNoNormalMapMaterial() override;

    void bind(const gx_engine::Renderable& activeRenderable) override;
    void unbind() override;

    void setAlbedoTexture(gx_engine::Texture& albedo);
    void setAlbedoColor(unsigned char red, unsigned char green, unsigned char blue);
    void setAlbedoScale(float scaleU, float scaleV);
    void setAlbedoOffset(float offsetU, float offsetV);

    void setEmissionTexture(gx_engine::Texture& emission);
    void setEmissionColor(unsigned char red, unsigned char green, unsigned char blue);
    void setEmissionIntensity(float intensity);
    void setEmissionScale(float scaleU, float scaleV);
    void setEmissionOffset(float offsetU, float offsetV);

    // texture components:
    // red - roughess. Remapping to [0.0f, 1.0f] and take square.
    // green - index of refraction. Remapping [0.0f, 5.0f]
    // blue - specular intensity. Remapping to [0.0f, 1.0f]
    // alpha - metallic. Remapping rule: 1.0f if value > 129, 0.0f otherwise
    void setCookTorranceParamsTexture(gx_engine::Texture& cookTorranceParams);
    void setCookTorranceParamsScale(float scaleU, float scaleV);
    void setCookTorranceParamsOffset(float offsetU, float offsetV);
    void setRoughnessFactor(float factor);

private:
    GLint _currentModelViewProjectionMatrixLocation;
    GLint _currentModelViewMatrixLocation;
    GLint _lastFrameModelViewMatrixLocation;
    GLint _albedoColorLocation;
    GLint _albedoScaleOffsetLocation;
    GLint _emissionHDRColorLocation;
    GLint _emissionScaleOffsetLocation;
    GLint _roughnessFactorLocation;
    GLint _cookTorranceParamsScaleOffsetLocation;

    gx_engine::Texture* _albedo;
    gx_engine::Texture* _emission;
    gx_engine::Texture* _cookTorranceParams;

    gx_common::GXVec3 _albedoColor;
    gx_common::GXVec4 _albedoScaleOffset;
    gx_common::GXVec3 _emissionHDRColor;
    gx_common::GXVec3 _emissionColor;
    float _emissionIntensity;
    gx_common::GXVec4 _emissionScaleOffset;
    gx_common::GXVec4 _cookTorranceParamsScaleOffset;
    float _roughnessFactor;
};

} // namespace particles_game

#endif COOK_TORRANCE_NO_NORMAL_MAP_MATERIAL_H
