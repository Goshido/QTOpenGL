#include <ParticlesGame/light.h>

#define DEFAULT_COLOR_R 255
#define DEFAULT_COLOR_G 255
#define DEFAULT_COLOR_B 255
#define DEFAULT_INTENSITY 1.0f

namespace particles_game {

Light::Light(LightType type)
    : _type(type)
{
    _intensity = DEFAULT_INTENSITY;
    setColor(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B);
    disableShadowCasting();
}

Light::~Light()
{
    // NOTHING
}

void Light::setColor(unsigned char red, unsigned char green, unsigned char blue)
{
    _colorR = red;
    _colorG = green;
    _colorB = blue;

    gx_common::gxColorToVec3(_hdrColor, _colorR, _colorG, _colorB);
    gx_common::gxMulVec3Scalar(_hdrColor, _hdrColor, _intensity);
}

void Light::setIntensity(float intensity)
{
    _intensity = intensity;

    gx_common::gxColorToVec3(_hdrColor, _colorR, _colorG, _colorB);
    gx_common::gxMulVec3Scalar(_hdrColor, _hdrColor, _intensity);
}

void Light::getColor(unsigned char& red, unsigned char& green, unsigned char& blue) const
{
    red = _colorR;
    green = _colorG;
    blue = _colorB;
}

float Light::getIntensity() const
{
    return _intensity;
}

const gx_common::GXVec3& Light::getHDRColor() const
{
    return _hdrColor;
}

LightType Light::getType() const
{
    return _type;
}

bool Light::isCastShadow() const
{
    return _isCastShadow;
}

void Light::enableShadowCasting()
{
    _isCastShadow = true;
}

void Light::disableShadowCasting()
{
    _isCastShadow = false;
}

} // namespace particles_game
