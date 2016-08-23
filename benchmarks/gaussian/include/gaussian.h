#pragma once

#include <vector>

#include "images.h"
#include "vector2.h"

template<typename T>
class GaussianProcessor {
public:
    static const int ITERATIONS = 3;
    static std::vector<int> boxesRadiuses(double sigma, int n = ITERATIONS);

    virtual bool isAvailable() = 0;

    void process(images::Image<T>& image, double sigma);

    virtual void processHorizontal(images::Image<T>& src, images::Image<T>& dst, int r) = 0;
};

template<typename T>
class GaussianProcessorCPU : public GaussianProcessor<T> {
public:

    virtual bool isAvailable() override;

protected:
    virtual void processHorizontal(images::Image<T> &src, images::Image<T> &dst, int r) final override;
};

template<typename T>
class GaussianProcessorSingleThreaded : public GaussianProcessor<T> {
public:

    GaussianProcessorSingleThreaded(std::shared_ptr<GaussianProcessor<T>> processor);

    virtual bool isAvailable() override;

protected:
    virtual void processHorizontal(images::Image<T> &src, images::Image<T> &dst, int r) final override;

    std::shared_ptr<GaussianProcessor<T>> processor;

};
