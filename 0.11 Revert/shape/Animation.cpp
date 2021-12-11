#include "Animation.h"
#include <Windows.h>

void WndDesign::Animation::TimerCallBack(PVOID para, BOOLEAN timer) {
	Animation* animation = (Animation*)para;
	if (animation->board.GetSize() == Size{0,0}) {  // Uninitialized
		return;
	}
	animation->Paint();
}

WndDesign::Animation::Animation(uint period):period(period), Timer(NULL) {
	CreateTimerQueueTimer(&Timer, NULL, TimerCallBack, this,
						  0, period, WT_EXECUTEDEFAULT);
}

WndDesign::Animation::~Animation() {
	DeleteTimerQueueTimer(NULL, Timer, INVALID_HANDLE_VALUE);
}
