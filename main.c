#define SDL_MAIN_HANDLED
#include "rasterizer.h"
// #include <stdlib.h>
#include <include/SDL.h>
#include <stdio.h>

#define def_triangle(x0, y0, z0, x1, y1, z1, x2, y2, z2, v0_col, v1_col, v2_col) \
    Triangle_new(Vertex_new(vector3_new(x0, y0, z0), r), Vertex_new(vector3_new(x1, y1, z1), g), Vertex_new(vector3_new(x2, y2, z2), b));

int main(int argc, char **argv)
{
    Context_t ctx;
    ctx.camera = vector3_new(0, 0, -5);
    ctx.width = 600;
    ctx.height = 600;

    ctx.framebuffer = NULL;

    vector3_t color1 = { .x = 255, .y = 0, .z = 255};
    vector3_t color2 = { .x = 0, .y = 255, .z = 125};
    vector3_t color3 = { .x = 0, .y = 0, .z = 255};

    Triangle_t triangle_a = Triangle_new(Vertex_new(vector3_new(-1, 0, 2), color1), Vertex_new(vector3_new(-1, -1, 2), color2), Vertex_new(vector3_new(0, -1, 2), color3));
    Triangle_t triangle_b = Triangle_new(Vertex_new(vector3_new(1, -1, 0), color2), Vertex_new(vector3_new(0, -1, 0), color2), Vertex_new(vector3_new(1, 0, 1), color3));
    Triangle_t triangle_c = Triangle_new(Vertex_new(vector3_new(-1, 1, 1), color1), Vertex_new(vector3_new(-1, 0, 0), color2), Vertex_new(vector3_new(0, 1, 1), color3));
    Triangle_t triangle_d = Triangle_new(Vertex_new(vector3_new(1, 1, -1), color1), Vertex_new(vector3_new(0, 1, 1), color2), Vertex_new(vector3_new(1, 0, 0), color3));

    Triangle_t* array_of_vector = NULL;
    size_t array_of_vector_size = 0;

    append_vector(&array_of_vector, &array_of_vector_size, triangle_a);
    //printf("%llu\n", sizeof(array_of_vector));
    append_vector(&array_of_vector, &array_of_vector_size, triangle_b);
    append_vector(&array_of_vector, &array_of_vector_size, triangle_c);
    append_vector(&array_of_vector, &array_of_vector_size, triangle_d);
    //printf("%llu\n", sizeof(array_of_vector));

    // array_of_vector_size += 1;
    // Triangle_t* resized_area = (Triangle_t*)realloc(array_of_vector,sizeof(Triangle_t) * array_of_vector_size);
    // if(!resized_area){
    //     printf("PANIC");
    //     return 0;
    // }
    // array_of_vector = resized_area;
    // array_of_vector[array_of_vector_size-1] = triangle_a;

    // array_of_vector_size += 1;
    // resized_area = (Triangle_t*)realloc(array_of_vector,sizeof(Triangle_t) * array_of_vector_size);
    // if(!resized_area){
    //     printf("PANIC");
    //     return 0;
    // }
    // array_of_vector = resized_area;
    // array_of_vector[array_of_vector_size-1] = triangle_b;

    // array_of_vector_size += 1;
    // resized_area = (Triangle_t*)realloc(array_of_vector,sizeof(Triangle_t) * array_of_vector_size);
    // if(!resized_area){
    //     printf("PANIC");
    //     return 0;
    // }
    // array_of_vector = resized_area;
    // array_of_vector[array_of_vector_size-1] = triangle_c;

    // array_of_vector_size += 1;
    // resized_area = (Triangle_t*)realloc(array_of_vector,sizeof(Triangle_t) * array_of_vector_size);
    // if(!resized_area){
    //     printf("PANIC");
    //     return 0;
    // }
    // array_of_vector = resized_area;
    // array_of_vector[array_of_vector_size-1] = triangle_d;

    // append_vector(array_of_vector, &array_of_vector_size, triangle_a);
    // printf("%f\n", array_of_vector[0].a.position.x);
    //append_vector(&array_of_vector, &array_of_vector_size, &triangle_c);
    //append_vector(&array_of_vector, &array_of_vector_size, &triangle_d);
    //printf("%zu", array_of_vector_size);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 600, 600);

    // int dir_a = -1;
    // int dir_b = -1;
    // int dir_c = -1;

    int pitch;

    SDL_LockTexture(texture, NULL, (void **)&ctx.framebuffer, &pitch);

    // for(int i = 0; i < array_of_vector_size; i++){
    //     draw_triangle(&ctx, &array_of_vector[i]);
    // }
    for(int i = 0; i < array_of_vector_size; i++){
        draw_triangle(&ctx, &array_of_vector[i]);
    }
    //draw_triangle(&ctx, &array_of_vector[0]);
    //draw_triangle(&ctx, &triangle_c);
    //draw_triangle(&ctx, &triangle_d);

    SDL_UnlockTexture(texture);

    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_RenderPresent(renderer);

    //memset(ctx.framebuffer, 0, 600 * 600 * 4);

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
    }

    return 0;
}