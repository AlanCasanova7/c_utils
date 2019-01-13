#include "rasterizer.h"
#include <stdio.h>

Vertex_t Vertex_new(vector3_t position, vector3_t color)
{
    Vertex_t vertex;
    memset(&vertex, 0, sizeof(Vertex_t));
    vertex.position = position;
    vertex.color = color;
    return vertex;
}

Triangle_t Triangle_new(Vertex_t a, Vertex_t b, Vertex_t c)
{
    Triangle_t triangle = {.a = a, .b = b, .c = c};
    return triangle;
}

void put_pixel(Context_t* ctx, unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b){
    int pixel = ((y * ctx->width) + x) * 4;
    ctx->framebuffer[pixel--] = 255;
    ctx->framebuffer[pixel--] = r;
    ctx->framebuffer[pixel--] = g;
    ctx->framebuffer[pixel] = b;
}

void rasterize(Context_t *ctx, Triangle_t* triangle)
{
    vector2_t screen_size = {.width = ctx->width, .height = ctx->height};
    vector3_t screen_point_a = pixel_range_to_screen_point_v3(triangle->a.position, screen_size);
    vector3_t screen_point_b = pixel_range_to_screen_point_v3(triangle->b.position, screen_size);
    vector3_t screen_point_c = pixel_range_to_screen_point_v3(triangle->c.position, screen_size);

    triangle->a.raster_x = screen_point_a.x;
    triangle->b.raster_x = screen_point_b.x;
    triangle->c.raster_x = screen_point_c.x;

    triangle->a.raster_y = screen_point_a.y;
    triangle->b.raster_y = screen_point_b.y;
    triangle->c.raster_y = screen_point_c.y;
    return;
}

Triangle_t sort_triangle_vertex(Triangle_t* triangle){
    Triangle_t to_return;

    if(triangle->a.raster_y > triangle->b.raster_y){
        to_return.a = triangle->a;
        to_return.b = triangle->b;
    } 
    else {
        to_return.a = triangle->b;
        to_return.b = triangle->a;
    }

    if(to_return.b.raster_y > triangle->c.raster_y){
        to_return.c = triangle->c;
    } 
    else {
        to_return.c = to_return.b;
        to_return.b = triangle->c;
    }

    if(triangle->a.raster_y > to_return.c.raster_y){
        int old_a = to_return.a.raster_y;
        to_return.a.raster_y = to_return.c.raster_y;
        to_return.c.raster_y = old_a;
    }
    return to_return;
}

vector3_t interpolate_vertex_color(Vertex_t a, Vertex_t b, Vertex_t c, float x_position, float y_position){
    float weight_vertex_a = ((b.raster_y - c.raster_y) * (x_position - c.raster_x) + (c.raster_x - b.raster_x) * (y_position - c.raster_y)) / ((b.raster_y - c.raster_y) * (a.raster_x - c.raster_x) + (c.raster_x - b.raster_x) * (a.raster_y - c.raster_y));
    float weight_vertex_b = ((c.raster_y - a.raster_y) * (x_position - c.raster_x) + (a.raster_x - c.raster_x) * (y_position - c.raster_y)) / ((b.raster_y - c.raster_y) * (a.raster_x - c.raster_x) + (c.raster_x - b.raster_x) * (a.raster_y - c.raster_y)); 
    float weight_vertex_C = 1 - weight_vertex_a - weight_vertex_b;                                                                                                               

    float red = (weight_vertex_C * c.color.r) + (weight_vertex_b * b.color.r) + (weight_vertex_a * a.color.r);
    float green = (weight_vertex_C * c.color.g) + (weight_vertex_b * b.color.g) + (weight_vertex_a * a.color.g);
    float blue = (weight_vertex_C * c.color.b) + (weight_vertex_b * b.color.b) + (weight_vertex_a * a.color.b);
    
    vector3_t to_return = vector3_new(red, green, blue);
    return to_return;
}

