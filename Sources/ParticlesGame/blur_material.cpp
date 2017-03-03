#include <ParticlesGame/blur_material.h>
#include <GXEngine/mesh_geometry.h>
#include <GXEngine/camera.h>

#define VERTEX_SHADER "Shaders/ParticlesGame/SimpleMesh_vs.txt"
#define FRAGMENT_SHADER "Shaders/ParticlesGame/Blur_fs.txt"

#define IMAGE_SLOT 0
#define VELOCITY_SLOT 1
#define DEPTH_SLOT 2

#define DEFAULT_BLUR_STRENGTH 1.0
#define DEFAULT_BLUR_VELOCITY_MINIMUM 1.0
#define DEFAULT_SCREEN_WIDTH 1920.0f
#define DEFAULT_SCREEN_HEIGHT 1080.0f

namespace particles_game {

BlurMaterial::BlurMaterial()
    : _image(nullptr)
    , _velocity(nullptr)
    , _depth(nullptr)
    , _invViewerProjectionMatrix(nullptr)
{
    static const GLchar* attributeNames[2] = {"vertex", "uv"};
    static const GLuint attributeLocations[2] = {VERTEX_STREAM, UV_COORD_STREAM};
    static const GLchar* samplerNames[3] = {"imageSampler", "velocitySampler", "depthSampler"};
    static const GLuint samplerLocations[3] = {IMAGE_SLOT, VELOCITY_SLOT, DEPTH_SLOT};
    static const GLchar* fragmentNames[1] = {"fragmentColor"};
    static const GLuint fragmentLocations[1] = {0};

    gx_engine::ShaderProgramInfo info;
    info.vs = VERTEX_SHADER;
    info.fs = FRAGMENT_SHADER;
    info.numAttributes = 2;
    info.attributeNames = attributeNames;
    info.attributeLocations = attributeLocations;
    info.numSamplers = 3;
    info.samplerNames = samplerNames;
    info.samplerLocations = samplerLocations;
    info.numFragments = 1;
    info.fragmentNames = fragmentNames;
    info.fragmentLocations = fragmentLocations;

    _program = gx_engine::ShaderProgram::getShaderProgram(info);

    _modelViewProjectionMatrixLocation = _program->getUniform("mod_view_proj_mat");
    _paramsLocation = _program->getUniform("params");
    _invViewerProjectionMatrixLocation = _program->getUniform("inv_proj_mat");

    setBlurStrength(DEFAULT_BLUR_STRENGTH);
    setBlurVelocityMinimum(DEFAULT_BLUR_VELOCITY_MINIMUM);
    _params.x = 1.0f / DEFAULT_SCREEN_WIDTH;
    _params.y = 1.0f / DEFAULT_SCREEN_HEIGHT;
}

BlurMaterial::~BlurMaterial()
{
    // NOTHING
}

void BlurMaterial::bind(const gx_engine::Renderable& activeRenderable)
{
    if (!_image || !_velocity || !_depth || !_invViewerProjectionMatrix) return;

    glUseProgram(_program->getProgram());

    gx_common::GXMat4 modelViewProjectionMatrix;
    gx_common::gxMulMat4Mat4(modelViewProjectionMatrix, activeRenderable.getCurrentModelMatrix(),
                             gx_engine::Camera::getActiveCamera()->getViewProjectionMatrix());

    glUniformMatrix4fv(_modelViewProjectionMatrixLocation, 1, GL_FALSE, modelViewProjectionMatrix.arr);
    glUniformMatrix4fv(_invViewerProjectionMatrixLocation, 1, GL_FALSE, _invViewerProjectionMatrix->arr);
    glUniform4fv(_paramsLocation, 1, _params.arr);

    _image->bind(IMAGE_SLOT);
    _velocity->bind(VELOCITY_SLOT);
    _depth->bind(DEPTH_SLOT);
}

void BlurMaterial::unbind()
{
    if (!_image || !_velocity || !_depth || !_invViewerProjectionMatrix) return;

    _image->unbind();
    _velocity->unbind();
    _depth->unbind();

    glUseProgram(0);
}

void BlurMaterial::setBlurStrength(float strength)
{
    _params.z = strength;
}

void BlurMaterial::setBlurVelocityMinimum(float velocity)
{
    _params.w = velocity;
}

void BlurMaterial::setInverseViewerProjectionMatrix(const gx_common::GXMat4& matrix)
{
    _invViewerProjectionMatrix = &matrix;
}

void BlurMaterial::setControlTextures(gx_engine::Texture& image, gx_engine::Texture& velocity,
                                      gx_engine::Texture& depth)
{
    _image = &image;
    _velocity = &velocity;
    _depth = &depth;

    _params.x = 1.0f / (float)_image->getWidth();
    _params.y = 1.0f / (float)_image->getHeight();
}

} // namespace particles_game
