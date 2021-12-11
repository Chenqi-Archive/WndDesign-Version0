#include <windows.h>
#include <cstdio>
#include "./lib/allocator.h"
#include <crtdbg.h>

HINSTANCE hInstance;

extern void Init();

//#define CONSOLE_DEBUG
#ifdef CONSOLE_DEBUG
void InitConsole() {
    AllocConsole();
    freopen("CON", "r", stdin);
    freopen("CON", "w", stdout);
}
void FinishConsole() {
    FreeConsole();
}
#endif

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE hPrevInstance,
                      LPWSTR lpCmdLine, int nCmdShow) {
#ifdef CONSOLE_DEBUG 
    InitConsole();

    FinishConsole();
    return 0;
#endif

    hInstance = hInst;

    Init();

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
