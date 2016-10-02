#include <iostream>

#include "mandelbrot.h"

using images::Image;
using images::ImageWindow;
using primitives::Vector2f;

int main() {
    size_t width = 1024;
    size_t height = 768;
    Image<unsigned char> image(width, height, 3);
    Image<unsigned short> iterations(width, height, 1);

    ImageWindow window("Mandelbrot");

    std::unique_ptr<MandelbrotProcessor> processor;
    if (MandelbrotProcessorOCL::available()) {
        std::cout << "Using OpenCL implementation..." << std::endl;

        cl::Engine_ptr engine = cl::createGPUEngine();

        if (engine) {
            engine->device->printInfo();

            if (engine->init()) {
                MandelbrotProcessorOCL *ocl_processor = new MandelbrotProcessorOCL(engine);
                if (ocl_processor->init()) {
                    processor = std::unique_ptr<MandelbrotProcessor>((MandelbrotProcessor *) ocl_processor);
                } else {
                    std::cout << "OpenCL implementation failed to initialize!" << std::endl;
                }
            } else {
                std::cout << "Failed to initialize context and queue!" << std::endl;
            }
        } else {
            std::cout << "No GPU device." << std::endl;
        }
    }

    if (!processor) {
        if (MandelbrotProcessorCPU_AVX::available()) {
            std::cout << "Using CPU AVX2 implementation..." << std::endl;
            processor = std::unique_ptr<MandelbrotProcessor>((MandelbrotProcessor *) new MandelbrotProcessorCPU_AVX());
        } else if (MandelbrotProcessorCPU_SSE::available()) {
            std::cout << "Using CPU SSE2+SSSE3 implementation..." << std::endl;
            processor = std::unique_ptr<MandelbrotProcessor>((MandelbrotProcessor *) new MandelbrotProcessorCPU_SSE());
        } else {
            std::cout << "Using CPU implementation..." << std::endl;
            processor = std::unique_ptr<MandelbrotProcessor>((MandelbrotProcessor *) new MandelbrotProcessorCPU());
        }
    }

    do {
        if (window.isResized()) {
            width = window.width();
            height = window.height();
            image = Image<unsigned char>(width, height, 3);
            iterations = Image<unsigned short>(width, height, 1);
            window.resize();
        }

        float worldWidth = 2.0f;
        float worldHeight = 2.3f;
        if (worldWidth / image.width < worldHeight / image.height) {
            worldWidth = image.width * worldHeight / image.height;
        } else {
            worldHeight = image.height * worldWidth / image.width;
        }
        processor->process(Vector2f(-0.5f - worldWidth / 2.0f, -worldHeight / 2.0f),
                           Vector2f(-0.5f + worldWidth / 2.0f, worldHeight / 2.0f), iterations);

        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                float intensity = ((float) iterations(y, x, 0)) / MandelbrotProcessor::MAX_ITERATIONS;
                image(y, x, 0) = 173.0f * intensity;
                image(y, x, 1) = 12.0f + 189.0f * intensity;
                image(y, x, 2) = 173.0f * intensity;
            }
        }

        window.display(image);
        window.wait(20);
    } while (!window.isClosed());
    return 0;
}
