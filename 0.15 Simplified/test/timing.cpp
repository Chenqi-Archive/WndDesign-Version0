#include <Windows.h>
#include "timing.h"

static LARGE_INTEGER frequency = { (long long)10000000 };
static LARGE_INTEGER timeStart;
static LARGE_INTEGER timeEnd;

void init_timing() {
	QueryPerformanceFrequency(&frequency);
}

void start_timing() {
	QueryPerformanceCounter(&timeStart);
}

double end_timing() {
	QueryPerformanceCounter(&timeEnd);
	return (timeEnd.QuadPart - timeStart.QuadPart) / (double)frequency.QuadPart;
}
