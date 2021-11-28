#include "emscripten.h"
#include "src/webp/encode.h"
#include <stdlib.h> // required for malloc definition

EMSCRIPTEN_KEEPALIVE
int version()
{
    return WebPGetEncoderVersion();
}

EMSCRIPTEN_KEEPALIVE
uint8_t *create_buffer(int width, int height)
{
    return malloc(width * height * 4 * sizeof(uint8_t));
}

EMSCRIPTEN_KEEPALIVE
void destroy_buffer(uint8_t *p)
{
    free(p);
}

EMSCRIPTEN_KEEPALIVE
uint8_t *blur(uint8_t *img_in, int width, int height, float distance)
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            int new_red = 0;
            int new_green = 0;
            int new_blue = 0;
            int count = 0;

            // check boundaries
            int row_start = (row > distance) ? row - distance : 0;
            int row_end = (row + distance + 1 < height) ? row + distance + 1 : height;
            for (int r = row_start; r < row_end; r++)
            {
                int col_start = (col > distance) ? col - distance : 0;
                int col_end = (col + distance + 1 < width) ? col + distance + 1 : width;
                for (int c = col_start; c < col_end; c++)
                {
                    int curr_pixel = (r * width + col) * 4;
                    new_red += img_in[curr_pixel + 0];
                    new_green += img_in[curr_pixel + 1];
                    new_blue += img_in[curr_pixel + 2];
                    count++;
                }
            }

            // compute average
            new_red /= count;
            new_green /= count;
            new_blue /= count;
            int new_pixel = (row * width + col) * 4;
            img_in[new_pixel + 0] = new_red;
            img_in[new_pixel + 1] = new_green;
            img_in[new_pixel + 2] = new_blue;
        }
    }
    return img_in;
}

int result[2];
EMSCRIPTEN_KEEPALIVE
void encode(uint8_t *img_in, int width, int height, float quality)
{
    uint8_t *img_out;
    size_t size;

    size = WebPEncodeRGBA(img_in, width, height, width * 4, quality, &img_out);

    result[0] = (int)img_out;
    result[1] = size;
}

EMSCRIPTEN_KEEPALIVE
void free_result(uint8_t *result)
{
    WebPFree(result);
}

EMSCRIPTEN_KEEPALIVE
int get_result_pointer()
{
    return result[0];
}

EMSCRIPTEN_KEEPALIVE
int get_result_size()
{
    return result[1];
}
