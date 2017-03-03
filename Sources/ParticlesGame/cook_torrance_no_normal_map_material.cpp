#include <ParticlesGame/cook_torrance_no_normal_map_material.h>
#include <ParticlesGame/cook_torrance_renderer.h>
#include <GXEngine/mesh_geometry.h>

#define DEFAULT_TEXTURE_SCALE_U 1.0f
#define DEFAULT_TEXTURE_SCALE_V 1.0f
#define DEFAULT_TEXTURE_OFFSET_U 0.0f
#define DEFAULT_TEXTURE_OFFSET_V 0.0f

#define DEFAULT_ALBEDO_R 255
#define DEFAULT_ALBEDO_G 255
#define DEFAULT_ALBEDO_B 255

#define DEFAULT_EMISSION_R 255
#define DEFAULT_EMISSION_G 255
#define DEFAULT_EMISSION_B 255
#define DEFAULT_EMISSION_INTENSITY 1.0f

#define DEFAULT_ROUGHNESS_FACTOR 1.0f

#define ALBEDO_SLOT 0
#define EMISSION_SLOT 1
#define COOK_TORRANCE_PARAMS_SLOT 2

#define VERTEX_SHADER "Shaders/ParticlesGame/VertexNormalUVVelocity_vs.txt"
#define FRAGMENT_SHADER "Shaders/ParticlesGame/CookTorranceNoNormalMap_fs.txt"

