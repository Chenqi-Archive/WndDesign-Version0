#include "String.h"

// Only support wchar_t(Unicode)
class TextBox : public Wnd{
	
    String str;
	
	// Font settings. Could support latex.
    FontType font;
	Size size;
	
	// Style: editable or uneditable (dynamic or static)
	Sytle style;
	
	
	// current cursor pos
	uint cursorPos;
	
	// Add blinking cursor pos 
	
	
	
	
}