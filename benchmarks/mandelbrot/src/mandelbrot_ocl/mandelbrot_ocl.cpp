#include "mandelbrot.h"

#include "mandelbrot_cl.h"

#include <cassert>
#include <limits>

using images::Image;
using primitives::Vector2f;

bool MandelbrotProcessorOCL::available() {
    try {
        size_t availableDevices = 0;
        auto platforms = cl::getPlatforms();
        for (auto platform : platforms) {
            auto devices = cl::getDevices(platform);
            availableDevices += devices.size();
        }
        return availableDevices > 0;
    } catch (std::exception& e) {
        return false;
    }
}

MandelbrotProcessorOCL::MandelbrotProcessorOCL(const cl::Engine_ptr& engine) :
        engine(engine), buffer(NULL), buffer_width(0), buffer_height(0) {
}

MandelbrotProcessorOCL::~MandelbrotProcessorOCL() {
    deallocate();
}

bool MandelbrotProcessorOCL::init() {
    assert (engine->ready());
    std::string params = std::string("-D MAX_ITERATIONS=") + std::to_string(MAX_ITERATIONS)
                         + " -D INFINITY_THRESHOLD=" + std::to_string(INFINITY) + "f";
    kernel = engine->compileKernel(mandelbrot_kernel, mandelbrot_kernel_length, "mandelbrotProcess", params.data());
    return kernel != nullptr;
}

bool MandelbrotProcessorOCL::isAvailable() {
    return kernel != nullptr;
}

void MandelbrotProcessorOCL::process(Vector2f from, Vector2f to,
                                     Image<unsigned short>& iterations) {
    assert (kernel);
    if (!allocate(iterations.width, iterations.height)) {
        throw "OpenCL memory allocation failed!";
    }

    int width = (int) iterations.width;
    int height = (int) iterations.height;
    float fromX = from.x();
    float fromY = from.y();
    float toX = to.x();
    float toY = to.y();

    unsigned int i = 0;
    kernel->setArg(i++, sizeof(cl_mem), &buffer);
    kernel->setArg(i++, sizeof(int), &width);
    kernel->setArg(i++, sizeof(int), &height);
    kernel->setArg(i++, sizeof(float), &fromX);
    kernel->setArg(i++, sizeof(float), &fromY);
    kernel->setArg(i++, sizeof(float), &toX);
    kernel->setArg(i++, sizeof(float), &toY);

    size_t localWorkSize[] = {16, 16};
    size_t globalWorkSize[] = {(size_t) width, (size_t) height};
    for (int d = 0; d < 2; d++) {
        globalWorkSize[d] = (globalWorkSize[d] + localWorkSize[d] - 1) / localWorkSize[d] * localWorkSize[d];
    }

    engine->enqueueKernel(kernel, 2, globalWorkSize, localWorkSize);
    engine->readBuffer(buffer, width * height * sizeof(cl_ushort), iterations.ptr());
}

bool MandelbrotProcessorOCL::allocate(size_t width, size_t height) {
    if (width == buffer_width && height == buffer_height) {
        return true;
    }
    if (!deallocate()) {
        return false;
    }

    cl_mem new_buffer = engine->createBuffer(width * height * sizeof(cl_ushort), CL_MEM_WRITE_ONLY);

    buffer = new_buffer;
    buffer_width = width;
    buffer_height = height;
    return true;
}

bool MandelbrotProcessorOCL::deallocate() {
    if (buffer != NULL) {
        engine->deallocateBuffer(buffer);
        buffer = NULL;
        buffer_width = 0;
        buffer_height = 0;
    }
    return true;
}
