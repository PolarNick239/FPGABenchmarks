#include <iostream>
#include "images.h"

using images::Image;
using images::ImageWindow;

int main() {
    const int MAX_ITERATIONS = 1024;

    size_t width = 1024;
    size_t height = 768;
    Image<unsigned char> image(width, height, 3);

    ImageWindow window("Mandelbrot");

    do {
        for (size_t py = 0; py < height; py++) {
            for (size_t px = 0; px < width; px++) {
                int iteration;
                float x0 = 2.0f * (((float) px) / width - 0.5f);
                float y0 = 2.0f * (((float) py) / width - 0.5f);
                float x = 0.0f;
                float y = 0.0f;
                for (iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
                    float xn = x * x - y * y + x0;
                    y = 2 * x * y + y0;
                    x = xn;
                    if (x * x + y * y > 10000.0f) {
                        break;
                    }
                }
                for (int c = 0; c < 3; c++) {
                    image(py, px, c) = (unsigned char) (255 * iteration / MAX_ITERATIONS);
                }
            }
        }

        window.display(image);

        if (window.isResized()) {
            width = window.width();
            height = window.height();
            image = Image<unsigned char>(width, height, 3);
            window.resize();
        }

        window.wait(20);
    } while (!window.isClosed());
    return 0;
}