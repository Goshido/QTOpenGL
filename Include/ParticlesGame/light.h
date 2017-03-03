#ifndef LIGHT_H
#define LIGHT_H

#include <GXCommon/gx_math.h>

namespace particles_game {

enum class LightType {
    Directed,
    Bulp,
    Spot
};

class Light {
public:
    explicit Light(LightType type);
    virtual ~Light();

    void setColor(unsigned char red, unsigned char green, unsigned char blue);
    void setIntensity(float intensity);

    void getColor(unsigned char& red, unsigned char& green, unsigned char& blue) const;
    float getIntensity() const;
    const gx_common::GXVec3& getHDRColor() const;

    LightType getType() const;
    bool isCastShadow() const;

    void enableShadowCasting();
    void disableShadowCasting();

protected:
    float _intensity;

private:
    unsigned char _colorR;
    unsigned char _colorG;
    unsigned char _colorB;
    gx_common::GXVec3 _hdrColor;
    LightType _type;
    bool _isCastShadow;
};

} // namespace particles_game

#endif LIGHT_H
