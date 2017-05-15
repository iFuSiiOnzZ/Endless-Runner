
/*
    Project includes
*/
#include "engine\graphics.h"
#include "engine\animation.h"
#include "engine\stb_image.h"

#include "game\Utils\ResourceManager.h"

/*
    Libs include
*/

/*
    STDLib/OS includes
*/
#include <Windows.h>

///////////////////////////////////////////////////////////////////////////////

#ifndef UNUSED
    #define UNUSED(x) (void)(x)
#endif

#define WINDOW_NAME "Endless Runner"
#define CLASS_NAME   "EndlessRunner"

#define WINDOW_HEIGHT    720
#define WINDOW_WIDTH    1280

///////////////////////////////////////////////////////////////////////////////

typedef struct offscreen_buffer_t
{
    BITMAPINFO Info;
    void *Memory;

    int Width;
    int Height;

    int Pitch;
    int BytesPerPixel;
} offscreen_buffer_t;

typedef struct  window_dimensions_t
{
    int Width;
    int Height;
} window_dimensions_t;

///////////////////////////////////////////////////////////////////////////////

static offscreen_buffer_t GlobalBackBuffer = { 0 };

///////////////////////////////////////////////////////////////////////////////

static window_dimensions_t GetWindowDimension(HWND Window)
{
    RECT ClientRect = { 0 };
    GetClientRect(Window, &ClientRect);

    window_dimensions_t r = { 0 };
    r.Width = ClientRect.right - ClientRect.left;
    r.Height = ClientRect.bottom - ClientRect.top;

    return r;
}


