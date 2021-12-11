#pragma once

namespace WndDesign {

enum class CursorStyle {
	Arrow,			// IDC_ARROW
	Hand,			// IDC_HAND
	Edit,		    // IDC_IBEAM
	Move,			// IDC_SIZEALL
	Size0,		    // IDC_SIZEWE
	Size45,		    // IDC_SIZENESW
	Size90,		    // IDC_SIZENS
	Size135,	    // IDC_SIZENWSE
};

bool ChangeCursor(CursorStyle style);


Pos GetCursorPos();


} // namespace WndDesign