#include <ParticlesGame/cook_torrance_renderer.h>
#include <ParticlesGame/directed_light.h>
#include <GXEngine/renderer.h>
#include <GXCommon/gx_memory.h>
#include <GXCommon/gx_log.h>

#define SCREEN_QUAD_MESH "3D Models/System/ScreenQuad.obj"
#define DEFAULT_WIDTH 1920
#define DEFAULT_HEIGHT 1080
#define DEFAULT_Z_NEAR 0.0f
#define DEFAULT_Z_FAR 100.0f
#define DEFAULT_SCREEN_QUAD_Z 1.0f
#define BLUR_STRENGTH 0.375f
#define BLUR_VELOCITY_MINIMUM 0.002f

namespace particles_game {

CookTorranceRenderer* CookTorranceRenderer::_instance = nullptr;

CookTorranceRenderer::~CookTorranceRenderer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &_fbo);

    delete _albedoTexture;
    delete _emissionTexture;
    delete _normalTexture;
    delete _cookTorranceTexture;
    delete _velocityTexture;
    delete _depthStencilTexture;
    delete _hdrTexture;
    delete _ldrTextureA;
    delete _ldrTextureB;

    _instance = nullptr;
}

void CookTorranceRenderer::beginGeometryPass()
{
    if (!_hdrTexture) {
        gx_engine::Renderer* renderer = gx_engine::Renderer::instance();
        unsigned short w = (unsigned short)renderer->getWidth();
        unsigned short h = (unsigned short)renderer->getHeight();

        _albedoTexture = new gx_engine::Texture(w, h, GL_RGBA8);
        _emissionTexture = new gx_engine::Texture(w, h, GL_RGBA16F);
        _normalTexture = new gx_engine::Texture(w, h, GL_RGB16);
        _cookTorranceTexture = new gx_engine::Texture(w, h, GL_RGBA8);
        _velocityTexture = new gx_engine::Texture(w, h, GL_RGBA16F);
        _depthStencilTexture = new gx_engine::Texture(w, h, GL_DEPTH24_STENCIL8);
        _hdrTexture = new gx_engine::Texture(w, h, GL_RGBA16F);
        _ldrTextureA = new gx_engine::Texture(w, h, GL_RGBA8);
        _ldrTextureB = new gx_engine::Texture(w, h, GL_RGBA8);

        _albedoTexture->disableResampling();
        _emissionTexture->disableResampling();
        _normalTexture->disableResampling();
        _cookTorranceTexture->disableResampling();
        _velocityTexture->disableResampling();
        _depthStencilTexture->disableResampling();
        _hdrTexture->disableResampling();
        _ldrTextureA->disableResampling();
        _ldrTextureB->disableResampling();

        _directedLightNoShadowCastingMaterial.setControlTextures(*_albedoTexture, *_emissionTexture, *_normalTexture,
                                                                 *_cookTorranceTexture, *_depthStencilTexture);
        _blurMaterial.setControlTextures(*_ldrTextureA, *_velocityTexture, *_depthStencilTexture);
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + RENDERER_ALBEDO_TARGET,
                           GL_TEXTURE_2D, _albedoTexture->getTextureObject(), 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + RENDERER_EMISSION_TARGET,
                           GL_TEXTURE_2D, _emissionTexture->getTextureObject(), 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + RENDERER_NORMAL_TARGET,
                           GL_TEXTURE_2D, _normalTexture->getTextureObject(), 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + RENDERER_COOK_TORRANCE_PARAMS_TARGET,
                           GL_TEXTURE_2D, _cookTorranceTexture->getTextureObject(), 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + RENDERER_VELOCITY_TARGET,
                           GL_TEXTURE_2D, _velocityTexture->getTextureObject(), 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                           GL_TEXTURE_2D, _depthStencilTexture->getTextureObject(), 0);

    static const GLenum buffers[5] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
                                      GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};
    glDrawBuffers(5, buffers);

    glDepthMask(GL_TRUE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        gx_common::gxLogA("CookTorranceRenderer::beginGeometryPass::Warning - Something wrong with FBO\n");
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CookTorranceRenderer::renderMesh(gx_engine::Renderable& mesh, gx_engine::Material& material)
{
    material.bind(mesh);
    mesh.onDraw();
    material.unbind();
}

void CookTorranceRenderer::beginLightPass()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, _hdrTexture->getTextureObject(), 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                           GL_TEXTURE_2D, _depthStencilTexture->getTextureObject(), 0);

    static const GLenum buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, buffers);

    glDepthMask(GL_FALSE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        gx_common::gxLogA("CookTorranceRenderer::beginLightPass::Warning - Something wrong with FBO\n");
    }

    glClear(GL_COLOR_BUFFER_BIT);
}

void CookTorranceRenderer::lightUp(const Light& light)
{
    gx_engine::Camera* oldCamera = gx_engine::Camera::getActiveCamera();
    gx_engine::Camera::setActiveCamera(&_hudCamera);

    switch (light.getType()) {
        case LightType::Directed: {
            unsigned char r = 0;
            unsigned char g = 0;
            unsigned char b = 0;

            const DirectedLight& directedLight = (const DirectedLight&)light;
            directedLight.getColor(r, g, b);
            _directedLightNoShadowCastingMaterial.setColor(r, g, b, directedLight.getIntensity());
            _directedLightNoShadowCastingMaterial.setViewerInverseProjectionMatrix(
                        oldCamera->getInverseProjectionMatrix());

            const gx_common::GXMat4& modelMatrix = directedLight.getRotation();
            gx_common::GXVec3 directionView;
            gx_common::gxMulVec3Mat4AsNormal(directionView, modelMatrix.zv, oldCamera->getCurrentViewMatrix());
            _directedLightNoShadowCastingMaterial.setLightDirectionView(directionView);

            _directedLightNoShadowCastingMaterial.bind(_screenQuad);
            _screenQuad.onDraw();
            _directedLightNoShadowCastingMaterial.unbind();
            break;
        }
        case LightType::Bulp:
            // TODO
            break;
        case LightType::Spot:
            // TODO
            break;
    }

    gx_engine::Camera::setActiveCamera(oldCamera);
}

