#ifndef RENDERER_THREAD_H
#define RENDERER_THREAD_H

#include <GXCommon/gx_time.h>
#include <QThread>

namespace gx_engine {

class RendererThread : public QThread {
    Q_OBJECT
public:
    explicit RendererThread(QObject* parent = 0);
    virtual ~RendererThread();

protected:
    void run() override;

private:
    gx_common::GXTimer _timer;
};

} // namespace gx_engine

#endif RENDERER_THREAD_H
