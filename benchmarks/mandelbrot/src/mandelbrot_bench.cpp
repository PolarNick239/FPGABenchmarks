#include <omp.h>
#include <vector>
#include <iostream>

#include "mandelbrot.h"
#include "timer.h"

using std::pair;
using std::string;
using std::to_string;
using std::vector;
using std::shared_ptr;
using images::Image;
using commons::Timer;
using primitives::Vector2f;


int main() {
    int iters_number = 3;
    size_t width = 1024;
    size_t height = 1024;

    Image<unsigned short> iterations(width, height, 1);

    string threads_suffix = string("") + to_string(omp_get_max_threads()) + " threads\t\t";
    string single_thread_suffix("1 thread\t");

    vector<pair<string, shared_ptr<MandelbrotProcessor>>> processors;

    shared_ptr<MandelbrotProcessor> cpu_avx(new MandelbrotProcessorCPU_AVX());
    shared_ptr<MandelbrotProcessor> cpu_avx_single(new MandelbrotProcessorSingleThreaded(cpu_avx));
    shared_ptr<MandelbrotProcessor> cpu_sse(new MandelbrotProcessorCPU_SSE());
    shared_ptr<MandelbrotProcessor> cpu_sse_single(new MandelbrotProcessorSingleThreaded(cpu_sse));
    shared_ptr<MandelbrotProcessor> cpu(new MandelbrotProcessorCPU());
    shared_ptr<MandelbrotProcessor> cpu_single(new MandelbrotProcessorSingleThreaded(cpu));

    shared_ptr<MandelbrotProcessor> cpu_ispc_sse(new MandelbrotProcessorISPC_SSE());
    shared_ptr<MandelbrotProcessor> cpu_ispc_avx(new MandelbrotProcessorISPC_AVX());

    auto platforms = cl::getPlatforms();
    for (auto platform : platforms) {
        auto devices = cl::getDevices(platform);
        for (auto device : devices) {
            try {
                cl::Engine_ptr engine(new cl::Engine(device));
                engine->init();
                shared_ptr<MandelbrotProcessorOCL> ocl(new MandelbrotProcessorOCL(engine));
                ocl->init();
                processors.push_back(pair<string, shared_ptr<MandelbrotProcessor>>(string("OpenCL  \t") + device->name, ocl));
            } catch (std::exception& e) {
                std::cout << "OpenCL  \t" << "initialization failed for device " << device->name << std::endl;
            }
        }
    }

    processors.push_back(pair<string, shared_ptr<MandelbrotProcessor>>(string("CPU AVX \t") + threads_suffix, cpu_avx));
    processors.push_back(pair<string, shared_ptr<MandelbrotProcessor>>(string("CPU AVX \t") + single_thread_suffix, cpu_avx_single));
    processors.push_back(pair<string, shared_ptr<MandelbrotProcessor>>(string("CPU SSE \t") + threads_suffix, cpu_sse));
    processors.push_back(pair<string, shared_ptr<MandelbrotProcessor>>(string("CPU SSE \t") + single_thread_suffix, cpu_sse_single));
    processors.push_back(pair<string, shared_ptr<MandelbrotProcessor>>(string("CPU     \t") + threads_suffix, cpu));
    processors.push_back(pair<string, shared_ptr<MandelbrotProcessor>>(string("CPU     \t") + single_thread_suffix, cpu_single));

    processors.push_back(pair<string, shared_ptr<MandelbrotProcessor>>(string("ISPC SSE\t") + single_thread_suffix, cpu_ispc_sse));
    processors.push_back(pair<string, shared_ptr<MandelbrotProcessor>>(string("ISPC AVX\t") + single_thread_suffix, cpu_ispc_avx));

    for (size_t i = 0; i < processors.size(); i++) {
        string label = processors[i].first;
        auto processor = processors[i].second;

        if (!processor->isAvailable()) {
            std::cout << "Not available: " << label << std::endl;
            continue;
        }

        size_t best_result = std::numeric_limits<size_t>::max();
        for (int j = 0; j < iters_number; j++) {
            Timer timer;

            processor->process(Vector2f(-1.0f, -1.0f), Vector2f(1.0f, 1.0f), iterations);

            best_result = std::min(best_result, timer.elapsed<milliseconds>());
        }
        std::cout << best_result << " ms\t" << label << std::endl;
    }
    return 0;
}
