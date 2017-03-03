#include <GXEngine/shader_program.h>
#include <GXEngine/opengl.h>
#include <GXCommon/gx_log.h>
#include <QFile>

#define SHADER_LOG_BUFFER_SIZE 4096

namespace gx_engine {

ShaderProgramInfo::ShaderProgramInfo()
{
    vs = gs = fs = "";

    attributeNames = nullptr;
    attributeLocations = nullptr;
    numAttributes = 0;

    samplerNames = nullptr;
    samplerLocations = nullptr;
    numSamplers = 0;

    fragmentNames = nullptr;
    fragmentLocations = nullptr;
    numFragments = 0;
}

//------------------------------------------------------------------

QHash<QString, QWeakPointer<ShaderProgram>> ShaderProgram::_storage;

GLuint ShaderProgram::getProgram() const
{
    return _program;
}

void ShaderProgram::setAttributes(const QString* names, const GLuint* locations, int numAttributes)
{
    for (int i = 0; i < numAttributes; i++) {
        glBindAttribLocation(_program, locations[i], names[i].toLatin1().data());
    }
}

QSharedPointer<ShaderProgram> ShaderProgram::getShaderProgram(const ShaderProgramInfo& info)
{
    QString hash = info.vs + info.gs + info.fs;

    QSharedPointer<ShaderProgram> program;
    QHash<QString, QWeakPointer<ShaderProgram>>::iterator it = _storage.find(hash);

    if (it == _storage.end()) {
        program = QSharedPointer<ShaderProgram>(new ShaderProgram(info), &ShaderProgram::removeShaderProgram);
        _storage.insert(hash, program.toWeakRef());
    } else {
        program = it.value().lock();
    }

    return program;
}

GLint ShaderProgram::getUniform(const QString& name)
{
    GLint location = glGetUniformLocation(_program, name.toLatin1().data());
    if (location == -1) {
        gx_common::gxLogA("ShaderProgram::getUniform::Error - Uniform %s is not found in shader program %s.\n",
                          name.toLatin1().data(), _hash.toLatin1().data());
    }

    return location;
}

ShaderProgram::ShaderProgram(const ShaderProgramInfo& info)
{
    _hash = info.vs + info.gs + info.fs;

    static GLchar log[SHADER_LOG_BUFFER_SIZE] = {0};

    _program = glCreateProgram();

    GLuint shader = getShader(GL_VERTEX_SHADER, info.vs);
    glAttachShader(_program, shader);
    glDeleteShader(shader);

    if (!info.gs.isEmpty()) {
        shader = getShader(GL_GEOMETRY_SHADER, info.gs);
        glAttachShader(_program, shader);
        glDeleteShader(shader);
    }

    shader = getShader(GL_FRAGMENT_SHADER, info.fs);
    glAttachShader(_program, shader);
    glDeleteShader(shader);

    for (int i = 0; i < info.numAttributes; i++) {
        glBindAttribLocation(_program, info.attributeLocations[i], info.attributeNames[i]);
    }

    for (int i = 0; i < info.numFragments; i++) {
        glBindFragDataLocation(_program, info.fragmentLocations[i], info.fragmentNames[i]);
    }

    glLinkProgram(_program);

    GLint status = 0;
    glGetProgramiv ( _program, GL_LINK_STATUS, &status );

    if (status == GL_FALSE) {
        GLint length = 0;
        glGetProgramInfoLog(_program, SHADER_LOG_BUFFER_SIZE, &length, log);

        gx_common::gxLogA("Can't link shader program %s + %s + %s\n",
                          info.vs.toLatin1().data(), info.gs.toLatin1().data(), info.fs.toLatin1().data());

        if (!length) {
            gx_common::gxLogA("%s\n", log);
        }

        glDeleteProgram(_program);
        _program = 0;
    } else {
        if (info.numSamplers) {
            glUseProgram(_program);

            for (int i = 0; i < info.numSamplers; i++) {
                glUniform1i(getUniform(info.samplerNames[i]), info.samplerLocations[i]);
            }

            glUseProgram(0);
        }
    }
}

ShaderProgram::~ShaderProgram()
{
    if (_program) {
        glDeleteProgram(_program);
    }

    gx_common::gxLogA("ShaderProgram::~ShaderProgram::Info - Worked\n");
}

GLuint ShaderProgram::getShader(GLenum type, const QString& path)
{
    static GLchar log[SHADER_LOG_BUFFER_SIZE] = {0};

    GLuint shader = glCreateShader(type);

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    GLint size = data.size();
    const GLchar* sourceCode = (const GLchar*)data.data();

    glShaderSource(shader, 1, (const GLchar**)&sourceCode, &size);
    file.close();

    glCompileShader(shader);

    GLint status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLsizei length = 0;

        glGetShaderInfoLog(shader, SHADER_LOG_BUFFER_SIZE, &length, log);

        if (length == 0) return 0;

        gx_common::gxLogA("%s compile error:\n%s\n", path.toLatin1().data(), log);
        return 0;
    }

    return shader;
}

void ShaderProgram::removeShaderProgram(ShaderProgram* program)
{
    _storage.remove(program->_hash);
    delete program;
}

} // namespace gx_engine
