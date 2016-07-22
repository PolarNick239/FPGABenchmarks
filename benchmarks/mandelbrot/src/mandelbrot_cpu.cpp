#include "mandelbrot.h"

#include <cassert>
#include <limits>

using images::Image;
using primitives::Vector2f;

bool MandelbrotProcessorCPU::isAvailable() {
    return true;
}

void MandelbrotProcessorCPU::process(Vector2f from, Vector2f to,
                                     Image<unsigned short>& iterations) {
    assert (MAX_ITERATIONS < std::numeric_limits<unsigned short>::max());
    assert (iterations.cn == 1);

    size_t width = iterations.width;
    size_t height = iterations.height;

    float x_step = (to.x() - from.x()) / width;
    float y_step = (to.y() - from.y()) / height;

    #pragma omp parallel for
    for (size_t py = 0; py < height; py++) {
        float y0 = from.y() + y_step * py;

        for (size_t px = 0; px < width; px++) {
            float x0 = from.x() + x_step * px;

            unsigned short iteration;
            float x = 0.0f;
            float y = 0.0f;
            for (iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
                float xn = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xn;
                if (x * x + y * y > INFINITY) {
                    break;
                }
            }
            iterations(py, px) = iteration;
        }
    }
}
