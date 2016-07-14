#include <vector2.h>

using primitives::Vector2;

template <typename T>
Vector2<T>::Vector2(T x, T y) : x_(x), y_(y) {}

template <typename T>
T Vector2<T>::x() const {
    return x_;
}

template <typename T>
T Vector2<T>::y() const {
    return y_;
}


template class Vector2<float>;
