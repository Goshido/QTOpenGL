#include <GXEngine/mesh_geometry.h>
#include <GXCommon/gx_log.h>
#include <GXCommon/gx_obj_loader.h>
#include <GXCommon/gx_native_static_mesh.h>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#define CACHE_FOLDER_NAME "Cache"
#define CACHE_FILE_SUFFIX ".cache"

namespace gx_engine {

QHash<QString, QWeakPointer<MeshGeometry>> MeshGeometry::_storage;

MeshGeometry::MeshGeometry(const QString fileName)
    : _fileName(fileName)
    ,_totalElements(0)
{
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBindVertexArray(0);
}

MeshGeometry::~MeshGeometry()
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glDeleteBuffers(1, &_vbo);

    gx_common::gxLogA("MeshGeometry::~MeshGeometry::Info - Worked\n");
}

void MeshGeometry::render() const
{
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, _totalElements);
    glBindVertexArray(0);
}

void MeshGeometry::setBoundsLocal(const gx_common::GXAABB& boundsLocal)
{
    _boundsLocal = boundsLocal;
}

const gx_common::GXAABB& MeshGeometry::getBoundsLocal() const
{
    return _boundsLocal;
}

void MeshGeometry::updateBoundsWorld(const gx_common::GXMat4& transform)
{
    gx_common::GXVec3 verticesLocal[8];

    verticesLocal[0] = gx_common::gxCreateVec3(_boundsLocal.min.x, _boundsLocal.min.y, _boundsLocal.min.z);
    verticesLocal[1] = gx_common::gxCreateVec3(_boundsLocal.min.x, _boundsLocal.min.y, _boundsLocal.max.z);
    verticesLocal[2] = gx_common::gxCreateVec3(_boundsLocal.max.x, _boundsLocal.min.y, _boundsLocal.max.z);
    verticesLocal[3] = gx_common::gxCreateVec3(_boundsLocal.max.x, _boundsLocal.min.y, _boundsLocal.min.z);

    verticesLocal[4] = gx_common::gxCreateVec3(_boundsLocal.max.x, _boundsLocal.max.y, _boundsLocal.max.z);
    verticesLocal[5] = gx_common::gxCreateVec3(_boundsLocal.max.x, _boundsLocal.max.y, _boundsLocal.min.z);
    verticesLocal[6] = gx_common::gxCreateVec3(_boundsLocal.min.x, _boundsLocal.max.y, _boundsLocal.min.z);
    verticesLocal[7] = gx_common::gxCreateVec3(_boundsLocal.min.x, _boundsLocal.max.y, _boundsLocal.max.z);

    gx_common::gxSetAABBEmpty(_boundsWorld);

    gx_common::GXVec3 v;
    for (char i = 0; i < 8; i++) {
        gx_common::gxMulVec3Mat4AsPoint(v, verticesLocal[i], transform);
        gx_common::gxAddVertexToAABB(_boundsWorld, v);
    }
}

const gx_common::GXAABB& MeshGeometry::getBoundsWorld() const
{
    return _boundsWorld;
}

void MeshGeometry::setTotalElements(int elements)
{
    _totalElements = (GLsizei)elements;
}

void MeshGeometry::fillVertexBuffer(const void* data, size_t size, GLenum usage)
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, data, usage);
    glBindVertexArray(0);
}

void MeshGeometry::setBufferStream(GLuint streamIndex, GLint numElements,
                                   GLenum elementType, GLsizei stride, const GLvoid* offset)
{
    glBindVertexArray(_vao);
    glEnableVertexAttribArray(streamIndex);
    glVertexAttribPointer(streamIndex, numElements, elementType, GL_FALSE, stride, offset);
    glBindVertexArray(0);
}

