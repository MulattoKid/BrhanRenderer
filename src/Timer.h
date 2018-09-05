#ifndef TIMER_H
#define TIMER_H

#include <chrono>

struct ElapsedTime
{
	unsigned int ms;
	unsigned int s;
	unsigned int m;
};

std::chrono::high_resolution_clock::time_point GetTime();
void LogElapsedTime(const char* msg, std::chrono::high_resolution_clock::time_point start, std::chrono::high_resolution_clock::time_point end);

#endif
