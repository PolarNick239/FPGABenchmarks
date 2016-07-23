#line 2

/*
 * Required defines:
 *      unsigned short MAX_ITERATIONS;
 *      float INFINITY_THRESHOLD;
 */

__kernel void mandelbrotProcess(__global unsigned short* iterations,
                                 int width, int height,
                                 float fromX, float fromY, float toX, float toY) {
    int px = get_global_id(0);
    int py = get_global_id(1);
    if (px >= width || py >= height) return;

    float x0 = fromX + px * (toX - fromX) / width;
    float y0 = fromY + py * (toY - fromY) / height;

    unsigned short iteration;
    float x = 0.0f;
    float y = 0.0f;
    for (iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        float xn = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xn;
        if (x * x + y * y > INFINITY_THRESHOLD) {
            break;
        }
    }
    iterations[width * py + px] = iteration;
}
