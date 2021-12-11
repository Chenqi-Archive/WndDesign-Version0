#include "WinAPI.h"
#include "SysWndMng.h"
#include "../shape/Pattern.h"
#include <Windows.h>
#include <windowsx.h>

using namespace WndDesign;

extern HINSTANCE hInstance;
static const wchar_t WndClassName[] = L"BlankWnd";
static bool WndClassRegistered = false;

static inline RECT Rect2RECT(Rect region) {
    RECT rect;
    rect.left = region.pos.x;
    rect.right = region.pos.x + region.size.width;
    rect.top = region.pos.y;
    rect.bottom = region.pos.y + region.size.height;
    return rect;
}

static inline Rect RECT2Rect(RECT rect) {
    Rect region;
    region.pos.x = rect.left;
    region.pos.y = rect.top;
    region.size.width = rect.right - rect.left;
    region.size.height = rect.bottom - rect.top;
    return region;
}

static void SysPaintWnd(HWND hWnd, const Bitmap* bitmap);

// Global message handler
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Only for mouse messages.
    Pos mousePos = {GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};

    switch (msg) {
        case WM_MOUSEMOVE:
            ReceiveSysMsg(hWnd, Msg::MOUSE_MOVEON, &mousePos);
            break;
        case WM_LBUTTONDOWN:
            SetCapture(hWnd);
            ReceiveSysMsg(hWnd, Msg::MOUSE_LEFTDOWN, &mousePos);
            break;
        case WM_LBUTTONUP:
            ReceiveSysMsg(hWnd, Msg::MOUSE_LEFTUP, &mousePos);
            ReleaseCapture();
            break;
        case WM_RBUTTONUP:
            ReceiveSysMsg(hWnd, Msg::MOUSE_RIGHTUP, &mousePos);
            break;
        case WM_PAINT:
            const Bitmap* bitmap;
            ReceiveSysMsg(hWnd, Msg::WND_PAINT, &bitmap);
            SysPaintWnd(hWnd, bitmap);
            break;
        case WM_DESTROY:
            // May check if the window is main window.
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

static ATOM RegisterBlankClass() {
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.lpszClassName = WndClassName;

    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

    return RegisterClassExW(&wcex);
}

SysID SysCreateWnd(Rect rect) {
    if (!WndClassRegistered) {
        if (RegisterBlankClass()) { WndClassRegistered = true; } 
        else { return NULL; }
    }
    HWND hWnd = CreateWindowExW(NULL, WndClassName, L"Name", WS_POPUP,
                           rect.pos.x, rect.pos.y, rect.size.width, rect.size.height,
                           NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    return hWnd;
}

void SysDestroyWnd(SysID sysid) {
    PostMessage((HWND)sysid, WM_DESTROY, NULL, NULL);
}

void SysShowWnd(HWND hWnd) {
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
}

static void SysPaintWnd(HWND hWnd, const Bitmap* bitmap) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    HDC mdc = CreateCompatibleDC(hdc);

    Size size = bitmap->GetSize();
    HBITMAP hBmp = CreateBitmap(size.width, size.height, 1, 32, bitmap->GetBits());
    SelectObject(mdc, hBmp);

    Rect rect = RECT2Rect(ps.rcPaint);
    BitBlt(hdc, rect.pos.x, rect.pos.y, rect.size.width, rect.size.height,
           mdc, rect.pos.x, rect.pos.y, SRCCOPY);

    DeleteObject(hBmp);
    DeleteDC(mdc);
    ReleaseDC(hWnd, hdc);
    EndPaint(hWnd, &ps);
}

//void SysMoveWnd(SysID sysid, Pos vector) {
//    RECT rect;
//    GetWindowRect((HWND)sysid, &rect);
//    Pos pos = {rect.left + vector.x, rect.top + vector.y};
//    Size size = {rect.right - rect.left, rect.bottom - rect.top};
//    MoveWindow((HWND)sysid, pos.x, pos.y, size.width, size.height, true);
//}

void SysUpdateWnd(SysID sysid, Rect region) {
    HWND hWnd = (HWND)sysid;
    RECT rect = Rect2RECT(region); 
    InvalidateRect(hWnd, &rect, true);
    UpdateWindow(hWnd);
}