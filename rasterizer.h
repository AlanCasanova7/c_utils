#include "my_math.h"
#include <stdlib.h>
#include <string.h>
typedef struct Context
{
    vector3_t camera;
    int width;
    int height;

    unsigned char *framebuffer;

} Context_t;

typedef struct Vertex
{
    vector3_t position;
    vector3_t normal;
    vector3_t color;
    vector3_t view_position;

    int raster_x;
    int raster_y;
} Vertex_t;


typedef struct Triangle
{
    Vertex_t a;
    Vertex_t b;
    Vertex_t c;
} Triangle_t;

Vertex_t Vertex_new(vector3_t position, vector3_t color);
Triangle_t Triangle_new(Vertex_t a, Vertex_t b, Vertex_t c);

void rasterize(Context_t *ctx, Triangle_t *triangle);
Triangle_t sort_triangle_vertex(Triangle_t* triangle);
void draw_triangle(Context_t* ctx, Triangle_t* triangle);
void append_vector(Triangle_t** array_of_vector, size_t* array_of_vector_size, Triangle_t value);
void put_pixel(Context_t* ctx, unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b);