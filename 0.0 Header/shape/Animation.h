#pragma once

#include "../basic/Figure.h"
#include "../sys/Timer.h"

namespace WndDesign {

class Animation :public Figure {
private:
	Timer timer;
	static inline void __stdcall TimerProc(void* para) {
		Animation* animation = (Animation*)para;
		animation->Paint();
		animation->Next();
	}
private:
	uint period;
public:
	Animation(uint period = 40):period(period){}
	virtual void Init() = 0;
	virtual void Next() = 0;
	inline void Play() {
		Init();
		timer.Start(TimerProc, this, period);
	}
	inline void Stop() {
		timer.Stop();
	}
};



} // namespace WndDesign