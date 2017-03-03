#ifndef GX_TIME_H
#define GX_TIME_H

namespace gx_common {

class GXTimer {
public:
    explicit GXTimer();
    virtual ~GXTimer();

    float elapse();

private:
    double _ticksPerSecond;
    double _startTimeStamp;
};

} // namespace gx_common

#endif GX_TIME_H
