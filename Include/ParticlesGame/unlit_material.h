#ifndef UNLIT_MATERIAL_H
#define UNLIT_MATERIAL_H

#include <GXEngine/material.h>
#include <GXEngine/texture.h>

namespace particles_game {

class UnlitMaterial : public gx_engine::Material {
public:
    explicit UnlitMaterial();
    ~UnlitMaterial() override;

    void bind(const gx_engine::Renderable& activeRenderable) override;
    void unbind() override;

    void setTexture(gx_engine::Texture& diffuseTexture);
    void setMappingParams(float scaleU, float scaleV, float offsetU, float offsetV);

private:
    gx_engine::Texture* _diffuseTexture;
    GLint _mappingParamsLocation;
    GLint _modelViewProjectionMatrixLocation;
    gx_common::GXVec4 _mappingParams;    // x, y - scale. z, w - offset
};

} // namespace particles_game

#endif UNLIT_MATERIAL_H
