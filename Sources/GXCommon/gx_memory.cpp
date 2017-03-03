#include <GXCommon/gx_memory.h>

namespace gx_common {

GXCircleBuffer::GXCircleBuffer(unsigned int size)
    : _size(size)
    , _offset(0)
{
    _buffer = (unsigned char*)malloc(size);
}

GXCircleBuffer::~GXCircleBuffer()
{
    free(_buffer);
}

void* GXCircleBuffer::allocate(unsigned int size)
{
    if (size == 0) return nullptr;

    if ((_offset + size) >= _size) {
        _offset = size;
        return _buffer;
    }

    unsigned char* b = _buffer + _offset;
    _offset += size;

    return b;
}

} // namespace gx_common
