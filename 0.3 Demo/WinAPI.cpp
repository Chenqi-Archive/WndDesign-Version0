#include <Windows.h>
#include <windowsx.h>
#include "Wnd.h"
#include "List.h"

#define MSG_SYS_NEWWND   0x01
#define MSG_SYS_GETWND   0x02
#define MSG_SYS_GETHWND  0x03
extern bool SendSysMsg(Wnd* wnd, uint msg, void* para);


// The map between HWND and Wnd class
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static ATOM RegisterBlankClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.lpszClassName = L"BlankWnd";

    return RegisterClassExW(&wcex);
}

static HWND CreateBlankWindow(HINSTANCE hInstance) {
    HWND hWnd = CreateWindowExW(NULL, L"BlankWnd", L"Name", WS_POPUP,
        120, 120, 800, 480,
        NULL, NULL, hInstance, NULL
    );

    if (!hWnd) {
        return NULL;
    }

    Wnd* wnd = new Wnd(800, 480, nullptr, MainWndHandler);
    wnd->AddSubWnd({ new Button(50, 30, wnd), {5,5}, WND_SHOW });
    wnd->AddSubWnd({ new Button(50, 30, wnd), {60,5}, WND_SHOW });
    wnd->AddSubWnd({ new Button(50, 30, wnd), {115,5}, WND_SHOW });
    wnd->AddSubWnd({ new Button(50, 30, wnd), {170,5}, WND_SHOW });
    wnd->AddSubWnd({ new Button(50, 30, wnd), {225,5}, WND_SHOW });

    wnd->AddSubWnd({ new Button(30, 30, wnd), {765,5}, WND_SHOW }); // close icon
    wnd->AddSubWnd({ new Button(30, 30, wnd), {730,5}, WND_SHOW });


    wnd->AddSubWnd({ new Wnd(270,435,wnd),{5,40},WND_SHOW });
    wnd->AddSubWnd({ new Wnd(515,435,wnd),{280,40},WND_SHOW });
    wnd->AddSubWnd({ new Wnd(445,30,wnd),{280,5},WND_SHOW });

    SendSysMsg(nullptr, MSG_SYS_NEWWND, &{ hWnd, wnd });

    ShowWindow(hWnd, SW_SHOW);
    //UpdateWindow(hWnd);

    return hWnd;
}

// Initialize: Register blank window class, create first window, 
//   and start painting with Wnd.
void Init(HINSTANCE hInstance) {
    RegisterBlankClass(hInstance);
    CreateBlankWindow(hInstance);
}

int StartMessageLoop()
{
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}


// Global message handler
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int mouseState = 0;  // 0: nothing  1: left down
    switch (msg)
    {
    case WM_MOUSEMOVE: {
        //if (mouseState) {
        //    Wnd* wnd = globalWndMap.GetWnd(hWnd);
        //    wnd->OnClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

        //    InvalidateRect(hWnd, NULL, true);
        //    UpdateWindow(globalWndMap.GetHWND(wnd));
        //}
    }break;
    case WM_LBUTTONDOWN: {
        Wnd* wnd = WndMapGetWnd(hWnd);
        wnd->OnClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

        SendSysMsg(nullptr, )

        InvalidateRect(hWnd, NULL, true);
        UpdateWindow(WndMapGetHWND(wnd));

        mouseState = 1;
    }break;
    case WM_LBUTTONUP: {
        mouseState = 0;
    }break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HDC mdc = CreateCompatibleDC(hdc);
        RECT rect;
        GetWindowRect(hWnd, &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        // Get bitmap drawed.
        HBITMAP hBmp = CreateBitmap(width, height, 1, 32, WndMapGetWnd(hWnd)->GetBitmap());
        
        //BITMAP bmp;
        //GetObject(hBmp, sizeof(BITMAP), &bmp);

        SelectObject(mdc, hBmp);
        BitBlt(hdc, 0, 0, width, height, mdc, 0, 0, SRCCOPY);

        EndPaint(hWnd, &ps);
    }break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}