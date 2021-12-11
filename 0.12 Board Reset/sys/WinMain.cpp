#include <windows.h>
#include "SysWndMng.h"

HINSTANCE hInstance;

WndDesign::Wnd* master;
WndDesign::SysWndMng* sysWndMng;

extern void Init();

int MsgLoop() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    ::hInstance = hInstance;

    sysWndMng = new WndDesign::SysWndMng;
    master = sysWndMng;

    Init();

    int ret = MsgLoop();

    delete sysWndMng;
    master = sysWndMng = nullptr;

    return ret;
}
