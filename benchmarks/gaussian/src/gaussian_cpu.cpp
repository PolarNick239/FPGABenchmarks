#include "gaussian.h"

#include <cassert>
#include "type_helpers.h"

template<typename T>
bool GaussianProcessorCPU<T>::isAvailable() {
    return true;
}

template<typename T>
void GaussianProcessorCPU<T>::processHorizontal(images::Image<T> &src, images::Image<T> &dst, int r) {
    assert (src.width == dst.height && src.height == dst.width && src.cn == dst.cn);
    size_t cn = src.cn;

    #pragma omp parallel for
    for (size_t j = 0; j < src.height; ++j) {
        int boxWidth = 0;
        assert (AccumulatingType<T>::capacityLimit >= cn);
        std::vector<typename AccumulatingType<T>::type> sums(cn);

        for (ptrdiff_t i = -r; i < (ptrdiff_t) src.width; ++i) {
            if (i >= r) {
                --boxWidth;
                for (size_t c = 0; c < 3; c++) {
                    sums[c] -= src(j, (size_t) (i - r), c);
                }
            }
            if ((size_t) (i + r) < src.width) {
                ++boxWidth;
                for (size_t c = 0; c < 3; c++) {
                    sums[c] += src(j, (size_t) (i + r), c);
                }
            }

            if (i >= 0) {
                for (size_t c = 0; c < 3; c++) {
                    dst((size_t) i, j, c) = sums[c] / boxWidth;
                }
            }
        }
    }
}

template class GaussianProcessorCPU<unsigned char>;
template class GaussianProcessorCPU<float>;
