#include <omp.h>
#include <vector>
#include <iostream>

#include "gaussian.h"
#include "timer.h"

using std::pair;
using std::string;
using std::to_string;
using std::vector;
using std::shared_ptr;
using images::Image;
using commons::Timer;

int main() {
    int iters_number = 3;
    size_t width = 2048;
    size_t height = 2048;
    double sigma = 100;

    Image<unsigned char> image(width, height, 3);

    for (size_t y = 0; y < image.height; ++y) {
        for (size_t x = 0; x < image.width; ++x) {
            for (size_t c = 0; c < image.cn; ++c) {
                image(y, x, c) = (unsigned char) (x * 23 + y * 19 + c * 17);
            }
        }
    }

    string threads_suffix = string("") + to_string(omp_get_max_threads()) + " threads\t\t";
    string single_thread_suffix("1 thread\t");

    vector<pair<string, shared_ptr<GaussianProcessor<unsigned char>>>> processors;

    shared_ptr<GaussianProcessor<unsigned char>> cpu(new GaussianProcessorCPU<unsigned char>());
    shared_ptr<GaussianProcessor<unsigned char>> cpu_single(new GaussianProcessorSingleThreaded<unsigned char>(cpu));

    processors.push_back(pair<string, shared_ptr<GaussianProcessor<unsigned char>>>(string("CPU     \t") + threads_suffix, cpu));
    processors.push_back(pair<string, shared_ptr<GaussianProcessor<unsigned char>>>(string("CPU     \t") + single_thread_suffix, cpu_single));
    processors.push_back(pair<string, shared_ptr<GaussianProcessor<unsigned char>>>(string("CPU     \t") + threads_suffix, cpu));

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

            processor->process(image, sigma);

            best_result = std::min(best_result, timer.elapsed<milliseconds>());
        }
        std::cout << best_result << " ms\t" << label << std::endl;
    }
    return 0;
}
