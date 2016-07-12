#include <memory>
#include <cstddef>

class CImg8U;
class CImgDisplay8U;

namespace images {

    class Image;

    class ImageWindow {
    public:
        ImageWindow(std::string title);
        ~ImageWindow();

        void display(Image image);
        void resize();
        void wait(unsigned int milliseconds);
        void setTitle(std::string title);

        bool isClosed();
        bool isResized();

        size_t width();
        size_t height();

    protected:
        CImgDisplay8U* cimg_display;
        std::string title;
    };

    class Image {
    public:
        size_t width;
        size_t height;
        int cn;

        Image(size_t width, size_t height, int cn, const std::shared_ptr<unsigned char> &data=nullptr);
        Image(size_t width, size_t height, int cn, const std::shared_ptr<unsigned char> &data, size_t offset, ptrdiff_t stride);
        Image(const Image &image);

        unsigned char operator()(size_t row, size_t col) const;
        unsigned char operator()(size_t row, size_t col, int c) const;
        unsigned char& operator()(size_t row, size_t col);
        unsigned char& operator()(size_t row, size_t col, int c);
        Image copy() const;

        void fill(unsigned char value);
        void fill(unsigned char value[]);

        ImageWindow show(const char* title);

        unsigned char* ptr() { return data.get(); }

    protected:
        size_t offset;
        ptrdiff_t stride;

        std::shared_ptr<unsigned char> data;
    };

}
