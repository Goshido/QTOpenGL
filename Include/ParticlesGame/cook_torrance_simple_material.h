#ifndef COOK_TORRANCE_SIMPLE_MATERIAL_H
#define COOK_TORRANCE_SIMPLE_MATERIAL_H

#include <GXEngine/material.h>

namespace particles_game {

class CookTorranceSimpleMaterial : public gx_engine::Material {
public:
    explicit CookTorranceSimpleMaterial();
    ~CookTorranceSimpleMaterial() override;

    void bind(const gx_engine::Renderable& activeRenderable) override;
    void unbind() override;

    void setAlbedo(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
    void setEmission(unsigned char red, unsigned char green, unsigned char blue, float intensity);
    void setRoughness(float roughness);
    void setIndexOfRefraction(float ior);
    void setSpecularIntensity(float intensity);
    void setMetallic(bool isMetal);

private:
    gx_common::GXVec4 _albedo;
    gx_common::GXVec3 _emission;
    gx_common::GXVec4 _cookTorranceParams;

    GLint _currentModelViewProjectionMatrixLocation;
    GLint _currentModelViewMatrixLocation;
    GLint _lastFrameModelViewMatrixLocation;
    GLint _albedoLocation;
    GLint _emissionLocation;
    GLint _cookTorranceParamsLocation;
};

} // namespace particles_game

#endif COOK_TORRANCE_SIMPLE_MATERIAL_H
