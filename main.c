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

    vector3_t r = { .x = 255, .y = 1, .z = 1};
    vector3_t g = { .x = 1, .y = 255, .z = 1};
    vector3_t b = { .x = 255, .y = 1, .z = 255};

    Triangle_t triangle = Triangle_new(Vertex_new(vector3_new(0.5, 0.8, 0), r), Vertex_new(vector3_new(-0.7, 0.2, 0), g), Vertex_new(vector3_new(0, 0, 0), b));

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 600, 600);

    for (;;)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                return 0;
        }

        int pitch;

        SDL_LockTexture(texture, NULL, (void **)&ctx.framebuffer, &pitch);

        draw_triangle(&ctx, &triangle);

        SDL_UnlockTexture(texture);

        SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_RenderPresent(renderer);
    }

    return 0;
}