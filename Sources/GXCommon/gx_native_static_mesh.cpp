#include <GXCommon/gx_native_static_mesh.h>
#include <QFile>

namespace gx_common {

struct GXNativeStaticMeshHeader
{
    GXAABB bounds;

    unsigned int numVertices;
    unsigned int numUVs;
    unsigned int numNormals;
    unsigned int numTBPairs; //Tangent and Bitangent pairs
    unsigned int numElements;

    unsigned int elementOffset;
};

//----------------------------------------------------------------------------------------------

const size_t GXNativeStaticMeshInfo::vboStride = sizeof(GXVec3) + sizeof(GXVec2) + sizeof(GXVec3) + 2 * sizeof(GXVec3);

GXNativeStaticMeshInfo::GXNativeStaticMeshInfo()
{
    vboData = nullptr;
    eboData = nullptr;
}

size_t GXNativeStaticMeshInfo::getVBOSize()
{
    return numVertices * sizeof(GXVec3) + numUVs * sizeof(GXVec2)
            + numNormals * sizeof(GXVec3) + 2 * numTBPairs * sizeof(GXVec3);
}

size_t GXNativeStaticMeshInfo::getEBOSize()
{
    return numElements * sizeof(unsigned int);
}

void GXNativeStaticMeshInfo::cleanup()
{
    if (!vboData) {
        free(vboData);
        vboData = nullptr;
    }

    if (!eboData) {
        free(eboData);
        eboData = nullptr;
    }
}

//----------------------------------------------------------------------------------------------

void __stdcall gxLoadNativeStaticMesh(const QString fileName, GXNativeStaticMeshInfo& info)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QByteArray byteArray = file.readAll();
    const unsigned char* data = (const unsigned char*)byteArray.data();
    file.close();

    gxLoadNativeStaticMesh(data, info);
}

void __stdcall gxLoadNativeStaticMesh(const unsigned char* mappedFile, GXNativeStaticMeshInfo& info)
{
    GXNativeStaticMeshHeader* h = (GXNativeStaticMeshHeader*)mappedFile;

    info.bounds = h->bounds;

    info.numVertices = h->numVertices;
    info.numUVs = h->numUVs;
    info.numNormals = h->numNormals;
    info.numTBPairs = h->numTBPairs;

    unsigned int size = info.numVertices * sizeof(GXVec3) + info.numUVs * sizeof(GXVec2) + info.numNormals
            * sizeof(GXVec3) + info.numTBPairs * 2 * sizeof(GXVec3);

    info.vboData = (unsigned char*)malloc(size);
    memcpy(info.vboData, mappedFile + sizeof(GXNativeStaticMeshHeader), size);

    info.numElements = h->numElements;
    if(info.numElements > 0) {
        size = info.numElements * sizeof(unsigned int);
        info.eboData = (unsigned char*)malloc(size);
        memcpy(info.eboData, mappedFile + h->elementOffset, size);
    }
}

void __stdcall gxCompileNativeStaticMesh(unsigned char** data, unsigned int& size,
                                         const GXNativeStaticMeshDesc& descriptor)
{
    size = sizeof(GXNativeStaticMeshHeader);
    size += descriptor.numVertices * sizeof(GXVec3);
    size += descriptor.numUVs * sizeof(GXVec2);
    size += descriptor.numNormals * sizeof(GXVec3);
    size += 2 * descriptor.numTBPairs * sizeof(GXVec3); //Tangents and Bitangents
    size += descriptor.numElements * sizeof(unsigned int);

    *data = (unsigned char*)malloc(size);

    GXNativeStaticMeshHeader* h = (GXNativeStaticMeshHeader*)(*data);
    h->numVertices = descriptor.numVertices;
    h->numNormals = descriptor.numNormals;
    h->numTBPairs = descriptor.numTBPairs;
    h->numUVs = descriptor.numUVs;
    h->numElements = descriptor.numElements;

    gxSetAABBEmpty(h->bounds);

    unsigned int offset = sizeof(GXNativeStaticMeshHeader);
    for (unsigned int i = 0; i < descriptor.numVertices; i++) {
        gxAddVertexToAABB(h->bounds, descriptor.vertices[i]);

        memcpy(*data + offset, descriptor.vertices + i, sizeof(GXVec3));
        offset += sizeof(GXVec3);

        if (descriptor.numUVs > 0) {
            memcpy(*data + offset, descriptor.uvs + i, sizeof(GXVec2));
            offset += sizeof(GXVec2);
        }

        if (descriptor.numNormals > 0) {
            memcpy(*data + offset, descriptor.normals + i, sizeof(GXVec3));
            offset += sizeof ( GXVec3 );
        }

        if (descriptor.numTBPairs > 0) {
            memcpy(*data + offset, descriptor.tangents + i, sizeof(GXVec3));
            offset += sizeof(GXVec3);
            memcpy(*data + offset, descriptor.bitangents + i, sizeof(GXVec3));
            offset += sizeof(GXVec3);
        }
    }

    h->elementOffset = offset;
    memcpy(*data + offset, descriptor.elements, descriptor.numElements * sizeof(unsigned int));
}

void __stdcall gxExportNativeStaticMesh(const QString fileName, const GXNativeStaticMeshDesc& descriptor)
{
    unsigned char* data = nullptr;
    unsigned int size = 0;
    gxCompileNativeStaticMesh(&data, size, descriptor);

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write((const char*)data, (qint64)size);
    file.flush();
    file.close();

    free(data);
}

} // namespace gx_common
