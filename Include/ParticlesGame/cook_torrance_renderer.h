#ifndef COOK_TORRANCE_RENDERER_H
#define COOK_TORRANCE_RENDERER_H

#include "static_mesh.h"
#include "unlit_material.h"
#include "light.h"
#include "directed_light_no_shadow_casting_material.h"
#include "blur_material.h"
#include <GXEngine/orthographic_camera.h>

#define RENDERER_ALBEDO_TARGET 0
#define RENDERER_EMISSION_TARGET 1
#define RENDERER_NORMAL_TARGET 2
#define RENDERER_COOK_TORRANCE_PARAMS_TARGET 3
#define RENDERER_VELOCITY_TARGET 4

namespace particles_game {

enum RenderTarget {
    Albedo,
    Normal,
    Emission,
    CookTorrance,
    Depth,
    HDR,
    Velocity,
    Combine
};

class CookTorranceRenderer {
public:
    ~CookTorranceRenderer();

    void beginGeometryPass();
    void renderMesh(gx_engine::Renderable& mesh, gx_engine::Material& material);

    void beginLightPass();
    void lightUp(const Light& light);

    void beginTonePass();
    void beginBlurPass();

    void presentFrame(RenderTarget target = RenderTarget::Combine);
    void resize(int width, int height);

    static CookTorranceRenderer* instance();

private:
    explicit CookTorranceRenderer();

    GLuint _fbo;
    gx_engine::Texture* _albedoTexture;
    gx_engine::Texture* _emissionTexture;
    gx_engine::Texture* _normalTexture;
    gx_engine::Texture* _cookTorranceTexture;
    gx_engine::Texture* _velocityTexture;
    gx_engine::Texture* _depthStencilTexture;
    gx_engine::Texture* _hdrTexture;
    gx_engine::Texture* _ldrTextureA;
    gx_engine::Texture* _ldrTextureB;

    StaticMesh _screenQuad;
    UnlitMaterial _unlitMaterial;
    DirectedLightNoShadowCastingMaterial _directedLightNoShadowCastingMaterial;
    BlurMaterial _blurMaterial;
    gx_engine::OrthographicCamera _hudCamera;

    static CookTorranceRenderer* _instance;
};

} // namespace particles_game

#endif COOK_TORRANCE_RENDERER_H
