#include "mandelbrot.h"

#include <cpuinfo.h>

#ifdef SPMD_SUPPORT
    #include "gen/mandelbrotProcess_sse.h"
#endif

using images::Image;
using primitives::Vector2f;

bool MandelbrotProcessorISPC_SSE::available() {
#if SPMD_SUPPORT
    return cpuinfo.has_sse() && cpuinfo.has_sse2() && cpuinfo.has_sse3() && cpuinfo.has_ssse3() && cpuinfo.has_sse4_1() && cpuinfo.has_sse4_2();
#else
    return false;
#endif
}

bool MandelbrotProcessorISPC_SSE::isAvailable() {
    return MandelbrotProcessorISPC_SSE::available();
}

void MandelbrotProcessorISPC_SSE::process(Vector2f from, Vector2f to,
                                         Image<unsigned short>& iterations) {
#if SPMD_SUPPORT
    ispc::mandelbrotProcess_sse(iterations.ptr(), (int32_t) iterations.width, (int32_t) iterations.height,
                                 from.x(), from.y(), to.x(), to.y(),
                                 MAX_ITERATIONS, INFINITY);
#else
    throw std::runtime_error("SPMD unsupported!");
#endif
}