static void ResizeBIBSection(offscreen_buffer_t *Buffer, int Width, int Height)
{
    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biHeight = -Height;
    Buffer->Info.bmiHeader.biWidth = Width;

    Buffer->Info.bmiHeader.biCompression = BI_RGB;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biPlanes = 1;

    Buffer->Height = Height;
    Buffer->Width = Width;

    Buffer->Pitch = Width * 4;
    Buffer->BytesPerPixel = 4;

    size_t BitmapMemorySize = (size_t)(4 * Width * Height);
    if (Buffer->Memory != NULL) VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

static void DisplayBuffer(offscreen_buffer_t *Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight)
{
    int  OffsetX = 0, OffsetY = 0;

    PatBlt(DeviceContext, 0, 0, WindowWidth, OffsetY, BLACKNESS);
    PatBlt(DeviceContext, 0, OffsetY + Buffer->Height, WindowWidth, WindowHeight, BLACKNESS);

    PatBlt(DeviceContext, 0, 0, OffsetX, WindowHeight, BLACKNESS);
    PatBlt(DeviceContext, OffsetX + Buffer->Width, 0, WindowWidth, WindowHeight, BLACKNESS);

    StretchDIBits
    (
        DeviceContext,

        OffsetX, OffsetY, Buffer->Width, Buffer->Height,
        0, 0, Buffer->Width, Buffer->Height,

        Buffer->Memory, &Buffer->Info,
        DIB_RGB_COLORS, SRCCOPY
    );
}

float seconds_now()
{
    static LARGE_INTEGER s_frequency = { 0 };
    static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);

    LARGE_INTEGER now = { 0 };
    QueryPerformanceCounter(&now);
    return (float) ((double)now.QuadPart / (double)s_frequency.QuadPart);
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT hMsg, WPARAM wParam, LPARAM lParam)
{
    if (hMsg == WM_CLOSE)
    {
        DestroyWindow(hWnd);
        return 0;
    }
    else if (hMsg == WM_DESTROY)
    {
        PostQuitMessage(EXIT_SUCCESS);
        return 0;
    }
    else if (hMsg == WM_PAINT)
    {
        PAINTSTRUCT Paint = { 0 };
        HDC DeviceContext = BeginPaint(hWnd, &Paint);

        window_dimensions_t d = GetWindowDimension(hWnd);
        ResizeBIBSection(&GlobalBackBuffer, d.Width, d.Height);
        DisplayBuffer(&GlobalBackBuffer, DeviceContext, d.Width, d.Height);

        EndPaint(hWnd, &Paint);
    }

    return DefWindowProcA(hWnd, hMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hActualInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow)
{
    UNUSED(hPrevInst);
    UNUSED(cmdLine);
    UNUSED(cmdShow);

    WNDCLASSEXA	wndCls = { 0 };
    HWND hWnd = nullptr;

    wndCls.hInstance = hActualInst;
    wndCls.cbSize = sizeof(WNDCLASSEXA);

    wndCls.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndCls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndCls.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    wndCls.lpfnWndProc = WndProc;
    wndCls.lpszClassName = CLASS_NAME;
    wndCls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassExA(&wndCls))
    {
        MessageBoxA(NULL, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return EXIT_FAILURE;
    }

    int StartX = GetSystemMetrics(SM_CXSCREEN) / 2 - WINDOW_WIDTH / 2;
    int StartY = GetSystemMetrics(SM_CYSCREEN) / 2 - WINDOW_HEIGHT / 2;

    hWnd = CreateWindowExA
    (
        WS_EX_CLIENTEDGE, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, StartX, StartY,
        WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hActualInst, NULL
    );

    if (!hWnd)
    {
        MessageBoxA(NULL, "Window creation failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return EXIT_FAILURE;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    HDC DeviceContext = GetDC(hWnd);
    MSG hMsg = { 0 };

    bitmap_t bmp_tree = { 0 };
    bitmap_t bmp_sun = { 0 };

    bmp_tree.Memory = stbi_load("..\\assets\\tree2.png", &bmp_tree.Width, &bmp_tree.Height, &bmp_tree.BytesPerPixel, 0);
    bmp_tree.Pitch = bmp_tree.Width * bmp_tree.BytesPerPixel;

    bmp_sun.Memory = stbi_load("..\\assets\\sun.png", &bmp_sun.Width, &bmp_sun.Height, &bmp_sun.BytesPerPixel, 0);
    bmp_sun.Pitch = bmp_sun.Width * bmp_sun.BytesPerPixel;

    std::vector<animation_frame_t> frames;
    animation_frame_t frame = { 0 };

    CResourceManager *r = CResourceManager::GetInstance();
    r->Register<bitmap_t>();
    r->Add<bitmap_t>("tree", &bmp_tree);
    r->Add<bitmap_t>("sun", &bmp_sun);

    bitmap_t *t = r->Get<bitmap_t>("tree");
    bitmap_t *e = r->Get<bitmap_t>("treeeee");

    for (int i = 0; i < 5; ++i)
    {
        char s[256] = { 0 };
        sprintf_s(s, "..\\assets\\run_anim%d.png", i + 1);

        frame.bitmap.Memory = (stbi_load(s, &frame.bitmap.Width, &frame.bitmap.Height, &frame.bitmap.BytesPerPixel, 0));
        frame.bitmap.Pitch = frame.bitmap.Width * frame.bitmap.BytesPerPixel;
        frame.duration = 0.12f;

        frames.push_back(frame);
    }

    CGraphicsManager graphics_manager;
    CAnimation player_animation(frames);

    float current_frame_time = 0.0f;
    float last_frame_time = seconds_now();
    float fix_frame_time = 1.0f / 60.0f;

    while (hMsg.message != WM_QUIT)
    {
        current_frame_time = seconds_now();
        float dt = current_frame_time - last_frame_time;

        while (PeekMessage(&hMsg, NULL, 0U, 0U, PM_REMOVE) > 0)
        {
            TranslateMessage(&hMsg);
            DispatchMessageA(&hMsg);
        }

        while (dt > 0.0f)
        {
            float delta_time = (dt < fix_frame_time) ? dt : fix_frame_time;
            player_animation.Update(delta_time);
            dt -= fix_frame_time;
        }

        window_dimensions_t WndDimensions = GetWindowDimension(hWnd);
        game_offscreen_buffer_t GameBuffer = { 0 };

        GameBuffer.Memory = GlobalBackBuffer.Memory;
        GameBuffer.BytesPerPixel = GlobalBackBuffer.BytesPerPixel;

        GameBuffer.Height= GlobalBackBuffer.Height;
        GameBuffer.Width = GlobalBackBuffer.Width;
        GameBuffer.Pitch = GlobalBackBuffer.Pitch;

        CGraphicsManager::ClearBuffer(&GameBuffer, 0.0f, 0.0f, 0.0f);

        CGraphicsManager::DrawBitmap(&GameBuffer, &bmp_tree, 100.0f, 100.0f);
        CGraphicsManager::DrawBitmap(&GameBuffer, &bmp_sun, 700.0f, 0.0f);

        player_animation.Render(&graphics_manager, &GameBuffer, 700.0f, 200.0f);

        DisplayBuffer(&GlobalBackBuffer, DeviceContext, WndDimensions.Width, WndDimensions.Height);
        last_frame_time = current_frame_time;
    }

    stbi_image_free(bmp_tree.Memory);
    stbi_image_free(bmp_sun.Memory);

    for (size_t i = 0; i < frames.size(); ++i) stbi_image_free(frames[i].bitmap.Memory);
    frames.clear();

    CResourceManager::Destroy();
    UnregisterClassA(CLASS_NAME, wndCls.hInstance);
    return EXIT_SUCCESS;
}