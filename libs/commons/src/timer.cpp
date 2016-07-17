#include "timer.h"

using commons::Timer;
using std::chrono::time_point;
using std::chrono::high_resolution_clock;
using std::chrono::time_point_cast;

Timer::Timer(bool isPaused) {
    if (!isPaused) {
        start();
    }
}

void Timer::start() {
    is_paused = false;
    start_time = now();
}

void commons::Timer::pause() {
    elapsed_time += now() - start_time;
    is_paused = true;
}

template <typename TIME_TICK>
size_t Timer::elapsed() {
    if (is_paused) {
        return time_point_cast<TIME_TICK>(elapsed_time).time_since_epoch().count();
    } else {
        return time_point_cast<TIME_TICK>(elapsed_time + (now() - start_time)).time_since_epoch().count();
    }
}

template size_t Timer::elapsed<milliseconds>();
template size_t Timer::elapsed<seconds>();

std::chrono::time_point<std::chrono::high_resolution_clock> Timer::now() {
    return high_resolution_clock::now();
}









