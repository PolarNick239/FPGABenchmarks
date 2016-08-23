#include "mandelbrot.h"

#include "omp_commons.h"

MandelbrotProcessorSingleThreaded::MandelbrotProcessorSingleThreaded(std::shared_ptr<MandelbrotProcessor> processor) : processor(processor) {}

bool MandelbrotProcessorSingleThreaded::isAvailable() {
    return processor->isAvailable();
}

void MandelbrotProcessorSingleThreaded::process(primitives::Vector2f from, primitives::Vector2f to,
                                                images::Image<unsigned short> &iterations) {
    auto foo = std::bind(&MandelbrotProcessor::process, processor.get(), from, to, iterations);
    commons::singleThreaded<void>(foo);
}
