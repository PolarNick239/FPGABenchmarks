#include "gaussian.h"

#include <cmath>

template<typename T>
std::vector<int> GaussianProcessor<T>::boxesRadiuses(double sigma, int n) {
    double wIdeal = std::sqrt((12 * sigma * sigma / n) + 1);
    int wl = (int) floor(wIdeal);
    if (wl % 2 == 0) {
        wl--;
    }
    int wu = wl + 2;

    double mIdeal = (12 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) / (-4 * wl - 4);
    int m = (int) round(mIdeal);

    std::vector<int> sizes;
    for (int i = 0; i < n; i++) {
        sizes.push_back(i < m ? wl : wu);
    }
    return sizes;
}

template<typename T>
void GaussianProcessor<T>::process(images::Image<T>& image, double sigma) {
    images::Image<T> tmp(image.height, image.width, image.cn);
    std::vector<int> radiuses = boxesRadiuses(sigma, ITERATIONS);

    for (int i = 0; i < ITERATIONS; ++i) {
        processHorizontal(image, tmp, radiuses[i]);
        processHorizontal(tmp, image, radiuses[i]);
    }
}

template class GaussianProcessor<unsigned char>;
template class GaussianProcessor<float>;
