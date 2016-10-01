#pragma once

#include "images.h"
#include "vector2.h"
#include "cl/Engine.h"

class MandelbrotProcessor {
public:
    static const int MAX_ITERATIONS = 1000;

    virtual void process(primitives::Vector2f from, primitives::Vector2f to,
                         images::Image<unsigned short>& iterations) = 0;

    virtual bool isAvailable() = 0;

protected:
    const float INFINITY = 2.0f;
};

class MandelbrotProcessorCPU : public MandelbrotProcessor {
public:

    virtual void process(primitives::Vector2f from, primitives::Vector2f to,
                         images::Image<unsigned short>& iterations) override;
    virtual bool isAvailable() override;

};

class MandelbrotProcessorCPU_SSE : public MandelbrotProcessor {
public:

    virtual void process(primitives::Vector2f from, primitives::Vector2f to,
                         images::Image<unsigned short>& iterations) override;
    virtual bool isAvailable() override;
    static bool available();

};

class MandelbrotProcessorCPU_AVX : public MandelbrotProcessor {
public:

    virtual void process(primitives::Vector2f from, primitives::Vector2f to,
                         images::Image<unsigned short>& iterations) override;
    virtual bool isAvailable() override;
    static bool available();

};

class MandelbrotProcessorSingleThreaded : public MandelbrotProcessor {
public:

    MandelbrotProcessorSingleThreaded(std::shared_ptr<MandelbrotProcessor> processor);

    virtual void process(primitives::Vector2f from, primitives::Vector2f to,
                         images::Image<unsigned short>& iterations) override;
    virtual bool isAvailable() override;

protected:

    std::shared_ptr<MandelbrotProcessor> processor;

};

class MandelbrotProcessorOCL : public MandelbrotProcessor {
public:

    static bool available();

    MandelbrotProcessorOCL(const cl::Engine_ptr& engine);
    ~MandelbrotProcessorOCL();

    bool init();
    virtual bool isAvailable() override;
    virtual void process(primitives::Vector2f from, primitives::Vector2f to,
                         images::Image<unsigned short>& iterations) override;

protected:
    const cl::Engine_ptr engine;
    cl::Kernel_ptr kernel;

    cl_mem  buffer;
    size_t  buffer_width;
    size_t  buffer_height;

    bool allocate(size_t width, size_t height);
    bool deallocate();

};
