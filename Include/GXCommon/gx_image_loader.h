#ifndef GX_IMAGE_LOADER_H
#define GX_IMAGE_LOADER_H


#include <QString>

namespace gx_common {

// numChannels      components
// 1                alpha
// 2                luminance, alpha
// 3                red, green, blue
// 4                red, green, blue, alpha


bool __stdcall gxLoadImage(const QString& fileName, unsigned short& width, unsigned short& height,
                           unsigned char& numChannels, void** data);

} // namespace gx_common

#endif GX_IMAGE_LOADER_H
