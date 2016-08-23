#include "images.h"

#include <cassert>

#include "CImg.h"
#undef cimg_display

using namespace images;
using namespace cimg_library;

template <typename T>
Image<T>::Image(size_t width, size_t height, size_t cn, const std::shared_ptr<T> &data) : Image(width, height, cn, data, 0, width * cn) {}

template <typename T>
Image<T>::Image(size_t width, size_t height, size_t cn, const std::shared_ptr<T> &data, size_t offset, ptrdiff_t stride)
        : width(width), height(height), cn(cn), offset(offset), stride(stride), data(data) {
    if (!this->data) {
        allocateData();
    }
}

template <typename T>
void Image<T>::allocateData() {
    this->data = std::shared_ptr<T>(new T[width * height * cn]);
}

template <typename T>
Image<T>::Image(const Image &that) : width(that.width), height(that.height), cn(that.cn), offset(that.offset), stride(that.stride), data(that.data) {}

template <typename T>
Image<T>::Image(const char *const filename) {
    CImg<T> img(filename);

    width = img.width();
    height = img.height();
    cn = img.spectrum();
    offset = 0;
    stride = width * cn;
    allocateData();

    T* src = img.data();
    for (size_t c = 0; c < cn; c++) {
        T* dst = this->ptr() + c;
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                *dst = *src;
                ++src;
                dst += cn;
            }
        }
    }
}

template <typename T>
Image<T> Image<T>::copy() const {
    Image<T> result(width, height, cn);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            for (size_t c = 0; c < cn; c++) {
               result(y, x, c) = this->operator()(y, x, c);
            }
        }
    }
    return result;
}

template<typename T>
Image<T> Image<T>::reshape(size_t width, size_t height, size_t cn) {
    assert (this->width * this->height * this->cn == width * height * cn);
    return Image<T>(width, height, cn, this->data, this->offset, this->stride);
}

template <typename T>
void Image<T>::fill(T value) {
    assert (cn == 1);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            for (size_t c = 0; c < cn; c++) {
                this->operator()(y, x, c) = value;
            }
        }
    }
}

template <typename T>
void Image<T>::fill(T value[]) {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            for (size_t c = 0; c < cn; c++) {
                this->operator()(y, x, c) = value[c];  // TODO: compare performance if accurately increasing pointer
            }
        }
    }
}

template <typename T>
void Image<T>::replace(T a, T b) {
    assert (cn == 1);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (this->operator()(y, x) == a) {
                this->operator()(y, x) = b;
            }
        }
    }
}

template <typename T>
void Image<T>::replace(T a[], T b[]) {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            bool match = true;
            for (size_t c = 0; c < cn; c++) {
                if (this->operator()(y, x, c) != a[c]) {
                    match = false;
                    break;
                }
            }

            if (match) {
                for (size_t c = 0; c < cn; c++) {
                    this->operator()(y, x, c) = b[c];
                }
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

    CImgWrapper(size_t width, size_t height, size_t cn) : img(width, height, 1, cn) {}
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
    for (size_t c = 0; c < image.cn; c++) {
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
template void ImageWindow::display<unsigned short>(Image<unsigned short> image);
template void ImageWindow::display<float>(Image<float> image);
template class Image<unsigned char>;
template class Image<unsigned short>;
template class Image<float>;
