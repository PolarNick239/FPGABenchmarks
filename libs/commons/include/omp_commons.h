#include <omp.h>

#include <functional>


namespace commons {

    template <typename T>
    T singleThreaded(std::function<T()> foo);

    template <>
    void singleThreaded(std::function<void()> foo) {
        int max_threads = omp_get_max_threads();

        omp_set_num_threads(1);
        foo();
        omp_set_num_threads(max_threads);
    }

    template <typename T>
    T singleThreaded(std::function<T()> foo) {
        int max_threads = omp_get_max_threads();

        omp_set_num_threads(1);
        T result = foo();
        omp_set_num_threads(max_threads);

        return result;
    }

}
