#pragma once

#include "../basic/Figure.h"

namespace WndDesign {

#ifndef CALLBACK
#define CALLBACK    __stdcall
#endif
typedef void* PVOID;
typedef void* HANDLE;
typedef unsigned char BOOLEAN;

class Animation:public Figure {

private:
	static void CALLBACK TimerCallBack(PVOID para, BOOLEAN timer);
private:
	uint period; // ms
	HANDLE Timer;
public:
	Animation(uint period = 40);
	~Animation();
	//void Play() {
	//	ChangeTimerQueueTimer(NULL, Timer, 0, period);
	//}
	//void Pause() {
	//	ChangeTimerQueueTimer(NULL, Timer, 0, 0);
	//}
	//void Stop() {
	//	DeleteTimerQueueTimer(NULL, Timer, NULL);
	//	Timer = NULL;
	//}
};

} // namespace WndDesign