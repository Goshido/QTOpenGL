#include <GXEngine/texture.h>
#include <GXCommon/gx_image_loader.h>
#include <GXCommon/gx_log.h>

namespace gx_engine {

QHash<QString, QWeakPointer<Texture>> Texture::_storage;

Texture::Texture(unsigned short width, unsigned short height, GLint internalFormat)
    : _width(width)
    , _height(height)
    , _internalFormat(internalFormat)
    , _isResamplingEnabled(true)
    , _textureUnit(0)
{
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &_textureObject);
    glBindTexture(GL_TEXTURE_2D, _textureObject);

    switch (_internalFormat) {
        case GL_R32I:
        case GL_R32UI: {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            _format = GL_RED;
            _type = GL_UNSIGNED_INT;
            break;
        }
        case GL_R8:
        case GL_R8I:
        case GL_R8UI: {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            _format = GL_RED;
            _type = GL_UNSIGNED_BYTE;
            break;
        }
        case GL_RG8:
        case GL_RG8I:
        case GL_RG8UI: {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
            _format = GL_RG;
            _type = GL_UNSIGNED_BYTE;
            break;
        }
        case GL_RGB8:
        case GL_RGB8I:
        case GL_RGB8UI: {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            _format = GL_RGB;
            _type = GL_UNSIGNED_BYTE;
            break;
        }
        case GL_RGB16: {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            _format = GL_RGB;
            _type = GL_UNSIGNED_SHORT;
            break;
        }
        case GL_RGBA8:
        case GL_RGBA8I:
        case GL_RGBA8UI: {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            _format = GL_RGBA;
            _type = GL_UNSIGNED_BYTE;
            break;
        }
        case GL_RGBA16F: {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            _format = GL_RGBA;
            _type = GL_HALF_FLOAT;
            break;
        }
        case GL_DEPTH24_STENCIL8: {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            _format = GL_DEPTH_STENCIL;
            _type = GL_UNSIGNED_INT_24_8;
            break;
        }
        default:
            // NOTHING
            break;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    fillPixelData(nullptr);
}

Texture::~Texture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &_textureObject);

    gx_common::gxLogA("Texture::~Texture::Info - Worked\n");
}

unsigned short Texture::getWidth() const
{
    return _width;
}

unsigned short Texture::getHeight() const
{
    return _height;
}

QSharedPointer<Texture> Texture::loadTexture(const QString& fileName)
{
    QSharedPointer<Texture> texture;
    QHash<QString, QWeakPointer<Texture>>::iterator it = _storage.find(fileName);

    if (it == _storage.end()) {
        unsigned short width = 0;
        unsigned short height = 0;
        unsigned char numChannels = 0;
        void* pixelData = 0;

        if (!gx_common::gxLoadImage(fileName, width, height, numChannels, &pixelData)) {
            gx_common::gxLogA("Texture::loadTexture::Error - Can't load texture %s\n", fileName.toLatin1().data());
            return texture;
        }

        GLint internalFormat;
        switch (numChannels) {
            case 1: {
                internalFormat = GL_R8;
                break;
            }
            case 2: {
                internalFormat = GL_RG8;
                break;
            }
            case 3: {
                internalFormat = GL_RGB8;
                break;
            }
            case 4: {
                internalFormat = GL_RGBA8;
                break;
            }
            default:
                // NOTHING
                break;
        }

        texture = QSharedPointer<Texture>(new Texture(width, height, internalFormat),
                                                                  &Texture::remove);

        texture->fillPixelData(pixelData);
        free(pixelData);

        texture->_name = fileName;

        _storage.insert(fileName, texture.toWeakRef());
    } else {
        texture = it.value().lock();
    }

    return texture;
}

void Texture::fillPixelData(const void* data)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureObject);

    glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, _width, _height, 0, _format, _type, data);
    glGetError();

    if (_isResamplingEnabled) {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        GLfloat maxAnisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::enableResampling()
{
    if (_isResamplingEnabled) return;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureObject);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    GLfloat maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

    glBindTexture(GL_TEXTURE_2D, 0);

    _isResamplingEnabled = true;
}

void Texture::disableResampling()
{
    if (!_isResamplingEnabled) return;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureObject);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);

    glBindTexture(GL_TEXTURE_2D, 0);

    _isResamplingEnabled = false;
}

void Texture::bind(unsigned char textureUnit)
{
    _textureUnit = textureUnit;
    glActiveTexture(GL_TEXTURE0 + _textureUnit);
    glBindTexture(GL_TEXTURE_2D, _textureObject);
}

void Texture::unbind()
{
    glActiveTexture(GL_TEXTURE0 + _textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getTextureObject() const
{
    return _textureObject;
}

void Texture::remove(Texture* texture)
{
    _storage.remove(texture->_name);
    delete texture;
}

} // namespace gx_engine
