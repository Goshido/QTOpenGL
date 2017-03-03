//version 1.5

#ifndef GX_OBJLOADER_H
#define GX_OBJLOADER_H

#include "gx_math.h"
#include <QString>

namespace gx_common {

#pragma pack(push, 1)

struct GXOBJPoint {
    GXVec3 vertex;
    GXVec3 normal;
    GXVec2 uv;
};

#pragma pack(pop)

int __stdcall gxLoadOBJ(const QString fileName, GXOBJPoint** points); //retruns total vertices

} // namespace gx_common

#endif //GX_OBJLOADER
