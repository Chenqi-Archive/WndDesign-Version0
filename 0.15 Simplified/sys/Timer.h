#pragma once

#include "../basic/type.h"

namespace WndDesign {

typedef void (__stdcall *TimerCallBack)(void* para);

class Timer {
	typedef void* HANDLE;
private:
	HANDLE timer;
public:
	Timer():timer(nullptr){}
	bool Start(TimerCallBack callback, void* para, uint period, uint delay = 0);
	bool Stop();
	~Timer() { Stop(); }
};

} // namespace WndDesign