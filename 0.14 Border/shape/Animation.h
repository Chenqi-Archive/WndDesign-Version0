#pragma once

#include "../Figure.h"

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
	uint period;
	HANDLE Timer;
private:
	virtual void Init() = 0;  // Reset to initial state.
	virtual void Next() = 0;  // Go to next state.
public:
	// Will not automatically play. You should call Play() to start playing.
	// period: 1000 / fps. (40 by default)
	Animation(uint period = 40);
	void Play();	// Start playing.
	void Pause();   // Pause, resume by calling Play() again.
	void Stop();    // Stop playing and reset to initial state.
	// May support changing rate later.
	~Animation();
};

} // namespace WndDesign