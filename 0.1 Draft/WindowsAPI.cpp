
#include <Windows.h>


// may use win32 assembly to implement these functions

// Windows related variables
// May need to be protected by semaphore

HWND   main_window_handle;
HDC    main_window_hdc;  // May use it to do some unrealized drawing. (like text output)



bool MsgFilter(){
	
}


HWND CreateBlankWindow(){
		
	RegisterClassEx(...);
	
	hWnd = CreateWindows(...);
		
}


static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
    case WM_CREATE:
        // Initialize bitmap and painting thread
		
		NewWnd(HWND);
		
        break;
    case WM_PAINT:
	    PAINTSTRUCT ps;
		HDC hdc;
    
        hdc = BeginPaint(hWnd, &ps);
		
		

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