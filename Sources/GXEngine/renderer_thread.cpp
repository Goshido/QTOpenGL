#include <GXEngine/renderer_thread.h>
#include <GXEngine/renderer.h>
#include <GXCommon/gx_time.h>
#include <GXCommon/gx_log.h>

#include <QCoreApplication>

namespace gx_engine {

RendererThread::RendererThread(QObject* parent)
    : QThread(parent)
{
    // NOTHING
}

RendererThread::~RendererThread()
{
    // NOTHING
}

void RendererThread::run()
{
    Renderer* renderer = Renderer::instance();

    if (!renderer->createWindow()) {
        renderer->destroyWindow();
        return;
    }

    MSG message;
    bool isWorking = true;
    HWND hwnd = renderer->getWindowHandle();

    while (isWorking) {
        if (PeekMessageW(&message, hwnd, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT) {
                isWorking = false;
            } else {
                TranslateMessage(&message);
                DispatchMessageW(&message);
            }
        }

        renderer->onFrame(_timer.elapse());
    }

    renderer->destroyWindow();
    delete renderer;
    QCoreApplication::quit();
}

} // namespace openg_demoы
