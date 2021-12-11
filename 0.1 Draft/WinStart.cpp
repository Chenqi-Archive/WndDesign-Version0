#include <Windows.h>
#include <tchar.h>

//
extern BITMAP board;
//

static TCHAR szClassName[] = _T("PlainWndClass");
static TCHAR szTitle[] = _T("PlainWindow");

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine, int nShowCmd) {

    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpszClassName = szClassName;
    wcex.lpfnWndProc = WndProc;
    if (!RegisterClassEx(&wcex)) {
        return 0;
    }

    HWND hWnd = CreateWindow(szClassName, NULL, WS_OVERLAPPED,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 480,
        NULL, NULL, hInstance, NULL);
    if (hWnd == NULL) {
        return 0;
    }

    ShowWindow(hWnd, SW_SHOW);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}


static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;
    
    switch(uMsg) {
    case WM_CREATE:
        // Initialize bitmap and painting thread

        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        
        RECT rect;
        GetWindowRect(hWnd, &rect);
        MoveToEx(hdc, 0, 0, NULL);
        LineTo(hdc, rect.right - rect.left, rect.bottom - rect.top);


        // Send current window info for paint function

        // Get bitmap painted

        EndPaint(hWnd, &ps);
        break;
    // Mouse messages
    case WM_LBUTTONDOWN:





    // Other messages
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}






