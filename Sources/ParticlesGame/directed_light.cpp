#include <ParticlesGame/directed_light.h>

namespace particles_game {

DirectedLight::DirectedLight()
    : Light(LightType::Directed)
{
    gx_common::gxSetMat4RotationX(_rotation, GX_MATH_HALFPI);
}

DirectedLight::~DirectedLight()
{
    // NOTHING
}

void DirectedLight::setRotation(float pitch_rad, float yaw_rad, float roll_rad)
{
    gx_common::gxSetMat4RotationXYZ(_rotation, pitch_rad, yaw_rad, roll_rad);
}

const gx_common::GXMat4& DirectedLight::getRotation() const
{
    return _rotation;
}

} // namespace particles_game
