#include <windows.h>
#include <cstdio>
#include "./lib/allocator.h"

HINSTANCE hInstance;

extern int Init();


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
    LPWSTR lpCmdLine, int nCmdShow)
{
#ifdef CONSOLE_DEBUG 
    InitConsole();

    FinishConsole(); 
#endif

    // Save hInstance for CreateWindow
    hInstance = hInst;

    Init();
}
