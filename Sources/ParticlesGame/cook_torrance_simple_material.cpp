#include <ParticlesGame/cook_torrance_simple_material.h>
#include <ParticlesGame/cook_torrance_renderer.h>
#include <GXEngine/mesh_geometry.h>
#include <GXEngine/camera.h>

#define DEFAULT_COLOR_R 115
#define DEFAULT_COLOR_G 185
#define DEFAULT_COLOR_B 0
#define DEFAULT_COLOR_A 255

#define DEFAULT_EMISSION_R 255
#define DEFAULT_EMISSION_G 255
#define DEFAULT_EMISSION_B 255
#define DEFAULT_EMISSION_INTENSITY 0.0f

#define DEFAULT_ROUGHNESS 0.2f
#define DEFAULT_INDEX_OF_REFRACTION 0.8f
#define DEFAULT_SPECULAR_INTENCITY 0.3f
#define DEFAULT_METALIC 1.0f

#define VERTEX_SHADER "Shaders/ParticlesGame/VertexNormalVelocity_vs.txt"
#define FRAGMENT_SHADER "Shaders/ParticlesGame/CookTorranceSimple_fs.txt"

namespace particles_game {

CookTorranceSimpleMaterial::CookTorranceSimpleMaterial()
{
    static const GLchar* attributeNames[2] = {"vertex", "normal"};
    static const GLuint attributeLocations[2] = {VERTEX_STREAM, NORMAL_STREAM};

    const GLchar* fragmentNames[5] = {"albedoTarget", "emissionTarget", "normalTarget",
                                      "cookTorranceParamsTarget", "velocityTarget"};
    const GLuint fragmentLocations[5] = {RENDERER_ALBEDO_TARGET, RENDERER_EMISSION_TARGET, RENDERER_NORMAL_TARGET,
                                         RENDERER_COOK_TORRANCE_PARAMS_TARGET, RENDERER_VELOCITY_TARGET};

    gx_engine::ShaderProgramInfo info;
    info.vs = VERTEX_SHADER;
    info.fs = FRAGMENT_SHADER;
    info.numAttributes = 2;
    info.attributeNames = attributeNames;
    info.attributeLocations = attributeLocations;
    info.numFragments = 5;
    info.fragmentNames = fragmentNames;
    info.fragmentLocations = fragmentLocations;

    _program = gx_engine::ShaderProgram::getShaderProgram(info);

    _currentModelViewProjectionMatrixLocation = _program->getUniform("current_mod_view_proj_mat");
    _currentModelViewMatrixLocation = _program->getUniform("current_mod_view_mat");
    _lastFrameModelViewMatrixLocation = _program->getUniform("last_frame_mod_view_mat");
    _albedoLocation = _program->getUniform("albedo");
    _emissionLocation = _program->getUniform("emission");
    _cookTorranceParamsLocation = _program->getUniform("cookTorranceParams");

    setAlbedo(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B, DEFAULT_COLOR_A);
    setEmission(DEFAULT_EMISSION_R, DEFAULT_EMISSION_G, DEFAULT_EMISSION_B, DEFAULT_EMISSION_INTENSITY);
    setRoughness(DEFAULT_ROUGHNESS);
    setIndexOfRefraction(DEFAULT_INDEX_OF_REFRACTION);
    setSpecularIntensity(DEFAULT_SPECULAR_INTENCITY);
    setMetallic(DEFAULT_METALIC);
}

CookTorranceSimpleMaterial::~CookTorranceSimpleMaterial()
{
    // NOTHING
}

void CookTorranceSimpleMaterial::bind(const gx_engine::Renderable& activeRenderable)
{
    gx_engine::Camera* camera = gx_engine::Camera::getActiveCamera();

    gx_common::GXMat4 currentModelViewMatrix;
    gx_common::GXMat4 currentModelViewProjectionMatrix;
    gx_common::GXMat4 lastFrameModelViewMatrix;

    gx_common::gxMulMat4Mat4(currentModelViewMatrix, activeRenderable.getCurrentModelMatrix(),
                             camera->getCurrentViewMatrix());
    gx_common::gxMulMat4Mat4(currentModelViewProjectionMatrix, currentModelViewMatrix, camera->getProjectionMatrix());
    gx_common::gxMulMat4Mat4(lastFrameModelViewMatrix, activeRenderable.getLastFrameModelMatrix(),
                             camera->getLastFrameViewMatrix());

    glUseProgram(_program->getProgram());

    glUniformMatrix4fv(_currentModelViewMatrixLocation, 1, GL_FALSE, currentModelViewMatrix.arr);
    glUniformMatrix4fv(_currentModelViewProjectionMatrixLocation, 1, GL_FALSE, currentModelViewProjectionMatrix.arr);
    glUniformMatrix4fv(_lastFrameModelViewMatrixLocation, 1, GL_FALSE, lastFrameModelViewMatrix.arr);
    glUniform4fv(_albedoLocation, 1, _albedo.arr);
    glUniform3fv(_emissionLocation, 1, _emission.arr);
    glUniform4fv(_cookTorranceParamsLocation, 1, _cookTorranceParams.arr);
}

void CookTorranceSimpleMaterial::unbind()
{
    glUseProgram(0);
}

void CookTorranceSimpleMaterial::setAlbedo(unsigned char red, unsigned char green,
                                           unsigned char blue, unsigned char alpha)
{
    gx_common::gxColorToVec4(_albedo, red, green, blue, alpha);
}

void CookTorranceSimpleMaterial::setEmission(unsigned char red, unsigned char green, unsigned char blue,
                                             float intensity)
{
    gx_common::GXVec3 color;
    gx_common::gxColorToVec3(color, red, green, blue);
    gx_common::gxMulVec3Scalar(_emission, color, intensity);
}

void CookTorranceSimpleMaterial::setRoughness(float roughness)
{
    _cookTorranceParams.x = roughness * roughness;
}

void CookTorranceSimpleMaterial::setIndexOfRefraction(float ior)
{
    _cookTorranceParams.y = ior;
}

void CookTorranceSimpleMaterial::setSpecularIntensity(float intensity)
{
    _cookTorranceParams.z = intensity;
}

void CookTorranceSimpleMaterial::setMetallic(bool isMetal)
{
    _cookTorranceParams.w = isMetal ? 1.0f : 0.0f;
}

} // namespace particles_game
