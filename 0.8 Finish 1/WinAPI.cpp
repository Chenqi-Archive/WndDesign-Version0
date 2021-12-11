#include "./WinAPI.h"
#include <windowsx.h>

extern HINSTANCE hInstance;

static const wchar_t WndClassName[] = L"BlankWnd";

static bool WndClassRegistered = false;

// Global message handler
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Only for mouse messages.
    Pos mousePos = { GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam) };

    switch (msg)
    {
    case WM_MOUSEMOVE:
        ReceiveSysMsg(hWnd, MSG_MOUSE_MOVEON, &mousePos);
        break;
    case WM_LBUTTONDOWN:
        SetCapture(hWnd);
        ReceiveSysMsg(hWnd, MSG_MOUSE_LEFTDOWN, &mousePos);
        break;
    case WM_LBUTTONUP:
        ReceiveSysMsg(hWnd, MSG_MOUSE_LEFTUP, &mousePos);
        ReleaseCapture();
        break;
    case WM_PAINT:
        ReceiveSysMsg(hWnd, MSG_WND_PAINT, nullptr);
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

// It should be put at Init.
static ATOM RegisterBlankClass() {
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.lpszClassName = WndClassName;

    wcex.hCursor = LoadCursor(nullptr, IDC_HAND);

    return RegisterClassExW(&wcex);
}

// Below are message handlers for system message.

// Create a blank popup window.
HWND SysCreateWnd(Pos pos, Size size){
    if (!WndClassRegistered) {
        if (RegisterBlankClass()) {
            WndClassRegistered = true;
        }
        else{
            return NULL;
        }
    }

    return CreateWindowExW(NULL, WndClassName, L"Name", WS_POPUP,
        pos.x, pos.y, size.width, size.height,
        NULL, NULL, hInstance, NULL
    );
}

void SysShowWnd(HWND hWnd){
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
}

// Paint hWnd with bitmap bits.
void SysPaintWnd(HWND hWnd, const Bitmap& bitmap) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    HDC mdc = CreateCompatibleDC(hdc);
    RECT rect;
    GetWindowRect(hWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    //Size is assumed to be equal.


    // Get bitmap to draw.
    HBITMAP hBmp = CreateBitmap(width, height, 1, 32, bitmap.GetBits());

    // Could only draw invalid rect.


    SelectObject(mdc, hBmp);
    BitBlt(hdc, 0, 0, width, height, mdc, 0, 0, SRCCOPY);

    // Forgot to free at begin, detected when checking in task manager!
    DeleteObject(hBmp);
    DeleteDC(mdc);
    ReleaseDC(hWnd, hdc);

    EndPaint(hWnd, &ps);
}

void SysMoveWnd(HWND hWnd, Pos vector) {
    RECT rect;
    GetWindowRect(hWnd, &rect);
    Pos pos = { rect.left + vector.x, rect.top + vector.y };
    Size size = { rect.right - rect.left, rect.bottom - rect.top };
    MoveWindow(hWnd, pos.x, pos.y, size.width, size.height, true);
}

int SysMsgLoop()
{
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}