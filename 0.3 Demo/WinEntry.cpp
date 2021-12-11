#include <Windows.h>

extern void Init(HINSTANCE hInstance);
extern int StartMessageLoop();

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine, int nCmdShow)
{
    Init(hInstance);

    return StartMessageLoop();
}