QSharedPointer<MeshGeometry> MeshGeometry::loadFromObj(const QString& fileName)
{
    QFileInfo fileInfo(fileName);

    if (!fileInfo.exists()) {
        gx_common::gxLogA("MeshGeometry::loadFromObj::Error - File %s not found\n", fileName);
        return QSharedPointer<MeshGeometry>();
    }

    QDir dir;
    QString cacheFileDir = fileInfo.path() + "/" + CACHE_FOLDER_NAME;

    if (dir.mkdir(cacheFileDir)) {
        gx_common::gxLogA("MeshGeometry::loadFromObj::Info - Directory %s created\n", cacheFileDir.toLatin1().data());
    }

    QSharedPointer<MeshGeometry> meshGeometry;
    QHash<QString, QWeakPointer<MeshGeometry>>::iterator it = _storage.find(fileName);

    if (it == _storage.end()) {
        meshGeometry = QSharedPointer<MeshGeometry>(new MeshGeometry(fileName), &MeshGeometry::remove);
        _storage.insert(fileName, meshGeometry.toWeakRef());

        QString cacheFileName = cacheFileDir + "/" + fileInfo.completeBaseName() + CACHE_FILE_SUFFIX;
        QFile cacheFile(cacheFileName);
        if (cacheFile.exists()) {
            // Load from cache
            gx_common::GXNativeStaticMeshInfo info;
            gx_common::gxLoadNativeStaticMesh(cacheFileName, info);

            meshGeometry->setBoundsLocal(info.bounds);
            gx_common::GXMat4 transform;
            gx_common::gxSetMat4Identity(transform);
            meshGeometry->updateBoundsWorld(transform);

            meshGeometry->setTotalElements(info.numVertices);
            meshGeometry->fillVertexBuffer(info.vboData, info.getVBOSize(), GL_STATIC_DRAW);

            size_t offset = 0;
            meshGeometry->setBufferStream(VERTEX_STREAM, 3, GL_FLOAT, info.vboStride, (const void*)offset);
            offset += sizeof(gx_common::GXVec3);
            meshGeometry->setBufferStream(UV_COORD_STREAM, 2, GL_FLOAT, info.vboStride, (const void*)offset);
            offset += sizeof(gx_common::GXVec2);
            meshGeometry->setBufferStream(NORMAL_STREAM, 3, GL_FLOAT, info.vboStride, (const void*)offset);
            offset += sizeof(gx_common::GXVec3);
            meshGeometry->setBufferStream(TANGENT_STREAM, 3, GL_FLOAT, info.vboStride, (const void*)offset);
            offset += sizeof(gx_common::GXVec3);
            meshGeometry->setBufferStream(BITANGENT_STREAM, 3, GL_FLOAT, info.vboStride, (const void*)offset);

            info.cleanup ();
        } else {
            // Load from original file and save cache
            gx_common::GXOBJPoint* objPoints = nullptr;
            int numVertices = gx_common::gxLoadOBJ(fileName, &objPoints);

            gx_common::GXNativeStaticMeshDesc desc;

            desc.numVertices = numVertices;
            desc.numNormals = numVertices;
            desc.numTBPairs = numVertices;
            desc.numUVs = numVertices;
            desc.numElements = 0;

            size_t size = desc.numVertices * sizeof(gx_common::GXVec3);

            desc.vertices = (gx_common::GXVec3*)malloc(size);
            desc.uvs = (gx_common::GXVec2*)malloc(desc.numUVs * sizeof(gx_common::GXVec2));
            desc.normals = (gx_common::GXVec3*)malloc(size);
            desc.tangents = (gx_common::GXVec3*)malloc(size);
            desc.bitangents = (gx_common::GXVec3*)malloc(size);
            desc.elements = 0;

            size_t cachedDataSize = 4 * size + desc.numUVs * sizeof(gx_common::GXVec2);
            unsigned char* content = (unsigned char*)malloc(cachedDataSize);
            size_t offset = 0;

            gx_common::GXAABB localBounds;

            for (unsigned int i = 0; i < desc.numVertices; i++) {
                gx_common::gxAddVertexToAABB(localBounds, objPoints[i].vertex);

                desc.vertices[i] = objPoints[i].vertex;
                desc.uvs[i] = objPoints[i].uv;
                desc.normals[i] = objPoints[i].normal;

                const unsigned char* vertices = (const unsigned char*)(&objPoints[3 * (i / 3)].vertex);
                const unsigned char* uvs = (const unsigned char*)(&objPoints[3 * (i / 3)].uv);

                gx_common::gxGetTangentBitangent(desc.tangents[i], desc.bitangents[i], i % 3, vertices,
                                                 sizeof(gx_common::GXOBJPoint), uvs, sizeof(gx_common::GXOBJPoint));

                gx_common::GXVec3* v = (gx_common::GXVec3*)(content + offset);
                *v = desc.vertices[i];
                offset += sizeof(gx_common::GXVec3);

                gx_common::GXVec2* u = (gx_common::GXVec2*)(content + offset);
                *u = desc.uvs[i];
                offset += sizeof(gx_common::GXVec2);

                v = (gx_common::GXVec3*)(content + offset);
                *v = desc.normals[i];
                offset += sizeof(gx_common::GXVec3);

                v = (gx_common::GXVec3*)(content + offset);
                *v = desc.tangents[i];
                offset += sizeof(gx_common::GXVec3);

                v = (gx_common::GXVec3*)(content + offset);
                *v = desc.bitangents[i];
                offset += sizeof(gx_common::GXVec3);
            }

            meshGeometry->setBoundsLocal(localBounds);
            gx_common::GXMat4 transform;
            gx_common::gxSetMat4Identity(transform);
            meshGeometry->updateBoundsWorld(transform);

            meshGeometry->setTotalElements(desc.numVertices);
            meshGeometry->fillVertexBuffer(content, cachedDataSize, GL_STATIC_DRAW);

            static const size_t vboStride = sizeof(gx_common::GXVec3) + sizeof(gx_common::GXVec2)
                    + sizeof(gx_common::GXVec3) + 2 * sizeof(gx_common::GXVec3);

            offset = 0;
            meshGeometry->setBufferStream(VERTEX_STREAM, 3, GL_FLOAT, vboStride, (const void*)offset);
            offset += sizeof(gx_common::GXVec3);
            meshGeometry->setBufferStream(UV_COORD_STREAM, 2, GL_FLOAT, vboStride, (const void*)offset);
            offset += sizeof(gx_common::GXVec2);
            meshGeometry->setBufferStream(NORMAL_STREAM, 3, GL_FLOAT, vboStride, (const void*)offset);
            offset += sizeof(gx_common::GXVec3);
            meshGeometry->setBufferStream(TANGENT_STREAM, 3, GL_FLOAT, vboStride, (const void*)offset);
            offset += sizeof(gx_common::GXVec3);
            meshGeometry->setBufferStream(BITANGENT_STREAM, 3, GL_FLOAT, vboStride, (const void*)offset);

            free(content);

            gx_common::gxExportNativeStaticMesh(cacheFileName, desc);

            free(desc.vertices);
            free(desc.uvs);
            free(desc.normals);
            free(desc.tangents);
            free(desc.bitangents);
        }
    } else {
        meshGeometry = it.value().lock();
    }

    return meshGeometry;
}

void MeshGeometry::remove(MeshGeometry* meshGeometry)
{
    _storage.remove(meshGeometry->_fileName);
    delete meshGeometry;
}

} // namespace gx_engine
