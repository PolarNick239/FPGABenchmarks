#include <omp.h>
#include <vector>
#include <iostream>

#include "mandelbrot.h"
#include "timer.h"

using std::pair;
using std::string;
using std::to_string;
using std::vector;
using images::Image;
using commons::Timer;
using primitives::Vector2f;


int main() {
    int iters_number = 3;
    size_t width = 1024;
    size_t height = 1024;

    Image<unsigned short> iterations(width, height, 1);

    string threads_suffix = string(" (") + to_string(omp_get_max_threads()) + " threads)\t\t";
    string single_thread_suffix(" (single thread)\t");

    vector<pair<string, MandelbrotProcessor&>> processors;

    MandelbrotProcessorCPU_AVX cpu_avx;
    MandelbrotProcessorSingleThreaded cpu_avx_single(cpu_avx);
    MandelbrotProcessorCPU_SSE cpu_sse;
    MandelbrotProcessorSingleThreaded cpu_sse_single(cpu_sse);
    MandelbrotProcessorCPU cpu;
    MandelbrotProcessorSingleThreaded cpu_single(cpu);

    processors.push_back(pair<string, MandelbrotProcessor&>(string("CPU AVX2\t") + threads_suffix, cpu_avx));
    processors.push_back(pair<string, MandelbrotProcessor&>(string("CPU AVX2\t") + single_thread_suffix, cpu_avx_single));
    processors.push_back(pair<string, MandelbrotProcessor&>(string("CPU SSE \t") + threads_suffix, cpu_sse));
    processors.push_back(pair<string, MandelbrotProcessor&>(string("CPU SSE \t") + single_thread_suffix, cpu_sse_single));
    processors.push_back(pair<string, MandelbrotProcessor&>(string("CPU     \t") + threads_suffix, cpu));
    processors.push_back(pair<string, MandelbrotProcessor&>(string("CPU     \t") + single_thread_suffix, cpu_single));

    for (size_t i = 0; i < processors.size(); i++) {
        string label = processors[i].first;
        MandelbrotProcessor& processor = processors[i].second;
        std::cout << label << "\t";

        if (!processor.isAvailable()) {
            std::cout << "not available" << std::endl;
            continue;
        }

        size_t best_result = std::numeric_limits<size_t>::max();
        for (int j = 0; j < iters_number; j++) {
            Timer timer;

            processor.process(Vector2f(-1.0f, -1.0f), Vector2f(1.0f, 1.0f), iterations);

            best_result = std::min(best_result, timer.elapsed<milliseconds>());
        }
        std::cout << best_result << " ms" << std::endl;
    }
    return 0;
}
