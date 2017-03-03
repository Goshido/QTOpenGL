#include <ParticlesGame/unlit_material.h>
#include <GXEngine/mesh_geometry.h>
#include <GXEngine/camera.h>

#define DEFAULT_SCALE_U 1.0f
#define DEFAULT_SCALE_V 1.0f
#define DEFAULT_OFFSET_U 0.0f
#define DEFAULT_OFFSET_V 0.0f

#define VERTEX_SHADER "Shaders/ParticlesGame/SimpleMesh_vs.txt"
#define FRAGMENT_SHADER "Shaders/ParticlesGame/Unlit_fs.txt"

namespace particles_game {

UnlitMaterial::UnlitMaterial()
    : _diffuseTexture(nullptr)
{
    gx_engine::ShaderProgramInfo info;

    const GLchar* attributeNames[2] = {"vertex", "uv"};
    const GLuint attributeLocations[2] = {VERTEX_STREAM, UV_COORD_STREAM};

    const GLchar* samplerNames[1] = {"colorSampler"};
    const GLuint samplerLocations[1] = {0};

    const GLchar* fragmentNames[1] = {"fragmentColor"};
    const GLuint fragmentLocations[1] = {0};

    info.vs = VERTEX_SHADER;
    info.fs = FRAGMENT_SHADER;
    info.numAttributes = 2;
    info.attributeNames = attributeNames;
    info.attributeLocations = attributeLocations;
    info.numSamplers = 1;
    info.samplerNames = samplerNames;
    info.samplerLocations = samplerLocations;
    info.numFragments = 1;
    info.fragmentNames = fragmentNames;
    info.fragmentLocations = fragmentLocations;

    _program = gx_engine::ShaderProgram::getShaderProgram(info);

    _mappingParams = gx_common::gxCreateVec4(DEFAULT_SCALE_U, DEFAULT_SCALE_V, DEFAULT_OFFSET_U, DEFAULT_OFFSET_V);

    _modelViewProjectionMatrixLocation = _program->getUniform("mod_view_proj_mat");
    _mappingParamsLocation = _program->getUniform("mappingParams");
}

UnlitMaterial::~UnlitMaterial()
{
    // NOTHING
}

void UnlitMaterial::bind(const gx_engine::Renderable& activeRenderable)
{
    glUseProgram(_program->getProgram());
    const gx_common::GXMat4& viewProjectionMatrix = gx_engine::Camera::getActiveCamera()->getViewProjectionMatrix();
    const gx_common::GXMat4& modelMatrix = activeRenderable.getCurrentModelMatrix();
    gx_common::GXMat4 modelViewProjectionMatrix;
    gx_common::gxMulMat4Mat4(modelViewProjectionMatrix, modelMatrix, viewProjectionMatrix);
    glUniformMatrix4fv(_modelViewProjectionMatrixLocation, 1, GL_FALSE, modelViewProjectionMatrix.arr);

    if (_diffuseTexture) {
        _diffuseTexture->bind(0);
        glUniform4fv(_mappingParamsLocation, 1, _mappingParams.arr);
    }
}

void UnlitMaterial::unbind()
{
    if (_diffuseTexture) {
        _diffuseTexture->unbind();
    }

    glUseProgram(0);
}

void UnlitMaterial::setTexture(gx_engine::Texture& diffuseTexture)
{
    _diffuseTexture = &diffuseTexture;
}

void UnlitMaterial::setMappingParams(float scaleU, float scaleV, float offsetU, float offsetV)
{
    _mappingParams.x = scaleU;
    _mappingParams.y = scaleV;
    _mappingParams.z = offsetU;
    _mappingParams.w = offsetV;
}

} // namespace particles_game
