#include "Cursor.h"

#include <Windows.h>

bool WndDesign::ChangeCursor(CursorStyle style) {
    switch (style) {
        case WndDesign::CursorStyle::Arrow:     SetCursor(LoadCursorW(NULL, IDC_ARROW)); break;
        case WndDesign::CursorStyle::Hand:      SetCursor(LoadCursorW(NULL, IDC_HAND)); break;
        case WndDesign::CursorStyle::Edit:      SetCursor(LoadCursorW(NULL, IDC_IBEAM)); break;
        case WndDesign::CursorStyle::Move:      SetCursor(LoadCursorW(NULL, IDC_SIZEALL)); break;
        case WndDesign::CursorStyle::Size0:     SetCursor(LoadCursorW(NULL, IDC_SIZEWE)); break;
        case WndDesign::CursorStyle::Size45:    SetCursor(LoadCursorW(NULL, IDC_SIZENESW)); break;
        case WndDesign::CursorStyle::Size90:    SetCursor(LoadCursorW(NULL, IDC_SIZENS)); break;
        case WndDesign::CursorStyle::Size135:   SetCursor(LoadCursorW(NULL, IDC_SIZENWSE)); break;
        default:return false;
    }
    return true;
}