void draw_triangle(Context_t* ctx, Triangle_t* triangle){
    rasterize(ctx, triangle);

    Triangle_t to_draw = sort_triangle_vertex(triangle);

    // float Vx1 = to_draw.a.raster_x;
    // float Vx2 = to_draw.b.raster_x;
    // float Vx3 = to_draw.c.raster_x;

    // float Vy1 = to_draw.a.raster_y;
    // float Vy2 = to_draw.b.raster_y;
    // float Vy3 = to_draw.c.raster_y;

    int y_position = to_draw.a.raster_y;
    for (; y_position <= to_draw.b.raster_y; y_position++)
    {
        float gradient = (float)(y_position - to_draw.a.raster_y) / (float)(to_draw.b.raster_y - to_draw.a.raster_y);
        float x_final_position = lerp(to_draw.a.raster_x, to_draw.b.raster_x, gradient);

        float total_gradient = (float)(y_position - to_draw.a.raster_y) / (float)(to_draw.c.raster_y - to_draw.a.raster_y);
        float starting_x = lerp(to_draw.a.raster_x, to_draw.c.raster_x, total_gradient);
        float end = calculate_max(starting_x, x_final_position);
        float x_position = calculate_min(starting_x, x_final_position);
        for (; x_position < end; x_position++)
        {
            vector3_t color = interpolate_vertex_color(to_draw.a, to_draw.b, to_draw.c, x_position, y_position);
            put_pixel(ctx, x_position, y_position, color.r, color.g, color.b);
        }
    }
    for (; y_position <= to_draw.c.raster_y; y_position++)
    {
        float gradient = (float)(y_position - to_draw.c.raster_y) / (float)(to_draw.b.raster_y - to_draw.c.raster_y);
        float x_final_position = lerp(to_draw.c.raster_x, to_draw.b.raster_x, gradient);

        float total_gradient = (float)(y_position - to_draw.a.raster_y) / (float)(to_draw.c.raster_y - to_draw.a.raster_y);
        float starting_x = lerp(to_draw.a.raster_x, to_draw.c.raster_x, total_gradient);

        float end = calculate_max(starting_x, x_final_position);
        float x_position = calculate_min(starting_x, x_final_position);
        for (; x_position < end; x_position++)
        {
            vector3_t color = interpolate_vertex_color(to_draw.a, to_draw.b, to_draw.c, x_position, y_position);
            put_pixel(ctx, x_position, y_position, color.r, color.g, color.b);
        }
    }
    return;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // int max_x = calculate_max(calculate_max(to_draw.a.raster_x, to_draw.b.raster_x), to_draw.c.raster_x);
    // int min_x = calculate_min(calculate_min(to_draw.a.raster_x, to_draw.b.raster_x), to_draw.c.raster_x);
    // int max_y = calculate_max(calculate_max(to_draw.a.raster_y, to_draw.b.raster_y), to_draw.c.raster_y);
    // int min_y = calculate_min(calculate_min(to_draw.a.raster_y, to_draw.b.raster_y), to_draw.c.raster_y);

    // int y_pos = min_y;

    // float Vx1 = to_draw.a.raster_x;
    // float Vx2 = to_draw.b.raster_x;
    // float Vx3 = to_draw.c.raster_x;

    // float Vy1 = to_draw.a.raster_y;
    // float Vy2 = to_draw.b.raster_y;
    // float Vy3 = to_draw.c.raster_y;

    // for(; y_pos <= max_y; y_pos++){
    //     int x_pos = max_x;
    //     for(; x_pos >= min_x; x_pos--){
    //         float a = ((Vy2 - Vy3)*(x_pos - Vx3) + (Vx3 - Vx2)*(y_pos - Vy3)) / ((Vy2 - Vy3)*(Vx1 - Vx3) + (Vx3 - Vx2)*(Vy1 - Vy3)); // massimo in p0
    //         float b = ((Vy3 - Vy1)*(x_pos - Vx3) + (Vx1 - Vx3)*(y_pos - Vy3)) / ((Vy2 - Vy3)*(Vx1 - Vx3) + (Vx3 - Vx2)*(Vy1 - Vy3)); // massimo in p1
    //         float c = 1 - a - b;                                                                                                     // massimo in p2


    //         if(0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1){
    //             //printf("a: %f, b: %f, c: %f\n", a, b, c);
    //             put_pixel(ctx, x_pos, y_pos, a * 255, b * 255, c * 255);
    //         }
    //     }
    // }
}