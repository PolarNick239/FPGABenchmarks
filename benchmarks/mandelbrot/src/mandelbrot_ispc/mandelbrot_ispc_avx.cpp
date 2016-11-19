#include "mandelbrot.h"

#include <cpuinfo.h>

#ifdef SPMD_SUPPORT
    #include "gen/mandelbrotProcess_avx.h"
#endif

using images::Image;
using primitives::Vector2f;

bool MandelbrotProcessorISPC_AVX::available() {
#if SPMD_SUPPORT
    return cpuinfo.has_avx() && cpuinfo.has_avx2();
#else
    return false;
#endif
}

bool MandelbrotProcessorISPC_AVX::isAvailable() {
    return MandelbrotProcessorISPC_AVX::available();
}

void MandelbrotProcessorISPC_AVX::process(Vector2f from, Vector2f to,
                                         Image<unsigned short>& iterations) {
#if SPMD_SUPPORT
    ispc::mandelbrotProcess_avx(iterations.ptr(), (int32_t) iterations.width, (int32_t) iterations.height,
                                 from.x(), from.y(), to.x(), to.y(),
                                 MAX_ITERATIONS, INFINITY);
#else
    throw std::runtime_error("SPMD unsupported!");
#endif
}
