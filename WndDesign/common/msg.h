#pragma once

#include "core.h"

BEGIN_NAMESPACE(WndDesign)

enum class Msg {
	Create,
	Destroy,

	LeftDown,
	LeftUp,
	RightDown,
	RightUp,

	Hover,

	KeyDown,
};

enum class SysMsg {
	Resize,
	Paint,

};


END_NAMESPACE(WndDesign)