#include <windows.h>
#include "../wnd/WndMng.h"

HINSTANCE hInstance;

extern void Init();

using namespace WndDesign;

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInstance,
                      LPWSTR lpCmdLine, int nCmdShow) {
    hInstance = hInst;

    if (!InitWndMng()) {
        return 0;
    }

    Init();

    if (!InitEnd()) {
        return 0;
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
