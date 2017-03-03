#include <GXCommon/gx_time.h>
#include <Windows.h>
#include <GXCommon/gx_log.h>

#pragma comment(lib, "advapi32")

namespace gx_common {

GXTimer::GXTimer()
{
    LARGE_INTEGER ticks;
    QueryPerformanceCounter(&ticks);
    _startTimeStamp = (double)ticks.QuadPart;

    QueryPerformanceFrequency(&ticks);
    _ticksPerSecond = (double)ticks.QuadPart;
}

GXTimer::~GXTimer() {
    // NOTHING
}

float GXTimer::elapse()
{
    LARGE_INTEGER ticks;
    QueryPerformanceCounter(&ticks);
    double now = (double)ticks.QuadPart;
    float ans = (float)((now - _startTimeStamp) / _ticksPerSecond);

    _startTimeStamp = now;

    return ans;
}

} // namespace gx_common
