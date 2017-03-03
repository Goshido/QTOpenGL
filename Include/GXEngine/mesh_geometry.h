#ifndef MESH_GEOMETRY_H
#define MESH_GEOMETRY_H

#include "opengl.h"
#include <GXCommon/gx_math.h>
#include <QString>
#include <QSharedPointer>
#include <QHash>

#define VERTEX_STREAM 0
#define UV_COORD_STREAM 1
#define NORMAL_STREAM 2
#define TANGENT_STREAM 3
#define BITANGENT_STREAM 4

namespace gx_engine {

class MeshGeometry {
public:
    explicit MeshGeometry(const QString fileName);
    virtual ~MeshGeometry();

    void render() const;

    void setBoundsLocal(const gx_common::GXAABB& boundsLocal);
    const gx_common::GXAABB& getBoundsLocal() const;

    void updateBoundsWorld(const gx_common::GXMat4& transform);
    const gx_common::GXAABB& getBoundsWorld() const;

    void setTotalElements(int elements);
    void fillVertexBuffer(const void* data, size_t size, GLenum usage);
    void setBufferStream(GLuint streamIndex, GLint numElements, GLenum elementType,
                         GLsizei stride, const GLvoid* offset);

    static QSharedPointer<MeshGeometry> loadFromObj(const QString& fileName);

private:
    static void remove(MeshGeometry* meshGeometry);

    QString _fileName;
    GLsizei _totalElements;
    GLuint _vao;
    GLuint _vbo;
    gx_common::GXAABB _boundsLocal;
    gx_common::GXAABB _boundsWorld;

    static QHash<QString, QWeakPointer<MeshGeometry>> _storage;
};

} // gx_engine

#endif MESH_GEOMETRY_H
