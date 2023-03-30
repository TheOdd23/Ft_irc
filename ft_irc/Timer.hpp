#ifndef TIMER_H
#define TIMER_H

#include <time.h>

class Timer
{
public:
    Timer();
    ~Timer();

    void Start();
    float Timing();
private:
    clock_t _start_time;
};

#endif