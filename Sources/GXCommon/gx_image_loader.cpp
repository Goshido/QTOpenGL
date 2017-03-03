#include <GXCommon/gx_image_loader.h>
#include <GXCommon/gx_log.h>
#include <QFile>

#define STB_IMAGE_IMPLEMENTATION

#define STBI_NO_SIMD // TODO: check if SIMD works on all ARM processors.
#define STBI_NO_STDIO
#define STBI_NO_FAILURE_STRINGS

#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_BMP
#define STBI_ONLY_TGA

#include <STB/stb_image.h>

namespace gx_common {

bool __stdcall gxLoadImage(const QString& fileName, unsigned short& width, unsigned short& height,
                           unsigned char& numChannels, void** data)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QByteArray byteArray = file.readAll();
    const stbi_uc* mappedFile = (const stbi_uc*)byteArray.data();
    int size = (int)file.size();
    file.close();

    int channels = 0;
    int w = 0;
    int h = 0;

    unsigned char* rawData = (unsigned char*)stbi_load_from_memory(mappedFile, size, &w, &h, &channels, 0);

    if (!rawData) return false;

    int lineSize = w * channels;
    unsigned char* lineBuffer = (unsigned char*)malloc(lineSize);

    unsigned char* top = rawData + (w * h * channels - lineSize);
    unsigned char* bottom = rawData;

    while (top > bottom) {
        memcpy(lineBuffer, top, lineSize);
        memcpy(top, bottom, lineSize);
        memcpy(bottom, lineBuffer, lineSize);

        top -= lineSize;
        bottom += lineSize;
    }

    free(lineBuffer);

    numChannels = (unsigned char)channels;
    width = (unsigned short)w;
    height = (unsigned short)h;
    *data = rawData;

    return true;
}

} // namespace gx_common
