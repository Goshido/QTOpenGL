#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <Windows.h>
#include "game.h"

namespace gx_engine {

class RendererThread;

class Renderer {
    friend class RendererThread;

public:
    ~Renderer();

    void setGame(Game* game);
    void resize(int width, int height);

    int getWidth() const;
    int getHeight() const;

    float getUIScale() const;

    static Renderer* __stdcall instance();

protected:
    explicit Renderer();

private:
    bool createWindow();
    void destroyWindow();
    void initOpenGL() const;
    void onFrame(float deltaTime);

    HWND getWindowHandle() const;

    static LRESULT CALLBACK onWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    Game* _game;
    bool _isRenderableObjectsInited;

    int _width;
    int _height;
    float _uiScale;

    HINSTANCE _hInstance;
    HWND _hwnd;
    HDC _hDC;
    HGLRC _renderContext;

    bool _isWindowClassRegistered;

    RendererThread* _thread;

    static Renderer* _renderer;
};

} // namespace gx_engine

#endif OPENGL_RENDERER_H
