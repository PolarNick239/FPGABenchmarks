#include "images.h"

#include <cassert>

#include "CImg.h"
#undef cimg_display

using namespace images;
using namespace cimg_library;

template <typename T>
Image<T>::Image(size_t width, size_t height, int cn, const std::shared_ptr<T> &data) : Image(width, height, cn, data, 0, width * cn) {}

template <typename T>
Image<T>::Image(size_t width, size_t height, int cn, const std::shared_ptr<T> &data, size_t offset, ptrdiff_t stride)
        : width(width), height(height), cn(cn), offset(offset), stride(stride), data(data) {
    if (!this->data) {
        this->data = std::shared_ptr<T>(new T[width * height * cn]);
    }
}

template <typename T>
Image<T>::Image(const Image &that) : width(that.width), height(that.height), cn(that.cn), offset(that.offset), stride(that.stride), data(that.data) {}

template <typename T>
Image<T> Image<T>::copy() const {
    Image result(width, height, cn);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            result(y, x) = this->operator()(y, x);
        }
    }
    return result;
}

template <typename T>
T& Image<T>::operator()(size_t row, size_t col) {
    // TODO: check that maybe if inlined in header - performance is better
    assert (row < height);
    assert (col < width);
    return data.get()[offset + row * stride + col * cn];
}

template <typename T>
T& Image<T>::operator()(size_t row, size_t col, int c) {
    // TODO: check that maybe if inlined in header - performance is better
    assert (c >= 0 && c < cn);
    return data.get()[offset + row * stride + col * cn + c];
}

template <typename T>
T Image<T>::operator()(size_t row, size_t col) const {
    // TODO: check that maybe if inlined in header - performance is better
    assert (row < height);
    assert (col < width);
    return data.get()[offset + row * stride + col * cn];
}

template <typename T>
T Image<T>::operator()(size_t row, size_t col, int c) const {
    // TODO: check that maybe if inlined in header - performance is better
    assert (c >= 0 && c < cn);
    return data.get()[offset + row * stride + col * cn + c];
}

template <typename T>
void Image<T>::fill(T value) {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            for (int c = 0; c < cn; c++) {
                this->operator()(y, x, c) = value;
            }
        }
    }
}

template <typename T>
void Image<T>::fill(T value[]) {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            for (int c = 0; c < cn; c++) {
                this->operator()(y, x, c) = value[c];
            }
        }
    }
}

template <typename T>
ImageWindow Image<T>::show(const char *title) {
    ImageWindow window = ImageWindow(title);
    window.display(*this);
    return window;
}




template <typename T>
class CImgWrapper {
public:
    CImg<T> img;

    CImgWrapper(size_t width, size_t height, int cn) : img(width, height, 1, cn) {}
};

class CImgDisplayWrapper {
public:
    CImgDisplay display;
};




ImageWindow::ImageWindow(std::string title) : title(title) {
    cimg_display = new CImgDisplayWrapper();
    setTitle(title);
}

ImageWindow::~ImageWindow() {
    delete cimg_display;
}

template<typename T>
void ImageWindow::display(Image<T> image) {
    CImg<T> img(image.width, image.height, 1, image.cn);

    T* dst = img.data();
    for (int c = 0; c < image.cn; c++) {
        T* src = image.ptr() + c;
        for (size_t y = 0; y < image.height; y++) {
            for (size_t x = 0; x < image.width; x++) {
                *dst = *src;
                ++dst;
                src += image.cn;
            }
        }
    }

    cimg_display->display.display(img);
    setTitle(title);
}

void ImageWindow::resize() {
    cimg_display->display.resize();
}

void ImageWindow::wait(unsigned int milliseconds) {
    cimg_display->display.wait(milliseconds);
}

void ImageWindow::setTitle(std::string title) {
    cimg_display->display.set_title(title.data());
}

bool ImageWindow::isClosed() {
    return cimg_display->display.is_closed();
}

bool ImageWindow::isResized() {
    return cimg_display->display.is_resized();
}

size_t ImageWindow::width() {
    return cimg_display->display.window_width();
}

size_t ImageWindow::height() {
    return cimg_display->display.window_height();
}




template void ImageWindow::display<unsigned char>(Image<unsigned char> image);
template class Image<unsigned char>;
