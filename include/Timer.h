#pragma once

using namespace std;

class Timer {
private:
    static double _begin, _end;
    static unsigned long long current_ticks();
    static double seconds_per_tick();

public:
    static void begin();
    static void end();
    static double time();
    static double current_time();
};