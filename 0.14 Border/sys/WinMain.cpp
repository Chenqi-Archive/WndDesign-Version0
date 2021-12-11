#include <windows.h>
#include "SysWndMng.h"

HINSTANCE hInstance;

WndDesign::SysWndMng* master;

extern void Init();

inline int MsgLoop() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    ::hInstance = hInstance;

    master = new WndDesign::SysWndMng;

    Init();

    int ret = 0;

    if (master->SubWndCnt() > 0) {
        ret = MsgLoop();
    }

    delete master;
    master = nullptr;

    return ret;
}
