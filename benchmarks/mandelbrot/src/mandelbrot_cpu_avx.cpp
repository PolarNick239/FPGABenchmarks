#include "mandelbrot.h"

#include <cassert>
#include <limits>
#include <immintrin.h>

using images::Image;
using primitives::Vector2f;

void MandelbrotProcessorCPU_AVX::process(Vector2f from, Vector2f to,
                                         Image<unsigned short>& iterations) {
    assert (MAX_ITERATIONS < std::numeric_limits<unsigned short>::max());
    assert (iterations.cn == 1);

    assert (((float) iterations.width) + 1 == (float) (iterations.width + 1));
    assert (((float) iterations.height) + 1 == (float) (iterations.height + 1));
    float width = iterations.width;
    float height = iterations.height;

    float x_step = (to.x() - from.x()) / width;
    float y_step = (to.y() - from.y()) / height;

    #pragma omp parallel for
    for (size_t py = 0; py < iterations.height; py++) {
        float y0 = from.y() + y_step * py;

        for (size_t px = 0; px < iterations.width / 8 * 8; px += 8) {
            float pxf = (float) px;
            __m256 pxs_deltas128 = _mm256_mul_ps(_mm256_set_ps(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f), _mm256_set1_ps(x_step));
            __m256 xs0 = _mm256_add_ps(_mm256_set1_ps(from.x()), _mm256_add_ps(_mm256_set1_ps(x_step * pxf), pxs_deltas128));    // from.x() + x_step * px

            unsigned short iteration;
            __m256i maskAll = _mm256_setzero_si256();
            __m256i iters = _mm256_setzero_si256();
            __m256 xs = _mm256_set1_ps(0.0f);
            __m256 ys = _mm256_set1_ps(0.0f);
            for (iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
                __m256 xsn = _mm256_add_ps(_mm256_sub_ps(_mm256_mul_ps(xs, xs), _mm256_mul_ps(ys, ys)), xs0);               // xn = x * x - y * y + x0;
                __m256 ysn = _mm256_add_ps(_mm256_mul_ps(_mm256_mul_ps(_mm256_set1_ps(2.0f), xs), ys), _mm256_set1_ps(y0)); // yn = 2 * x * y + y0;
                xs = _mm256_add_ps(_mm256_andnot_ps((__m256) maskAll, xsn), _mm256_and_ps((__m256) maskAll, xs));
                ys = _mm256_add_ps(_mm256_andnot_ps((__m256) maskAll, ysn), _mm256_and_ps((__m256) maskAll, ys));

                maskAll = (__m256i) _mm256_or_ps(_mm256_cmp_ps(_mm256_add_ps(_mm256_mul_ps(xs, xs), _mm256_mul_ps(ys, ys)), _mm256_set1_ps(INFINITY), _CMP_GT_OS), (__m256) maskAll);
                iters = _mm256_add_epi16(iters, _mm256_andnot_si256(maskAll, _mm256_set1_epi16(1)));
                int mask = _mm256_movemask_epi8(maskAll);
                if (mask == (int) 0xffffffff) {
                    break;
                }
            }
            iters = _mm256_shuffle_epi8(iters, _mm256_setr_epi8(0, 1, -1, -1, 4, 5, -1, -1, 8, 9, -1, -1, 12, 13, -1, -1, 16, 17, -1, -1, 20, 21, -1, -1, 24, 25, -1, -1, 28, 29, -1, -1));
            unsigned int tmp[8];
            _mm256_storeu_si256((__m256i *) tmp, iters);
            for (int i = 0; i < 8; i++) {
                iterations(py, px + 7 - i) = (unsigned short) tmp[i];
            }
        }

        for (size_t px = iterations.width / 8 * 8; px < iterations.width; px++) {
            float x0 = from.x() + (to.x() - from.x()) * px / width;

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
