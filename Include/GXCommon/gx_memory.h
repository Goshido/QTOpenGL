#ifndef GX_MEMORY_H
#define GX_MEMORY_H

#include <string>

#define gxSafeDelete(ptr) \
    if (ptr) { \
        delete ptr; \
        ptr = nullptr; \
    }

#define gxSafeFree(ptr) \
    if (ptr) { \
        free(ptr); \
        ptr = nullptr; \
    }

namespace gx_common {

class GXCircleBuffer {
public:
    GXCircleBuffer(unsigned int size);
    ~GXCircleBuffer();

    void* allocate(unsigned int size);

private:
    unsigned char* _buffer;
    unsigned int _size;
    unsigned int _offset;
};

} // namespace gx_common

#endif GX_MEMORY_H
