export void mandelbrotProcess(uniform unsigned int16 iterations[],
                              uniform int width, uniform int height,
                              uniform float fromX, uniform float fromY, uniform float toX, uniform float toY,
                              uniform unsigned int16 max_iterations, uniform float infinity_threshold) {
    foreach (px = 0 ... width, py = 0 ... height) {
        float x0 = fromX + px * (toX - fromX) / width;
        float y0 = fromY + py * (toY - fromY) / height;

        unsigned int16 iteration;
        float x = 0.0f;
        float y = 0.0f;
        for (iteration = 0; iteration < max_iterations; iteration++) {
            float xn = x * x - y * y + x0;
            y = 2 * x * y + y0;
            x = xn;
            if (x * x + y * y > infinity_threshold) {
                break;
            }
        }
        iterations[width * py + px] = iteration;
    }
}
