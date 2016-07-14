#include "mandelbrot.h"

#include <cassert>
#include <limits>

using images::Image;
using primitives::Vector2f;

void MandelbrotProcessorCPU::process(Vector2f from, Vector2f to,
                                     Image<unsigned short>& iterations) {
    assert (MAX_ITERATIONS < std::numeric_limits<unsigned short>::max());
    assert (iterations.cn == 1);

    size_t width = iterations.width;
    size_t height = iterations.height;
    for (size_t py = 0; py < height; py++) {
        for (size_t px = 0; px < width; px++) {
            float x0 = from.x() + (to.x() - from.x()) * px / width;
            float y0 = from.y() + (to.y() - from.y()) * py / height;

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
