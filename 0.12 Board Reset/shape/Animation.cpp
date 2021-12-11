#include "Animation.h"
#include <Windows.h>

void WndDesign::Animation::TimerCallBack(PVOID para, BOOLEAN timer) {
	Animation* animation = (Animation*)para;

	// Caution! Asynchoronous function call! May cause race problems!

	animation->Paint();
	animation->Next();
}

WndDesign::Animation::Animation(uint period):period(period), Timer(NULL){
	Init();
}

void WndDesign::Animation::Play() {
	if (Timer != NULL) { return; }  // Already playing.
	CreateTimerQueueTimer(&Timer, NULL, TimerCallBack, this,
						  0, period, WT_EXECUTEDEFAULT);
}

void WndDesign::Animation::Pause() {
	DeleteTimerQueueTimer(NULL, Timer, INVALID_HANDLE_VALUE);
	Timer = NULL;
}

void WndDesign::Animation::Stop() {
	DeleteTimerQueueTimer(NULL, Timer, INVALID_HANDLE_VALUE);
	Timer = NULL;
	Init();
}

WndDesign::Animation::~Animation() {
	DeleteTimerQueueTimer(NULL, Timer, INVALID_HANDLE_VALUE);
}
