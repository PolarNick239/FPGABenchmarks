#include <iostream>
#include "images.h"

using images::Image;
using images::ImageWindow;

int main() {
    size_t width = 1024;
    size_t height = 768;
    Image image(width, height, 3);

    ImageWindow window("Mandelbrot");

    do {
        //TODO implement Mandelbrot rendering

        window.display(image);

        if (window.isResized()) {
            width = window.width();
            height = window.height();
            image = Image(width, height, 3);
            window.resize();
        }

        window.wait(20);
    } while (!window.isClosed());
    return 0;
}