namespace particles_game {

CookTorranceNoNormalMapMaterial::CookTorranceNoNormalMapMaterial()
    : _albedo(nullptr)
    , _emission(nullptr)
    , _cookTorranceParams(nullptr)
{
    static const GLchar* attributeNames[3] = {"vertex", "uv", "normal"};
    static const GLuint attributeLocations[3] = {VERTEX_STREAM, UV_COORD_STREAM, NORMAL_STREAM};

    static const GLchar* samplerNames[3] = {"albedoSampler", "emissionSampler", "cookTorranceParamsSampler"};
    static const GLuint samplerLocations[3] = {ALBEDO_SLOT, EMISSION_SLOT, COOK_TORRANCE_PARAMS_SLOT};

    static const GLchar* fragmentNames[5] = {"albedoTarget", "emissionTarget", "normalTarget",
                                             "cookTorranceParamsTarget", "velocityTarget"};
    static const GLuint fragmentLocations[5] = {RENDERER_ALBEDO_TARGET, RENDERER_EMISSION_TARGET,
                                                RENDERER_NORMAL_TARGET, RENDERER_COOK_TORRANCE_PARAMS_TARGET,
                                                RENDERER_VELOCITY_TARGET};

    gx_engine::ShaderProgramInfo info;
    info.vs = VERTEX_SHADER;
    info.fs = FRAGMENT_SHADER;
    info.numAttributes = 3;
    info.attributeNames = attributeNames;
    info.attributeLocations = attributeLocations;
    info.numSamplers = 3;
    info.samplerNames = samplerNames;
    info.samplerLocations = samplerLocations;
    info.numFragments = 5;
    info.fragmentNames = fragmentNames;
    info.fragmentLocations = fragmentLocations;

    _program = gx_engine::ShaderProgram::getShaderProgram(info);

    _currentModelViewProjectionMatrixLocation = _program->getUniform("current_mod_view_proj_mat");
    _currentModelViewMatrixLocation = _program->getUniform("current_mod_view_mat");
    _lastFrameModelViewMatrixLocation = _program->getUniform("last_frame_mod_view_mat");
    _albedoColorLocation = _program->getUniform("albedoColor");
    _albedoScaleOffsetLocation = _program->getUniform("albedoScaleOffset");
    _emissionHDRColorLocation = _program->getUniform("emissionHDRColor");
    _emissionScaleOffsetLocation = _program->getUniform("emissionScaleOffset");
    _roughnessFactorLocation = _program->getUniform("rougnessFactor");
    _cookTorranceParamsScaleOffsetLocation = _program->getUniform("cookTorranceParamsScaleOffset");

    setAlbedoColor(DEFAULT_ALBEDO_R, DEFAULT_ALBEDO_G, DEFAULT_ALBEDO_B);
    setAlbedoScale(DEFAULT_TEXTURE_SCALE_U, DEFAULT_TEXTURE_SCALE_V);
    setAlbedoOffset(DEFAULT_TEXTURE_OFFSET_U, DEFAULT_TEXTURE_OFFSET_V);

    _emissionIntensity = DEFAULT_EMISSION_INTENSITY;
    setEmissionColor(DEFAULT_EMISSION_R, DEFAULT_EMISSION_G, DEFAULT_EMISSION_B);
    setEmissionScale(DEFAULT_TEXTURE_SCALE_U, DEFAULT_TEXTURE_SCALE_V);
    setEmissionOffset(DEFAULT_TEXTURE_OFFSET_U, DEFAULT_TEXTURE_OFFSET_V);

    setRoughnessFactor(DEFAULT_ROUGHNESS_FACTOR);
    setCookTorranceParamsScale(DEFAULT_TEXTURE_SCALE_U, DEFAULT_TEXTURE_SCALE_V);
    setCookTorranceParamsOffset(DEFAULT_TEXTURE_OFFSET_U, DEFAULT_TEXTURE_OFFSET_V);
}

CookTorranceNoNormalMapMaterial::~CookTorranceNoNormalMapMaterial()
{
    // NOTHING
}

void CookTorranceNoNormalMapMaterial::bind(const gx_engine::Renderable& activeRenderable)
{
    if (!_albedo || !_emission || !_cookTorranceParams) return;

    glUseProgram(_program->getProgram());

    gx_common::GXMat4 currentModelViewMatrix;
    gx_common::GXMat4 currentModelViewProjectionMatrix;
    gx_common::GXMat4 lastFrameModelViewMatrix;

    gx_engine::Camera* camera = gx_engine::Camera::getActiveCamera();
    gx_common::gxMulMat4Mat4(currentModelViewMatrix, activeRenderable.getCurrentModelMatrix(),
                             camera->getCurrentViewMatrix());
    gx_common::gxMulMat4Mat4(currentModelViewProjectionMatrix, currentModelViewMatrix, camera->getProjectionMatrix());
    gx_common::gxMulMat4Mat4(lastFrameModelViewMatrix, activeRenderable.getLastFrameModelMatrix(),
                             camera->getLastFrameViewMatrix());

    glUniformMatrix4fv(_currentModelViewMatrixLocation, 1, GL_FALSE, currentModelViewMatrix.arr);
    glUniformMatrix4fv(_currentModelViewProjectionMatrixLocation, 1, GL_FALSE, currentModelViewProjectionMatrix.arr);
    glUniformMatrix4fv(_lastFrameModelViewMatrixLocation, 1, GL_FALSE, lastFrameModelViewMatrix.arr);
    glUniform3fv(_albedoColorLocation, 1, _albedoColor.arr);
    glUniform4fv(_albedoScaleOffsetLocation, 1, _albedoScaleOffset.arr);
    glUniform3fv(_emissionHDRColorLocation, 1, _emissionHDRColor.arr);
    glUniform4fv(_emissionScaleOffsetLocation, 1, _emissionScaleOffset.arr);
    glUniform1f(_roughnessFactorLocation, _roughnessFactor);
    glUniform4fv(_cookTorranceParamsScaleOffsetLocation, 1, _cookTorranceParamsScaleOffset.arr);

    _albedo->bind(ALBEDO_SLOT);
    _emission->bind(EMISSION_SLOT);
    _cookTorranceParams->bind(COOK_TORRANCE_PARAMS_SLOT);
}

void CookTorranceNoNormalMapMaterial::unbind()
{
    if (!_albedo || !_emission || !_cookTorranceParams) return;

    _albedo->unbind();
    _emission->unbind();
    _cookTorranceParams->unbind();

    glUseProgram(0);
}

void CookTorranceNoNormalMapMaterial::setAlbedoTexture(gx_engine::Texture& albedo)
{
    _albedo = &albedo;
}

void CookTorranceNoNormalMapMaterial::setAlbedoColor(unsigned char red, unsigned char green, unsigned char blue)
{
    gx_common::gxColorToVec3(_albedoColor, red, green, blue);
}

void CookTorranceNoNormalMapMaterial::setAlbedoScale(float scaleU, float scaleV)
{
    _albedoScaleOffset.x = scaleU;
    _albedoScaleOffset.y = scaleV;
}

void CookTorranceNoNormalMapMaterial::setAlbedoOffset(float offsetU, float offsetV)
{
    _albedoScaleOffset.z = offsetU;
    _albedoScaleOffset.w = offsetV;
}

void CookTorranceNoNormalMapMaterial::setEmissionTexture(gx_engine::Texture& emission)
{
    _emission = &emission;
}

void CookTorranceNoNormalMapMaterial::setEmissionColor(unsigned char red, unsigned char green, unsigned char blue)
{
    gx_common::gxColorToVec3(_emissionColor, red, green, blue);
    gx_common::gxMulVec3Scalar(_emissionHDRColor, _emissionColor, _emissionIntensity);
}

void CookTorranceNoNormalMapMaterial::setEmissionIntensity(float intensity)
{
    _emissionIntensity = intensity;
    gx_common::gxMulVec3Scalar(_emissionHDRColor, _emissionColor, _emissionIntensity);
}

void CookTorranceNoNormalMapMaterial::setEmissionScale(float scaleU, float scaleV)
{
    _emissionScaleOffset.x = scaleU;
    _emissionScaleOffset.y = scaleV;
}

void CookTorranceNoNormalMapMaterial::setEmissionOffset(float offsetU, float offsetV)
{
    _emissionScaleOffset.z = offsetU;
    _emissionScaleOffset.w = offsetV;
}

void CookTorranceNoNormalMapMaterial::setCookTorranceParamsTexture(gx_engine::Texture& cookTorranceParams)
{
    _cookTorranceParams = &cookTorranceParams;
}

void CookTorranceNoNormalMapMaterial::setCookTorranceParamsScale(float scaleU, float scaleV)
{
    _cookTorranceParamsScaleOffset.x = scaleU;
    _cookTorranceParamsScaleOffset.y = scaleV;
}

void CookTorranceNoNormalMapMaterial::setCookTorranceParamsOffset(float offsetU, float offsetV)
{
    _cookTorranceParamsScaleOffset.z = offsetU;
    _cookTorranceParamsScaleOffset.w = offsetV;
}

void CookTorranceNoNormalMapMaterial::setRoughnessFactor(float factor)
{
    _roughnessFactor = factor;
}

} // namespace particles_game
