#pragma once

namespace primitives {

    template<typename T>
    class Vector2 {
    public:

        Vector2(T x, T y);

        T x() const;
        T y() const;

    protected:
        T x_, y_;
    };

    typedef Vector2<float> Vector2f;

}
