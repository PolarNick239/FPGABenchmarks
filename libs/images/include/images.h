#pragma once

#include <memory>
#include <cstddef>

template <typename T>
class CImgWrapper;

class CImgDisplayWrapper;

namespace images {

    template <typename T>
    class Image;

    class ImageWindow {
    public:
        ImageWindow(std::string title);
        ~ImageWindow();

        template<typename T>
        void display(Image<T> image);
        void resize();
        void wait(unsigned int milliseconds);
        void setTitle(std::string title);

        bool isClosed();
        bool isResized();

        size_t width();
        size_t height();

    protected:
        CImgDisplayWrapper* cimg_display;
        std::string title;
    };

    template <typename T>
    class Image {
    public:
        size_t width;
        size_t height;
        size_t cn;

        Image(size_t width, size_t height, size_t cn, const std::shared_ptr<T> &data=nullptr);
        Image(size_t width, size_t height, size_t cn, const std::shared_ptr<T> &data, size_t offset, ptrdiff_t stride);
        Image(const Image &image);

        T operator()(size_t row, size_t col) const;
        T operator()(size_t row, size_t col, size_t c) const;
        T& operator()(size_t row, size_t col);
        T& operator()(size_t row, size_t col, size_t c);
        Image copy() const;

        void fill(T value);
        void fill(T value[]);
        void replace(T a, T b);
        void replace(T a[], T b[]);

        ImageWindow show(const char* title);

        T* ptr() { return data.get(); }

    protected:
        size_t offset;
        ptrdiff_t stride;

        std::shared_ptr<T> data;
    };

}
