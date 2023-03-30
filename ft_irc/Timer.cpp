#include "Timer.hpp"

Timer::Timer() : _start_time(0) {}

Timer::~Timer() {}

void Timer::Start() 
{
    _start_time = clock();
}

float Timer::Timing() 
{
    float time = (clock() - _start_time);
    return static_cast<float>((time / CLOCKS_PER_SEC) * 10000);
}