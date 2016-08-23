#include <limits>

namespace helper {

    template<typename T>
    struct AccumulatingTypeHelper;

    template<>
    struct AccumulatingTypeHelper<unsigned char> {
        typedef unsigned short type;
    };

    template<>
    struct AccumulatingTypeHelper<float> {
        typedef double type;
    };

}

template<class T, class Enable = void>
struct AccumulatingType;

template<class T>
struct AccumulatingType<T, typename std::enable_if<!std::is_floating_point<T>::value>::type> {
    typedef typename helper::AccumulatingTypeHelper<T>::type type;
    static const size_t capacityLimit = std::numeric_limits<type>::max() / std::numeric_limits<T>::max();
};

template<class T>
struct AccumulatingType<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
    typedef typename helper::AccumulatingTypeHelper<T>::type type;
    static const size_t capacityLimit = std::numeric_limits<size_t>::max();
};
