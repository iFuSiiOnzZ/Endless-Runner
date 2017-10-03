
/*
    Project includes
*/

#include "engine\font.h"

#include "engine\animation.h"

#include "engine\resource_manager.h"

#include "game\utils\ContentLoader.h"
#include "game\environment\ground.h"

#include "game\enemies\Slim.h"

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
static bool GlobalExit = false;

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
        GlobalExit = true;
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

    CResourceManager *r = CResourceManager::GetInstance();
    CContentLoader::Load();

    CGraphicsManager graphics_manager;
    CGround ground(0, WINDOW_HEIGHT - 93);
    CFont Font("C:/Windows/Fonts/arialbd.ttf", 30.0f);

    float current_frame_time = 0.0f;
    float fix_frame_time = 1.0f / 60.0f;
    float last_frame_time = seconds_now();

    CSlim Slim(WINDOW_WIDTH - 200, 100.0f);

    while (!GlobalExit)
    {
        while (PeekMessage(&hMsg, NULL, 0U, 0U, PM_REMOVE) > 0)
        {
            TranslateMessage(&hMsg);
            DispatchMessageA(&hMsg);
        }

        current_frame_time = seconds_now();
        float dt = current_frame_time - last_frame_time;

        window_dimensions_t WndDimensions = GetWindowDimension(hWnd);
        game_offscreen_buffer_t GameBuffer = { 0 };

        GameBuffer.Memory = GlobalBackBuffer.Memory;
        GameBuffer.BytesPerPixel = GlobalBackBuffer.BytesPerPixel;

        GameBuffer.Height = GlobalBackBuffer.Height;
        GameBuffer.Width = GlobalBackBuffer.Width;
        GameBuffer.Pitch = GlobalBackBuffer.Pitch;

        CGraphicsManager::ClearBuffer(&GameBuffer, 0.0f, 0.0f, 0.0f);
        unsigned int y = Font.DrawString(&graphics_manager, &GameBuffer, 10, 10, 1.0f, 1.0f, 1.0f, "Frame time: %.3fs", dt) + 10;

        while (dt > 0.0f)
        {
            float delta_time = (dt < fix_frame_time) ? dt : fix_frame_time;

            ground.Update(dt);
            Slim.Update(dt);

            dt -= fix_frame_time;
        }


        CGraphicsManager::DrawBitmap(&GameBuffer, r->Get<bitmap_t>("tree"), 100.0f, 100.0f);
        CGraphicsManager::DrawBitmap(&GameBuffer, r->Get<bitmap_t>("sun"), 700.0f, 0.0f);

        ground.Render(&graphics_manager, &GameBuffer);
        Slim.Render(&graphics_manager, &GameBuffer);

        y += Font.DrawString(&graphics_manager, &GameBuffer, 10, y, 1.0f, 1.0f, 1.0f, "Hello World");
        y += Font.DrawString(&graphics_manager, &GameBuffer, 10, y, 1.0f, 1.0f, 1.0f, "This is a test");

        DisplayBuffer(&GlobalBackBuffer, DeviceContext, WndDimensions.Width, WndDimensions.Height);
        last_frame_time = current_frame_time;
    }

    CContentLoader::UnLoad();
    CResourceManager::Destroy();

    UnregisterClassA(CLASS_NAME, wndCls.hInstance);
    return EXIT_SUCCESS;
}