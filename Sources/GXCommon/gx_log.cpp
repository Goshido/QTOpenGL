#include <GXCommon/gx_log.h>

namespace gx_common {

void __stdcall gxLogA(const QString format, ...)
{
    va_list ap;
    va_start(ap, format);
    vprintf(format.toLatin1().data(), ap);
    va_end(ap);
}

} // namespace gx_common
