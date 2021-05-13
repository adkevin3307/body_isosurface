#include "Timer.h"

#if defined(__APPLE__)
#if defined(__x86_64__)
#include <sys/sysctl.h>
#else
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif // __x86_64__ or not

#include <stdio.h> // fprintf
#include <stdlib.h> // exit

#elif _WIN32
#include <windows.h>
#include <time.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#endif

using namespace std;

double Timer::_begin;
double Timer::_end;

unsigned long long Timer::current_ticks()
{
#if defined(__APPLE__) && !defined(__x86_64__)
    return mach_absolute_time();
#elif defined(_WIN32)
    LARGE_INTEGER qwTime;
    QueryPerformanceCounter(&qwTime);
    return qwTime.QuadPart;
#elif defined(__x86_64__)
    unsigned int a, d;
    asm volatile("rdtsc"
                 : "=a"(a), "=d"(d));
    return static_cast<unsigned long long>(a) | (static_cast<unsigned long long>(d) << 32);
#elif defined(__ARM_NEON__) && 0 // mrc requires superuser.
    unsigned int val;
    asm volatile("mrc p15, 0, %0, c9, c13, 0"
                 : "=r"(val));
    return val;
#else
    timespec spec;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &spec);
    return CycleTimer::SysClock(static_cast<float>(spec.tv_sec) * 1e9 + static_cast<float>(spec.tv_nsec));
#endif
}

double Timer::seconds_per_tick()
{
    static bool initialized = false;
    static double secondsPerTick_val;

    if (initialized) return secondsPerTick_val;

#if defined(__APPLE__)
#ifdef __x86_64__
    int args[] = { CTL_HW, HW_CPU_FREQ };
    unsigned int Hz;
    size_t len = sizeof(Hz);

    if (sysctl(args, 2, &Hz, &len, NULL, 0) != 0) {
        fprintf(stderr, "Failed to initialize secondsPerTick_val!\n");
        exit(-1);
    }

    secondsPerTick_val = 1.0 / (double)Hz;
#else
    mach_timebase_info_data_t time_info;
    mach_timebase_info(&time_info);

    // Scales to nanoseconds without 1e-9f
    secondsPerTick_val = (1e-9 * static_cast<double>(time_info.numer)) / static_cast<double>(time_info.denom);
#endif // x86_64 or not
#elif defined(_WIN32)
    LARGE_INTEGER qwTicksPerSec;
    QueryPerformanceFrequency(&qwTicksPerSec);
    secondsPerTick_val = 1.0 / static_cast<double>(qwTicksPerSec.QuadPart);
#else
    FILE* fp = fopen("/proc/cpuinfo", "r");
    char input[1024];

    if (!fp) {
        fprintf(stderr, "CycleTimer::resetScale failed: couldn't find /proc/cpuinfo.");
        exit(-1);
    }
    // In case we don't find it, e.g. on the N900
    secondsPerTick_val = 1e-9;
    while (!feof(fp) && fgets(input, 1024, fp)) {
        // NOTE(boulos): Because reading cpuinfo depends on dynamic
        // frequency scaling it's better to read the @ sign first
        float GHz, MHz;
        if (strstr(input, "model name")) {
            char* at_sign = strstr(input, "@");

            if (at_sign) {
                char* after_at = at_sign + 1;
                char* GHz_str = strstr(after_at, "GHz");
                char* MHz_str = strstr(after_at, "MHz");

                if (GHz_str) {
                    *GHz_str = '\0';
                    if (1 == sscanf(after_at, "%f", &GHz)) {
                        //printf("GHz = %f\n", GHz);
                        secondsPerTick_val = 1e-9f / GHz;
                        break;
                    }
                }
                else if (MHz_str) {
                    *MHz_str = '\0';
                    if (1 == sscanf(after_at, "%f", &MHz)) {
                        //printf("MHz = %f\n", MHz);
                        secondsPerTick_val = 1e-6f / MHz;
                        break;
                    }
                }
            }
        }
        else if (1 == sscanf(input, "cpu MHz : %f", &MHz)) {
            //printf("MHz = %f\n", MHz);
            secondsPerTick_val = 1e-6f / MHz;
            break;
        }
    }

    fclose(fp);
#endif

    initialized = true;
    return secondsPerTick_val;
}

void Timer::begin()
{
    Timer::_begin = Timer::current_ticks() * Timer::seconds_per_tick();
}

void Timer::end()
{
    Timer::_end = Timer::current_ticks() * Timer::seconds_per_tick();
}

double Timer::time()
{
    return Timer::_end - Timer::_begin;
}

double Timer::current_time()
{
    return Timer::current_ticks() * Timer::seconds_per_tick();
}
