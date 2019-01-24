#include "my_math.h"
#include <stdlib.h>
#include <string.h>
typedef struct Context
{
    vector3_t camera;
    int width;
    int height;

    unsigned char *framebuffer;
    float *depth_buffer;
    unsigned char *stencil_buffer;

    vector3_t point_light_position;
} Context_t;

typedef struct Vertex
{
    vector3_t position;
    vector3_t normal;
    vector3_t color;
    vector2_t uv;

    vector3_t view_position;
    
    vector3_t world_position;
    vector3_t world_normal;

    int raster_x;
    int raster_y;

    float z;
} Vertex_t;

typedef struct Triangle
{
    Vertex_t a;
    Vertex_t b;
    Vertex_t c;
} Triangle_t;

typedef struct obj_model{
    Triangle_t* triangle_array;
    size_t number_of_triangles;
} obj_model_t;

Vertex_t Vertex_new(vector3_t position, vector3_t color);
Triangle_t Triangle_new(Vertex_t a, Vertex_t b, Vertex_t c);

void rasterize(Context_t *ctx, Triangle_t *triangle);
Triangle_t sort_triangle_vertex(Triangle_t* triangle);
void draw_triangle(Context_t* ctx, Triangle_t* triangle);
void draw_obj(Context_t* ctx, obj_model_t* model);
void put_pixel(Context_t* ctx, unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b);