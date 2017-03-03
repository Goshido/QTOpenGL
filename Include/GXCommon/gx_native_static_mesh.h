#ifndef GX_NATIVE_STATIC_MESH_H
#define GX_NATIVE_STATIC_MESH_H

#include "gx_math.h"
#include <QString>

namespace gx_common {

struct GXNativeStaticMeshInfo
{
    GXNativeStaticMeshInfo();
    size_t getVBOSize();
    size_t getEBOSize();
    void cleanup();

    GXAABB bounds;

    unsigned int numVertices;
    unsigned int numUVs;
    unsigned int numNormals;
    unsigned int numTBPairs;
    unsigned int numElements;

    unsigned char* vboData;
    unsigned char* eboData;

    static const size_t vboStride;
};

struct GXNativeStaticMeshDesc
{
    unsigned int numVertices;
    unsigned int numUVs;
    unsigned int numNormals;
    unsigned int numTBPairs; //Tangent and Bitangent pairs
    unsigned int numElements;

    GXVec3* vertices;
    GXVec2* uvs;
    GXVec3* normals;
    GXVec3* tangents;
    GXVec3* bitangents;

    unsigned int* elements;
};

void __stdcall gxLoadNativeStaticMesh(const QString fileName, GXNativeStaticMeshInfo& info);
void __stdcall gxLoadNativeStaticMesh(const unsigned char* mappedFile, GXNativeStaticMeshInfo& info);
void __stdcall gxExportNativeStaticMesh(const QString fileName, const GXNativeStaticMeshDesc& descriptor);

} // namespace gx_common

#endif GX_NATIVE_STATIC_MESH_H
