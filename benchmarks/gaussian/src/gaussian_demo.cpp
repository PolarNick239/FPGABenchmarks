#include <iostream>
#include <cmath>

#include "gaussian.h"

using images::Image;
using images::ImageWindow;

int blurLoop(Image<unsigned char>& image) {
    ImageWindow window("Gaussian blur");
    GaussianProcessorCPU<unsigned char> processor;

    int step = 0;
    do {
        Image<unsigned char> blurred = image.copy();
        double sigma = (sin(step * M_PI / 30) + 1.0) * 4;
        processor.process(blurred, sigma);

        window.display(blurred);
        window.wait(20);
        step++;
    } while (!window.isClosed());
    return 0;
}

int main(int argc, char *argv[]) {
    Image<unsigned char> image(0, 0, 0);

    if (argc != 2) {
        std::cout << "Usage:" << std::endl;
        std::cout << "  " << argv[0] << " <ImagePath>" << std::endl;
        return 0;
    }

    std::cout << "Loading image '" << argv[1] << "'..." << std::endl;
    try {
        image = Image<unsigned char>(argv[1]);
    } catch (std::exception& exception) {
        std::cerr << "Can't load image: " << exception.what() << std::endl;
        return 0;
    }
    std::cout << "Image " << image.width << "x" << image.height << "x" << image.cn << " loaded!" << std::endl;

    return blurLoop(image);
}
