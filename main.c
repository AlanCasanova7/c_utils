#define SDL_MAIN_HANDLED
#include "obj_parser.h"
// #include <stdlib.h>
#include <include/SDL.h>
#include <stdio.h>

#define def_triangle(x0, y0, z0, x1, y1, z1, x2, y2, z2, v0_col, v1_col, v2_col) \
    Triangle_new(Vertex_new(vector3_new(x0, y0, z0), r), Vertex_new(vector3_new(x1, y1, z1), g), Vertex_new(vector3_new(x2, y2, z2), b));

int main(int argc, char **argv)
{
    Context_t ctx;
    ctx.camera = vector3_new(0, 2, -3);
    ctx.width = 600;
    ctx.height = 600;

    ctx.framebuffer = NULL;

    obj_model_t* obj = load_obj("Stormtrooper.obj");

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

            if (event.type == SDL_KEYDOWN)
            {
                printf("%f\n", ctx.camera.z);
                ctx.camera.z--;
            }
        }
        int pitch;

        SDL_LockTexture(texture, NULL, (void **)&ctx.framebuffer, &pitch);

        // for(int i = 0; i < array_of_vector_size; i++){
        //     draw_triangle(&ctx, &array_of_vector[i]);
        // }
        //printf("HERE: a_x: %d, a_y: %d", &obj->array_of_vector[0].a.raster_x,  &obj->array_of_vector[0].a.raster_y);
        draw_obj(&ctx, obj);

        //draw_triangle(&ctx, &triangle_c);
        //draw_triangle(&ctx, &triangle_d);

        SDL_UnlockTexture(texture);

        SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_RenderPresent(renderer);

        memset(ctx.framebuffer, 0, 600 * 600 * 4);
    }

    return 0;
}