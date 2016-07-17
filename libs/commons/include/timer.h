#pragma once

#include <chrono>

using std::chrono::milliseconds;
using std::chrono::seconds;

namespace commons {
    class Timer {
    public:
        Timer(bool isPaused=false);

        void start();
        void pause();
        template <typename TIME_TICK>
        size_t elapsed();

    protected:
        std::chrono::time_point<std::chrono::high_resolution_clock> elapsed_time;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
        bool                                                        is_paused;

        std::chrono::time_point<std::chrono::high_resolution_clock> now();
    };
}
