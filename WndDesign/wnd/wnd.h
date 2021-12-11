#pragma once

#include "../common/core.h"
#include "../common/msg.h"
#include "../style.h"


struct WNDDESIGN_API Wnd{
	static Wnd* Create(Wnd* parent, const WndStyle* style, Handler handler);
	static void Destroy(Wnd** pWnd);


};

