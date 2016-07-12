#include "images.h"

#include <cassert>

#include "CImg.h"
#undef cimg_display

using namespace images;
using namespace cimg_library;

Image::Image(size_t width, size_t height, int cn, const std::shared_ptr<unsigned char> &data) : Image(width, height, cn, data, 0, width * cn) {}

Image::Image(size_t width, size_t height, int cn, const std::shared_ptr<unsigned char> &data, size_t offset, ptrdiff_t stride)
        : width(width), height(height), cn(cn), offset(offset), stride(stride), data(data) {
    if (!this->data) {
        this->data = std::shared_ptr<unsigned char>(new unsigned char[width * height * cn]);
    }
}

Image::Image(const Image &that) : width(that.width), height(that.height), cn(that.cn), offset(that.offset), stride(that.stride), data(that.data) {}

Image Image::copy() const {
    Image result(width, height, cn);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            result(y, x) = this->operator()(y, x);
        }
    }
    return result;
}

unsigned char& Image::operator()(size_t row, size_t col) {
    assert (row < height);
    assert (col < width);
    return data.get()[offset + row * stride + col * cn];
}

unsigned char& Image::operator()(size_t row, size_t col, int c) {
    assert (c >= 0 && c < cn);
    return data.get()[offset + row * stride + col * cn + c];
}

unsigned char Image::operator()(size_t row, size_t col) const {
    assert (row < height);
    assert (col < width);
    return data.get()[offset + row * stride + col * cn];
}

unsigned char Image::operator()(size_t row, size_t col, int c) const {
    assert (c >= 0 && c < cn);
    return data.get()[offset + row * stride + col * cn + c];
}

void Image::fill(unsigned char value) {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            for (int c = 0; c < cn; c++) {
                this->operator()(y, x, c) = value;
            }
        }
    }
}

void Image::fill(unsigned char value[]) {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            for (int c = 0; c < cn; c++) {
                this->operator()(y, x, c) = value[c];
            }
        }
    }
}

ImageWindow Image::show(const char *title) {
    ImageWindow window = ImageWindow(title);
    window.display(*this);
    return window;
}




class CImg8U {
public:
    CImg<unsigned char> img;

    CImg8U(size_t width, size_t height, int cn) : img(width, height, 1, cn) {}
};

class CImgDisplay8U {
public:
    CImgDisplay display;
};




ImageWindow::ImageWindow(std::string title) : title(title) {
    cimg_display = new CImgDisplay8U();
    setTitle(title);
}

ImageWindow::~ImageWindow() {
    delete cimg_display;
}

void ImageWindow::display(Image image) {
    CImg<unsigned char> img(image.width, image.height, 1, image.cn);

    unsigned char* dst = img.data();
    for (int c = 0; c < image.cn; c++) {
        unsigned char* src = image.ptr() + c;
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