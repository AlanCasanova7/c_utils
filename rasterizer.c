#include "rasterizer.h"

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

void draw_triangle(Context_t* ctx, Triangle_t* triangle){
    rasterize(ctx, triangle);

    Triangle_t to_draw = sort_triangle_vertex(triangle);
    
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
            put_pixel(ctx, x_position, y_position, 0, 255, 255);
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
            put_pixel(ctx, x_position, y_position, 255, 100, 0);
        }
    }
    return;

    // int max_x = calculate_max(calculate_max(triangle->a.raster_x, triangle->b.raster_x), triangle->c.raster_x);
    // int min_x = calculate_min(calculate_min(triangle->a.raster_x, triangle->b.raster_x), triangle->c.raster_x);
    // int max_y = calculate_max(calculate_max(triangle->a.raster_y, triangle->b.raster_y), triangle->c.raster_y);
    // int min_y = calculate_min(calculate_min(triangle->a.raster_y, triangle->b.raster_y), triangle->c.raster_y);

    // int y_pos = min_y;

    // float Vx1 = triangle->a.raster_x;
    // float Vx2 = triangle->b.raster_x;
    // float Vx3 = triangle->c.raster_x;

    // float Vy1 = triangle->a.raster_y;
    // float Vy2 = triangle->b.raster_y;
    // float Vy3 = triangle->c.raster_y;

    // for(; y_pos <= max_y; y_pos++){
    //     int x_pos = max_x;
    //     for(; x_pos >= min_x; x_pos--){
    //         float a = ((Vy2 - Vy3)*(x_pos - Vx3) + (Vx3 - Vx2)*(y_pos - Vy3)) / ((Vy2 - Vy3)*(Vx1 - Vx3) + (Vx3 - Vx2)*(Vy1 - Vy3));
    //         float b = ((Vy3 - Vy1)*(x_pos - Vx3) + (Vx1 - Vx3)*(y_pos - Vy3)) / ((Vy2 - Vy3)*(Vx1 - Vx3) + (Vx3 - Vx2)*(Vy1 - Vy3));
    //         float c = 1 - a - b;
    //         if(0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1){
    //             put_pixel(ctx, x_pos, y_pos, 255, 255, 255);
    //         }
    //     }
    // }
}