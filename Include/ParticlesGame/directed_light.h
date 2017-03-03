#ifndef DIRECTED_LIGHT_H
#define DIRECTED_LIGHT_H

#include "light.h"

namespace particles_game {

class DirectedLight : public Light {
public:
    explicit DirectedLight();
    ~DirectedLight() override;

    void setRotation(float pitch_rad, float yaw_rad, float roll_rad);
    const gx_common::GXMat4& getRotation() const;

private:
    gx_common::GXMat4 _rotation;
};

} // namespace particles_game

#endif DIRECTED_LIGHT_H
