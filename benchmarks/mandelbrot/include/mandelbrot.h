#pragma once

#include "images.h"
#include "vector2.h"

class MandelbrotProcessor {
public:
    static const int MAX_ITERATIONS = 1000;

    virtual void process(primitives::Vector2f from, primitives::Vector2f to,
                         images::Image<unsigned short>& iterations) = 0;

protected:
    const float INFINITY = 1000;
};

class MandelbrotProcessorCPU : public MandelbrotProcessor {
public:

    virtual void process(primitives::Vector2f from, primitives::Vector2f to,
                         images::Image<unsigned short>& iterations) override;

};

class MandelbrotProcessorCPU_SSE : public MandelbrotProcessor {
public:

    virtual void process(primitives::Vector2f from, primitives::Vector2f to,
                         images::Image<unsigned short>& iterations) override;

};

class MandelbrotProcessorSingleThreaded : public MandelbrotProcessor {
public:

    MandelbrotProcessorSingleThreaded(MandelbrotProcessor& processor);

    virtual void process(primitives::Vector2f from, primitives::Vector2f to,
                         images::Image<unsigned short>& iterations) override;

protected:

    MandelbrotProcessor& processor;

};
