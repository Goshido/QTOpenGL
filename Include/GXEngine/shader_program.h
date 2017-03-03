#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <QString>
#include <QSharedPointer>
#include <QHash>
#include "opengl.h"
#include <GXCommon/gx_math.h>

namespace gx_engine {

struct ShaderProgramInfo {
    QString vs;
    QString gs;
    QString fs;

    const GLchar** attributeNames;
    const GLuint* attributeLocations;
    int numAttributes;

    const GLchar** samplerNames;
    const GLuint* samplerLocations;
    int numSamplers;

    const GLchar** fragmentNames;
    const GLuint* fragmentLocations;
    int numFragments;

    ShaderProgramInfo();
};

class ShaderProgram {
public:
    GLuint getProgram() const;
    void setAttributes(const QString* names, const GLuint* locations, int numAttributes);
    GLint getUniform(const QString& name);

    static QSharedPointer<ShaderProgram> getShaderProgram(const ShaderProgramInfo& info);

private:
    explicit ShaderProgram(const ShaderProgramInfo& info);
    ~ShaderProgram();

    GLuint getShader(GLenum type, const QString& path);
    static void removeShaderProgram(ShaderProgram* program);

    GLuint _program;
    QString _hash;

    static QHash<QString, QWeakPointer<ShaderProgram>> _storage;
};

} // namespace gx_engine

#endif SHADER_PROGRAM_H
