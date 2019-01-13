#define SDL_MAIN_HANDLED
#include "rasterizer.h"
// #include <stdlib.h>
#include <include/SDL.h>

#define def_triangle(x0, y0, z0, x1, y1, z1, x2, y2, z2, v0_col, v1_col, v2_col) \
    Triangle_new(Vertex_new(vector3_new(x0, y0, z0), r), Vertex_new(vector3_new(x1, y1, z1), g), Vertex_new(vector3_new(x2, y2, z2), b));

int main(int argc, char **argv)
{
    Context_t ctx;
    ctx.width = 600;
    ctx.height = 600;

    ctx.framebuffer = NULL;

    vector3_t color1 = { .x = 255, .y = 0, .z = 0};
    vector3_t color2 = { .x = 0, .y = 255, .z = 0};
    vector3_t color3 = { .x = 0, .y = 0, .z = 255};

    Triangle_t triangle = Triangle_new(Vertex_new(vector3_new(0.5, 0.8, 0), color1), Vertex_new(vector3_new(-0.7, 0.2, 0), color2), Vertex_new(vector3_new(0, 0, 0), color3));

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 600, 600);

    int dir_a = -1;
    int dir_b = -1;
    int dir_c = -1;

    for (;;)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                return 0;
        }

        if(triangle.a.color.g == 255 || triangle.a.color.g == 0){ dir_a *= -1; }
        triangle.a.color.g = (int)(triangle.a.color.g + dir_a) % 256;

        if(triangle.b.color.b == 255 || triangle.b.color.b == 0){ dir_b *= -1; }
        triangle.b.color.b = (int)(triangle.b.color.b + dir_b) % 256;

        if(triangle.c.color.r == 255 || triangle.c.color.r == 0){ dir_c *= -1; }
        triangle.c.color.r = (int)(triangle.c.color.r + dir_c) % 256;

        int pitch;

        SDL_LockTexture(texture, NULL, (void **)&ctx.framebuffer, &pitch);

        draw_triangle(&ctx, &triangle);

        SDL_UnlockTexture(texture);

        SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_RenderPresent(renderer);
    }

    return 0;
}