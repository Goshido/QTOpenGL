#include <ParticlesGame/directed_light_no_shadow_casting_material.h>
#include <GXEngine/mesh_geometry.h>
#include <GXEngine/camera.h>

#define ALBEDO_SLOT 0
#define EMISSION_SLOT 1
#define NORMAL_SLOT 2
#define COOK_TORRANCE_SLOT 3
#define DEPTH_SLOT 4

#define FRAGMENT_TARGET 0

#define DEFAULT_LIGHT_DIRECTION_VIEW_X 0.0f
#define DEFAULT_LIGHT_DIRECTION_VIEW_Y -1.0f
#define DEFAULT_LIGHT_DIRECTION_VIEW_Z 0.0f

#define DEFAULT_COLOR_R 115
#define DEFAULT_COLOR_G 185
#define DEFAULT_COLOR_B 0
#define DEFAULT_COLOR_INTENSITY 1.0f

#define DEFAULT_INV_SCREEN_WIDTH 5.208333e-4f
#define DEFAULT_INV_SCREEN_HEIGHT 9.25925e-4f

namespace particles_game {

DirectedLightNoShadowCastingMaterial::DirectedLightNoShadowCastingMaterial()
    : _albedoTexture(nullptr)
    , _emissionTexture(nullptr)
    , _normalTexture(nullptr)
    , _cookTorranceTexture(nullptr)
    , _depthTexture(nullptr)
    , _invProjectionMatrix(nullptr)
{
    static const GLchar* attributeNames[2] = {"vertex", "uv"};
    static const GLuint attributeLocations[2] = {VERTEX_STREAM, UV_COORD_STREAM};

    static const GLchar* samplerNames[5] = {"albedoSampler", "emissionSampler", "normalSampler",
                                            "cookTorranceSampler", "depthSampler"};
    static const GLuint samplerLocations[5] = {ALBEDO_SLOT, EMISSION_SLOT, NORMAL_SLOT, COOK_TORRANCE_SLOT, DEPTH_SLOT};

    static const GLchar* fragmentNames[1] = {"fragmentColor"};
    static const GLuint fragmentLocations[1] = {FRAGMENT_TARGET};

    gx_engine::ShaderProgramInfo info;
    info.vs = "Shaders/ParticlesGame/SimpleMesh_vs.txt";
    info.fs = "Shaders/ParticlesGame/CookTorranceDirectedLightNoShadowCasting_fs.txt";
    info.numAttributes = 2;
    info.attributeNames = attributeNames;
    info.attributeLocations = attributeLocations;
    info.numSamplers = 5;
    info.samplerNames = samplerNames;
    info.samplerLocations = samplerLocations;
    info.numFragments = 1;
    info.fragmentNames = fragmentNames;
    info.fragmentLocations = fragmentLocations;

    _program = gx_engine::ShaderProgram::getShaderProgram(info);

    _modelViewProjectionMatrixLocation = _program->getUniform("mod_view_proj_mat");
    _invScreenResolutionLocation = _program->getUniform("invScreenResoluion");
    _invProjectionMatrixLocation = _program->getUniform("inv_proj_mat");
    _hdrColorLocation = _program->getUniform("hrdColor");
    _colorComponentsLocation = _program->getUniform("colorComponents");
    _invLightDirectionViewLocation = _program->getUniform("invLightDirectionView");

    gx_common::GXVec3 direction = gx_common::gxCreateVec3(DEFAULT_LIGHT_DIRECTION_VIEW_X,
                                                          DEFAULT_LIGHT_DIRECTION_VIEW_Y,
                                                          DEFAULT_LIGHT_DIRECTION_VIEW_Z);
    setLightDirectionView(direction);
    setColor(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B, DEFAULT_COLOR_INTENSITY);
    _invScreenResolution = gx_common::gxCreateVec2(DEFAULT_INV_SCREEN_WIDTH, DEFAULT_INV_SCREEN_HEIGHT);
}

DirectedLightNoShadowCastingMaterial::~DirectedLightNoShadowCastingMaterial()
{
    // NOTHING
}

void DirectedLightNoShadowCastingMaterial::bind(const gx_engine::Renderable& activeRenderable)
{
    glUseProgram(_program->getProgram());

    gx_common::GXMat4 modelViewProjectionMatrix;
    gx_common::gxMulMat4Mat4(modelViewProjectionMatrix, activeRenderable.getCurrentModelMatrix(),
                             gx_engine::Camera::getActiveCamera()->getViewProjectionMatrix());

    glUniformMatrix4fv(_modelViewProjectionMatrixLocation, 1, GL_FALSE, modelViewProjectionMatrix.arr);
    glUniform2fv(_invScreenResolutionLocation, 1, _invScreenResolution.arr);
    glUniformMatrix4fv(_invProjectionMatrixLocation, 1, GL_FALSE, _invProjectionMatrix->arr);
    glUniform4fv(_colorComponentsLocation, 1, _colorComponents.arr);
    glUniform3fv(_hdrColorLocation, 1, _hdrColor.arr);
    glUniform3fv(_invLightDirectionViewLocation, 1, _invLightDirectionView.arr);

    _albedoTexture->bind(ALBEDO_SLOT);
    _emissionTexture->bind(EMISSION_SLOT);
    _normalTexture->bind(NORMAL_SLOT);
    _cookTorranceTexture->bind(COOK_TORRANCE_SLOT);
    _depthTexture->bind(DEPTH_SLOT);
}

void DirectedLightNoShadowCastingMaterial::unbind()
{
    _albedoTexture->unbind();
    _emissionTexture->unbind();
    _normalTexture->unbind();
    _cookTorranceTexture->unbind();
    _depthTexture->unbind();
    glUseProgram(0);
}

void DirectedLightNoShadowCastingMaterial::setLightDirectionView(const gx_common::GXVec3& directionView)
{
    gx_common::gxMulVec3Scalar(_invLightDirectionView, directionView, -1.0f);
}

void DirectedLightNoShadowCastingMaterial::setColor(unsigned char red, unsigned char green,
                                                                unsigned char blue, float intensity)
{
    gx_common::gxColorToVec3(_hdrColor, red, green, blue);
    memcpy(_colorComponents.arr, _hdrColor.arr, 3 * sizeof(float));
    _colorComponents.w = intensity;

    gx_common::gxMulVec3Scalar(_hdrColor, _hdrColor, intensity);
}

void DirectedLightNoShadowCastingMaterial::setViewerInverseProjectionMatrix(const gx_common::GXMat4&
                                                                                        invProjectionMatrix)
{
    _invProjectionMatrix = &invProjectionMatrix;
}

void DirectedLightNoShadowCastingMaterial::setControlTextures(gx_engine::Texture& albedo, gx_engine::Texture& emission,
                                                              gx_engine::Texture& normal,
                                                              gx_engine::Texture& cookTorrance,
                                                              gx_engine::Texture& depth)
{
    _albedoTexture = &albedo;
    _emissionTexture = &emission;
    _normalTexture = &normal;
    _cookTorranceTexture = &cookTorrance;
    _depthTexture = &depth;

    _invScreenResolution = gx_common::gxCreateVec2(1.0f / (float)_albedoTexture->getWidth(),
                                                   1.0f / (float)_albedoTexture->getHeight());
}

} // namespace particles_game
