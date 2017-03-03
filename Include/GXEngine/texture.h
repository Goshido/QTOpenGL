#ifndef TEXTURE_H
#define TEXTURE_H

#include "opengl.h"
#include <QSharedPointer>
#include <QHash>

namespace gx_engine {

class Texture {
public:
    explicit Texture(unsigned short width, unsigned short height, GLint internalFormat);
    ~Texture();

    unsigned short getWidth() const;
    unsigned short getHeight() const;

    static QSharedPointer<Texture> loadTexture(const QString& fileName);
    void fillPixelData(const void* data);

    void enableResampling();
    void disableResampling();

    void bind(unsigned char textureUnit);
    void unbind();

    GLuint getTextureObject() const;

private:
    static void remove(Texture* texture);

    unsigned short _width;
    unsigned short _height;
    GLint _internalFormat;
    GLenum _format;
    GLenum _type;
    bool _isResamplingEnabled;
    unsigned char _textureUnit;
    GLuint _textureObject;
    QString _name;

    static QHash<QString, QWeakPointer<Texture>> _storage;
};

} // namespace gx_engine

#endif TEXTURE_H
