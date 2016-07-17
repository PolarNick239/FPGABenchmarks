#include "mandelbrot.h"

#include "omp_commons.h"

MandelbrotProcessorSingleThreaded::MandelbrotProcessorSingleThreaded(MandelbrotProcessor& processor) : processor(processor) {}

void MandelbrotProcessorSingleThreaded::process(primitives::Vector2f from, primitives::Vector2f to,
                                                images::Image<unsigned short> &iterations) {
    auto tmp = std::bind(&MandelbrotProcessor::process, &processor, from, to, iterations);
    commons::singleThreaded<void>(tmp);
}
