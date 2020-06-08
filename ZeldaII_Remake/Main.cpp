#include "AlteredWindows.h"
#include "Graphics.h"
#include "resource.h"
#include "Game.h"


bool ProcessMessage();

// The procedure to deal with window-message inputs
LRESULT CALLBACK HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// The entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    // Setup the window
    WNDCLASSEX wc{ sizeof(WNDCLASSEX), CS_CLASSDC, HandleMessage, 0, 0, hInstance,
                 (HICON)LoadImage(hInstance, MAKEINTRESOURCE(GameIcon), IMAGE_ICON, 32, 32, 0),
                 nullptr, nullptr, nullptr, L"32BitGameWindow",
                 (HICON)LoadImage(hInstance, MAKEINTRESOURCE(GameIcon), IMAGE_ICON, 16, 16, 0) };

    RegisterClassEx(&wc);

    // Adjust window so the screenWidth and screenHeight are the dimensions inside the window
    RECT wr = { 0, 0, Graphics::screenWidth, Graphics::screenHeight };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    // Setup the window handler and dispaly the program's window
    HWND hWnd;

    hWnd = CreateWindowEx(NULL, L"32BitGameWindow", L"Zelda II : The Adventure of Link REMASTERED",
        WS_OVERLAPPEDWINDOW, 200, 100, wr.right - wr.left, wr.bottom - wr.top,
        NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);

    // Set up and initialize Direct3D / Graphics / GameStuffs
    Game game(hWnd);

    // Programs's Main Loop
    while (ProcessMessage())
    {
        
        game.Cycle();
    }
    return 0;
}

bool ProcessMessage()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
        {
            return false;
        }
    }
    return true;
}

// Called indirectly by DispatchMessage(), allows us to deal with inputs passed to the app window
//    by the OS and/or from the user, as all messages are passed as ENUMS
LRESULT CALLBACK HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    } break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
