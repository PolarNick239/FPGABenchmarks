#include "mandelbrot.h"

#include <cassert>
#include <limits>
#include <xmmintrin.h>
#include <tmmintrin.h>

using images::Image;
using primitives::Vector2f;

void MandelbrotProcessorCPU_SSE::process(Vector2f from, Vector2f to,
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

        for (size_t px = 0; px < iterations.width / 4 * 4; px += 4) {
            float pxf = (float) px;
            __m128 pxs_deltas128 = _mm_mul_ps(_mm_set_ps(0.0f, 1.0f, 2.0f, 3.0f), _mm_set1_ps(x_step));
            __m128 xs0 = _mm_add_ps(_mm_set1_ps(from.x()), _mm_add_ps(_mm_set1_ps(x_step * pxf), pxs_deltas128));    // from.x() + x_step * px

            unsigned short iteration;
            __m128i maskAll = _mm_setzero_si128();
            __m128i iters = _mm_setzero_si128();
            __m128 xs = _mm_set1_ps(0.0f);
            __m128 ys = _mm_set1_ps(0.0f);
            for (iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
                __m128 xsn = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(xs, xs), _mm_mul_ps(ys, ys)), xs0);               // xn = x * x - y * y + x0;
                __m128 ysn = _mm_add_ps(_mm_mul_ps(_mm_mul_ps(_mm_set1_ps(2.0f), xs), ys), _mm_set1_ps(y0));    // yn = 2 * x * y + y0;
                xs = _mm_add_ps(_mm_andnot_ps((__m128) maskAll, xsn), _mm_and_ps((__m128) maskAll, xs));
                ys = _mm_add_ps(_mm_andnot_ps((__m128) maskAll, ysn), _mm_and_ps((__m128) maskAll, ys));

                maskAll = _mm_or_si128(_mm_castps_si128(_mm_cmpge_ps(_mm_add_ps(_mm_mul_ps(xs, xs), _mm_mul_ps(ys, ys)), _mm_set1_ps(INFINITY))), maskAll);
                iters = _mm_add_epi16(iters, _mm_andnot_si128(maskAll, _mm_set1_epi16(1)));
                int mask = _mm_movemask_epi8(maskAll);
                if (mask == 0xffff) {
                    break;
                }
            }
            iters = _mm_shuffle_epi8(iters, _mm_setr_epi8(12, 13, 8, 9, 4, 5, 0, 1, -1, -1, -1, -1, -1, -1, -1, -1));
            _mm_storel_epi64((__m128i *) &iterations(py, px), iters);
        }

        for (size_t px = iterations.width / 4 * 4; px < iterations.width; px++) {
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