void CookTorranceRenderer::beginTonePass()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, _ldrTextureA->getTextureObject(), 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

    static const GLenum buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, buffers);

    glDepthMask(GL_FALSE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClear(GL_COLOR_BUFFER_BIT);

    gx_engine::Camera* oldCamera = gx_engine::Camera::getActiveCamera();
    gx_engine::Camera::setActiveCamera(&_hudCamera);

    _unlitMaterial.setTexture(*_hdrTexture);
    renderMesh(_screenQuad, _unlitMaterial);

    gx_engine::Camera::setActiveCamera(oldCamera);
}

void CookTorranceRenderer::beginBlurPass()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, _ldrTextureB->getTextureObject(), 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

    static const GLenum buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, buffers);

    glDepthMask(GL_FALSE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClear(GL_COLOR_BUFFER_BIT);

    gx_engine::Camera* oldCamera = gx_engine::Camera::getActiveCamera();
    gx_engine::Camera::setActiveCamera(&_hudCamera);

    _blurMaterial.setInverseViewerProjectionMatrix(oldCamera->getInverseProjectionMatrix());
    renderMesh(_screenQuad, _blurMaterial);

    gx_engine::Camera::setActiveCamera(oldCamera);
}

void CookTorranceRenderer::presentFrame(RenderTarget target)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glDepthMask(GL_FALSE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        gx_common::gxLogA("CookTorranceRenderer::presentFrame::Warning - Something wrong with FBO\n");
    }

    switch (target) {
        case RenderTarget::Albedo:
            _unlitMaterial.setTexture(*_albedoTexture);
            break;
        case RenderTarget::Normal:
            _unlitMaterial.setTexture(*_normalTexture);
            break;
        case RenderTarget::Emission:
            _unlitMaterial.setTexture(*_emissionTexture);
            break;
        case RenderTarget::CookTorrance:
            _unlitMaterial.setTexture(*_cookTorranceTexture);
            break;
        case RenderTarget::Depth:
            _unlitMaterial.setTexture(*_depthStencilTexture);
            break;
        case RenderTarget::HDR:
            _unlitMaterial.setTexture(*_hdrTexture);
            break;
        case RenderTarget::Velocity:
            _unlitMaterial.setTexture(*_velocityTexture);
            break;
        case RenderTarget::Combine:
            _unlitMaterial.setTexture(*_ldrTextureB);
            break;
    }

    gx_engine::Camera* oldCam = gx_engine::Camera::getActiveCamera();
    gx_engine::Camera::setActiveCamera(&_hudCamera);

    renderMesh(_screenQuad, _unlitMaterial);

    gx_engine::Camera::setActiveCamera(oldCam);
}

void CookTorranceRenderer::resize(int width, int height)
{
    if (_hdrTexture && (int)_hdrTexture->getWidth() == width && (int)_hdrTexture->getHeight() == height) return;

    gxSafeDelete(_albedoTexture);
    gxSafeDelete(_emissionTexture);
    gxSafeDelete(_normalTexture);
    gxSafeDelete(_cookTorranceTexture);
    gxSafeDelete(_velocityTexture);
    gxSafeDelete(_depthStencilTexture);
    gxSafeDelete(_hdrTexture);
    gxSafeDelete(_ldrTextureA);
    gxSafeDelete(_ldrTextureB);

    glViewport(0, 0, width, height);
    _screenQuad.setScale((float)width, (float)height, 1.0f);
    _hudCamera.setResolution((float)width, (float)height);
}

CookTorranceRenderer* CookTorranceRenderer::instance()
{
    if (!_instance) {
        _instance = new CookTorranceRenderer();
    }

    return _instance;
}

CookTorranceRenderer::CookTorranceRenderer()
    : _albedoTexture(nullptr)
    , _emissionTexture(nullptr)
    , _normalTexture(nullptr)
    , _cookTorranceTexture(nullptr)
    , _velocityTexture(nullptr)
    , _depthStencilTexture(nullptr)
    , _hdrTexture(nullptr)
    , _ldrTextureA(nullptr)
    , _ldrTextureB(nullptr)
    , _screenQuad(SCREEN_QUAD_MESH)
    , _hudCamera(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_Z_NEAR, DEFAULT_Z_FAR)
{
    glGenFramebuffers(1, &_fbo);

    gx_engine::Renderer* engineRenderer = gx_engine::Renderer::instance();
    resize(engineRenderer->getWidth(), engineRenderer->getHeight());

    _screenQuad.setLocation(0.0f, 0.0f, DEFAULT_SCREEN_QUAD_Z);
    _blurMaterial.setBlurStrength(BLUR_STRENGTH);
    _blurMaterial.setBlurVelocityMinimum(BLUR_VELOCITY_MINIMUM);
}

} // namespace particles_game
