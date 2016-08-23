#include "gaussian.h"

#include "omp_commons.h"

template<typename T>
GaussianProcessorSingleThreaded<T>::GaussianProcessorSingleThreaded(std::shared_ptr<GaussianProcessor<T>> processor) : processor(processor) {}

template<typename T>
bool GaussianProcessorSingleThreaded<T>::isAvailable() {
    return processor->isAvailable();
}

template<typename T>
void GaussianProcessorSingleThreaded<T>::processHorizontal(images::Image<T> &src, images::Image<T> &dst, int r) {
    auto foo = std::bind(&GaussianProcessor<T>::processHorizontal, processor.get(), src, dst, r);
    commons::singleThreaded<void>(foo);
}

template class GaussianProcessorSingleThreaded<unsigned char>;
template class GaussianProcessorSingleThreaded<float>;
