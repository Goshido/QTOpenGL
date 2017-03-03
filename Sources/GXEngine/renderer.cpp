#include <GXEngine/renderer.h>
#include <GXEngine/game.h>
#include <GXEngine/opengl.h>
#include <GXEngine/renderer_thread.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")

namespace gx_engine {

#define DEFAULT_WIDTH           800
#define DEFAULT_HEIGHT          600

#define DEFAULT_COLOR_BITS      32
#define DEFAULT_DEPTH_BITS      24
#define DEFAULT_STENCIL_BITS    8

#define WND_CLASS_NAME          L"OpenGLRenderer"
#define WINDOW_NAME             L"Renderer"

#define OPENGL_VERSION_MAJOR    3
#define OPENGL_VERSION_MINOR    0

Renderer* Renderer::_renderer = 0;

Renderer::~Renderer()
{
    // NOTHING
}

void Renderer::setGame(Game* game)
{
    _game = game;
}

void Renderer::resize(int width, int height)
{
    bool isNeedResize = false;

    if (width < 0 || height < 0) return;

    if (_width != width) {
        _width = width;
        isNeedResize = true;
    }

    if (_height != height) {
        _height = height;
        isNeedResize = true;
    }

    if (!isNeedResize) return;

    if (_game) {
        if (!_isRenderableObjectsInited) {
            _game->onInitRenderer();
            _isRenderableObjectsInited = true;
        }

        _game->onResize(_width, _height);
    }
}

int Renderer::getWidth() const
{
    return _width;
}

int Renderer::getHeight() const
{
    return _height;
}

float Renderer::getUIScale() const
{
    return _uiScale;
}

Renderer* __stdcall Renderer::instance()
{
    if (!_renderer) {
        _renderer = new Renderer();
    }

    return _renderer;
}

Renderer::Renderer()
    : _game(nullptr)
    , _isRenderableObjectsInited(false)
    , _width(DEFAULT_WIDTH)
    , _height(DEFAULT_HEIGHT)
    , _uiScale(1.0f)
    , _hInstance(0)
    , _hwnd(0)
    , _hDC(0)
    , _renderContext(0)
    , _isWindowClassRegistered(false)
{
    _thread = new RendererThread();
    QObject::connect(_thread, &RendererThread::finished, _thread, &QObject::deleteLater);
    _thread->start();
}

bool Renderer::createWindow()
{
    _hInstance = GetModuleHandleW(0);

    WNDCLASSEXW wndClass;
    memset(&wndClass, 0, sizeof(WNDCLASSEXW));
    wndClass.hInstance = _hInstance;
    wndClass.cbSize = sizeof(WNDCLASSEXW);
    wndClass.lpfnWndProc = &Renderer::onWindowProc;
    wndClass.lpszClassName = WND_CLASS_NAME;
    wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndClass.cbWndExtra = 0;
    wndClass.cbClsExtra = 0;
    wndClass.hIcon = LoadIconW(0, IDI_WINLOGO);
    wndClass.hIconSm = wndClass.hIcon;
    wndClass.hCursor = LoadCursorW(0, IDC_ARROW);
    wndClass.hbrBackground = 0;
    wndClass.lpszMenuName = 0;

    if (!RegisterClassExW(&wndClass)) return false;

    _isWindowClassRegistered = true;

    RECT rect;
    rect.left = (LONG)0;
    rect.right = (LONG)_width;
    rect.top = (LONG)0;
    rect.bottom = (LONG)_height;

    DWORD extendedStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD style = WS_OVERLAPPEDWINDOW;

    AdjustWindowRectEx(&rect, style, FALSE, extendedStyle);

    _hwnd = CreateWindowExW(extendedStyle,
                            WND_CLASS_NAME,
                            WINDOW_NAME,
                            WS_CLIPSIBLINGS | WS_CLIPCHILDREN | style,
                            0,
                            0,
                            _width,
                            _height,
                            0,
                            0,
                            _hInstance,
                            0);

    if (!_hwnd) return false;

    _hDC = GetDC(_hwnd);

    static PIXELFORMATDESCRIPTOR pixelFormat;
    pixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pixelFormat.nVersion = 1;
    pixelFormat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pixelFormat.iPixelType = PFD_TYPE_RGBA;
    pixelFormat.cColorBits = DEFAULT_COLOR_BITS;
    pixelFormat.cRedBits = 0;
    pixelFormat.cRedShift = 0;
    pixelFormat.cGreenBits = 0;
    pixelFormat.cGreenShift = 0;
    pixelFormat.cBlueBits = 0;
    pixelFormat.cBlueShift = 0;
    pixelFormat.cAlphaBits = 0;
    pixelFormat.cAlphaShift = 0;
    pixelFormat.cAccumBits = 0;
    pixelFormat.cAccumRedBits = 0;
    pixelFormat.cAccumGreenBits = 0;
    pixelFormat.cAccumBlueBits = 0;
    pixelFormat.cDepthBits = DEFAULT_DEPTH_BITS;
    pixelFormat.cStencilBits = 8;
    pixelFormat.cAuxBuffers = 0;
    pixelFormat.iLayerType = PFD_MAIN_PLANE;
    pixelFormat.bReserved = 0;
    pixelFormat.dwLayerMask = 0;
    pixelFormat.dwVisibleMask = 0;
    pixelFormat.dwDamageMask = 0;

    GLuint matchFormat = ChoosePixelFormat(_hDC, &pixelFormat);

    if (!matchFormat) {
        ReleaseDC(_hwnd, _hDC);
        return false;
    }

    if (!SetPixelFormat(_hDC, matchFormat, &pixelFormat)) {
        ReleaseDC(_hwnd, _hDC);
        return false;
    }

    _renderContext = wglCreateContext(_hDC);
    if (!_renderContext) {
        ReleaseDC(_hwnd, _hDC);
        return false;
    }

    if (!wglMakeCurrent(_hDC, _renderContext)) {
        ReleaseDC(_hwnd, _hDC);
        return false;
    }

    initOpenGLBackend();

    wglMakeCurrent(0,0);
    wglDeleteContext(_renderContext);
    if (!wglCreateContextAttribsARB) {
        ReleaseDC(_hwnd, _hDC);
        return false;
    }

    int attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB,  OPENGL_VERSION_MAJOR,
        WGL_CONTEXT_MINOR_VERSION_ARB,  OPENGL_VERSION_MINOR,
        WGL_CONTEXT_FLAGS_ARB,          WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        WGL_CONTEXT_PROFILE_MASK_ARB,   WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    _renderContext = wglCreateContextAttribsARB(_hDC, 0, attribs);
    if (!_renderContext || !wglMakeCurrent(_hDC, _renderContext)) {
        ReleaseDC(_hwnd, _hDC);
        return false;
    }

    initOpenGL();

    ShowWindow(_hwnd,SW_SHOWNORMAL);
    SetForegroundWindow(_hwnd);
    SetFocus(_hwnd);

    wglSwapIntervalEXT(0);
    _uiScale = GetDeviceCaps(_hDC, LOGPIXELSX) / 2.54f;

    ShowCursor(TRUE);

    return true;
}

void Renderer::destroyWindow()
{
    if (_game && _isRenderableObjectsInited) {
        _game->onDestroyRenderer();

        _isRenderableObjectsInited = false;
    }

    if (_renderContext) {
        if (!wglMakeCurrent(0, 0)) {
            if (wglDeleteContext(_renderContext)) {
                _renderContext = 0;
            }
        }
    }

    if (_hwnd && _hDC && ReleaseDC(_hwnd, _hDC)) {
        _hDC = 0;
    }

    if (_hwnd && DestroyWindow(_hwnd)) {
        _hwnd = 0;
    }

    if (_hInstance && _isWindowClassRegistered) {
        if (UnregisterClassW(WND_CLASS_NAME, _hInstance)) {
            _isWindowClassRegistered = false;
        }

        _hInstance = 0;
    }
}

void Renderer::initOpenGL() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
}

void Renderer::onFrame(float deltaTime)
{
    if (_game && !_isRenderableObjectsInited) {
        _game->onInitRenderer();
        _isRenderableObjectsInited = true;
    }

    if (_game) {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        _game->onFrame(deltaTime);
    }

    SwapBuffers(_hDC);
}

HWND Renderer::getWindowHandle() const
{
    return _hwnd;
}

LRESULT CALLBACK Renderer::onWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_SYSCOMMAND: {
            switch (wParam) {
                case SC_SCREENSAVE: return 0;
                case SC_MONITORPOWER: return 0;
                default: {
                    // NOTHING
                    break;
                }
            }
            break;
        }
        case WM_CLOSE: {
            PostQuitMessage(0);
            return 0;
        }
        case WM_SIZE: {
            Renderer::instance()->resize((int)(LOWORD(lParam)), (int)(HIWORD(lParam)));
            return 0;
        }
        default: {
            // NOTHING
            break;
        }
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

} // namespace gx_engine
