#ifndef TIME_H_
#define TIME_H_
#include <sys/time.h>

inline long getTimeInMS() {
    struct timeval time_value;
    gettimeofday(&time_value, nullptr);
    return time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
}

#endif // TIME_H_
