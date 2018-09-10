#include "Logger.h"
#include "Timer.h"

std::chrono::high_resolution_clock::time_point GetTime()
{
	return std::chrono::high_resolution_clock::now();
}

void LogElapsedTime(const char* msg, std::chrono::high_resolution_clock::time_point start, std::chrono::high_resolution_clock::time_point end)
{
	ElapsedTime et;
	unsigned int ms = (unsigned int)(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());;
	unsigned int s = ms / 1000;
	unsigned int m = s / 60;
	
	et.m = m;
	et.s = s - (et.m * 60);
	et.ms = ms - (et.s * 1000);
	
	LOG_MESSAGE(true, "\n%s%lum %lus %lums\n", msg, et.m, et.s, et.ms);
}
