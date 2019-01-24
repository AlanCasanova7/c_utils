#include "obj_parser.h"
#include <stdio.h>
#include <math.h>

#define M_PI 3.1415926

Vertex_t Vertex_new(vector3_t position, vector3_t color){
    Vertex_t vertex;
    memset(&vertex, 0, sizeof(Vertex_t));
    vertex.position = position;
    vertex.color = color;
    return vertex;
}

Triangle_t Triangle_new(Vertex_t a, Vertex_t b, Vertex_t c){
    Triangle_t triangle = {.a = a, .b = b, .c = c};
    return triangle;
}

void put_pixel(Context_t* ctx, unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b){
    if(x < 0 || y < 0 || x >= ctx->width || y >= ctx->height){
        return;
    }

    //printf("%d, %d \n", x, y);

    int pixel = ((y * ctx->width) + x) * 4;
    ctx->framebuffer[pixel--] = 255;
    ctx->framebuffer[pixel--] = r;
    ctx->framebuffer[pixel--] = g;
    ctx->framebuffer[pixel] = b;
}

void view_to_raster(Context_t *ctx, Vertex_t* vertex){
    float fov = (60.0 / 2) * (M_PI / 180.0);
    float znear = 0.01;
    float zfar = 10;
    float camera_distance = tan(fov);

    float z = vertex->view_position.z;
    if (z == 0)
        z = znear; // ELSE IT WWILL BE A DIVISION BY 0 AND CRASH.

    vertex->z = linear_convert(z, znear, zfar, 0, 1);
    
    float projected_x = vertex->view_position.x / (camera_distance * vertex->view_position.z);
    float projected_y = vertex->view_position.y / (camera_distance * vertex->view_position.z);

    vertex->raster_x = (projected_x + 1) * (ctx->width * 0.5);
    vertex->raster_y = ctx->height - ((projected_y + 1) * (ctx->height * 0.5));
}

void rasterize(Context_t *ctx, Triangle_t* triangle){
    //vector2_t screen_size = {.width = ctx->width, .height = ctx->height};

    triangle->a.view_position.x = triangle->a.position.x - ctx->camera.x;
    triangle->a.view_position.y = triangle->a.position.y - ctx->camera.y;
    triangle->a.view_position.z = triangle->a.position.z - ctx->camera.z;

    triangle->b.view_position.x = triangle->b.position.x - ctx->camera.x;
    triangle->b.view_position.y = triangle->b.position.y - ctx->camera.y;
    triangle->b.view_position.z = triangle->b.position.z - ctx->camera.z;

    triangle->c.view_position.x = triangle->c.position.x - ctx->camera.x;
    triangle->c.view_position.y = triangle->c.position.y - ctx->camera.y;
    triangle->c.view_position.z = triangle->c.position.z - ctx->camera.z;

    //printf("BEFORE x: %f, y: %f, z: %f\n", triangle->a.view_position.x, triangle->a.view_position.y, triangle->a.view_position.z);

    view_to_raster(ctx, &triangle->a);
    //printf("AFTER x: %d, y: %d\n", triangle->a.raster_x, triangle->a.raster_y);
    view_to_raster(ctx, &triangle->b);
    view_to_raster(ctx, &triangle->c);
    
    //printf("a_x: %d, b_x: %d, c_x: %d\n", triangle->a.raster_x, triangle->b.raster_x, triangle->c.raster_x);
    //printf("a_y: %d, b_y: %d, c_y: %d\n", triangle->a.raster_y, triangle->b.raster_y, triangle->c.raster_y);
    // vector3_t screen_point_a = pixel_range_to_screen_point_v3(position_a, screen_size);
    // vector3_t screen_point_b = pixel_range_to_screen_point_v3(position_b, screen_size);
    // vector3_t screen_point_c = pixel_range_to_screen_point_v3(position_c, screen_size);

    // triangle->a.raster_x = screen_point_a.x;
    // triangle->a.raster_y = screen_point_a.y;
    
    // triangle->b.raster_x = screen_point_b.x;
    // triangle->b.raster_y = screen_point_b.y;
    
    // triangle->c.raster_x = screen_point_c.x;
    // triangle->c.raster_y = screen_point_c.y;
    return;
}

Triangle_t sort_triangle_vertex(Triangle_t* triangle){
    Triangle_t to_return;

    if(triangle->a.raster_y > triangle->b.raster_y){
        to_return.a = triangle->b;
        to_return.b = triangle->a;
    } 
    else {
        to_return.a = triangle->a;
        to_return.b = triangle->b;
    }

    if(to_return.b.raster_y > triangle->c.raster_y){
        to_return.c = to_return.b;
        to_return.b = triangle->c;
    } 
    else {
        to_return.c = triangle->c;
    }

    if(to_return.a.raster_y > to_return.b.raster_y){
        Vertex_t old_a = to_return.a;
        to_return.a = to_return.b;
        to_return.b = old_a;
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

void draw_obj(Context_t* ctx, obj_model_t* model){
    for(int i = 0; i < model->number_of_triangles; i++){
        draw_triangle(ctx, &model->triangle_array[i]);
    }
}

void draw_triangle(Context_t* ctx, Triangle_t* triangle){
    rasterize(ctx, triangle);
    Triangle_t to_draw = sort_triangle_vertex(triangle);
    
    // int y_position = to_draw.a.raster_y;
    // for (; y_position <= to_draw.b.raster_y; y_position++){
    //     float gradient = 1;
    //     if(to_draw.a.raster_y != to_draw.b.raster_y){
    //         gradient = (float)(y_position - to_draw.a.raster_y) / (float)(to_draw.b.raster_y - to_draw.a.raster_y);
    //     } 
    //     //float gradient = (float)(y_position - to_draw.a.raster_y) / (float)(to_draw.b.raster_y - to_draw.a.raster_y);
    //     float x_final_position = lerp(to_draw.a.raster_x, to_draw.b.raster_x, gradient);

    //     float total_gradient = 1;
    //     if(to_draw.a.raster_y != to_draw.c.raster_y){
    //         total_gradient = (float)(y_position - to_draw.a.raster_y) / (float)(to_draw.c.raster_y - to_draw.a.raster_y);
    //     }
    //     float starting_x = lerp(to_draw.a.raster_x, to_draw.c.raster_x, total_gradient);
    //     float end = calculate_max(starting_x, x_final_position);
    //     float x_position = calculate_min(starting_x, x_final_position);
    //     for (; x_position < end; x_position++)
    //     {
    //         //vector3_t color = interpolate_vertex_color(triangle->a, triangle->b, triangle->c, x_position, y_position);
    //         //put_pixel(ctx, x_position, y_position, color.r, color.g, color.b);
    //         put_pixel(ctx, x_position, y_position, 255, 255, 255);
    //     }
    // }
    // for (; y_position <= to_draw.c.raster_y; y_position++){
    //     float gradient = 1;
    //     if(to_draw.c.raster_y != to_draw.b.raster_y){
    //         gradient = (float)(y_position - to_draw.c.raster_y) / (float)(to_draw.b.raster_y - to_draw.c.raster_y);
    //     } 
    //     //float gradient = (float)(y_position - to_draw.c.raster_y) / (float)(to_draw.b.raster_y - to_draw.c.raster_y);
    //     float x_final_position = lerp(to_draw.c.raster_x, to_draw.b.raster_x, gradient);

    //     float total_gradient = 1;
    //     if(to_draw.a.raster_y != to_draw.c.raster_y){
    //         total_gradient = (float)(y_position - to_draw.a.raster_y) / (float)(to_draw.c.raster_y - to_draw.a.raster_y);
    //     }
    //     // float total_gradient = (float)(y_position - to_draw.a.raster_y) / (float)(to_draw.c.raster_y - to_draw.a.raster_y);
    //     float starting_x = lerp(to_draw.a.raster_x, to_draw.c.raster_x, total_gradient);

    //     //printf("x1: %f, x2: %f\n", starting_x, x_final_position);
    //     float end = calculate_max(starting_x, x_final_position);
    //     float x_position = calculate_min(starting_x, x_final_position);
    //     for (; x_position < end; x_position++)
    //     {
    //         //vector3_t color = interpolate_vertex_color(triangle->a, triangle->b, triangle->c, x_position, y_position);
    //         //put_pixel(ctx, x_position, y_position, color.r, color.g, color.b);
    //         put_pixel(ctx, x_position, y_position, 255, 255, 255);
    //     }
    // }
    // return;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int max_x = calculate_max(calculate_max(to_draw.a.raster_x, to_draw.b.raster_x), to_draw.c.raster_x);
    int min_x = calculate_min(calculate_min(to_draw.a.raster_x, to_draw.b.raster_x), to_draw.c.raster_x);
    int max_y = calculate_max(calculate_max(to_draw.a.raster_y, to_draw.b.raster_y), to_draw.c.raster_y);
    int min_y = calculate_min(calculate_min(to_draw.a.raster_y, to_draw.b.raster_y), to_draw.c.raster_y);

    int y_pos = min_y;

    float Vx1 = to_draw.a.raster_x;
    float Vx2 = to_draw.b.raster_x;
    float Vx3 = to_draw.c.raster_x;

    float Vy1 = to_draw.a.raster_y;
    float Vy2 = to_draw.b.raster_y;
    float Vy3 = to_draw.c.raster_y;

    for(; y_pos <= max_y; y_pos++){
        int x_pos = max_x;
        for(; x_pos >= min_x; x_pos--){
            float a = ((Vy2 - Vy3)*(x_pos - Vx3) + (Vx3 - Vx2)*(y_pos - Vy3)) / ((Vy2 - Vy3)*(Vx1 - Vx3) + (Vx3 - Vx2)*(Vy1 - Vy3)); // massimo in p0
            float b = ((Vy3 - Vy1)*(x_pos - Vx3) + (Vx1 - Vx3)*(y_pos - Vy3)) / ((Vy2 - Vy3)*(Vx1 - Vx3) + (Vx3 - Vx2)*(Vy1 - Vy3)); // massimo in p1
            float c = 1 - a - b;                                                                                                     // massimo in p2
            
            if(0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1){
                float red = 255;
                float green = 255;
                float blue = 255;

                vector3_t pixel_normal;
                pixel_normal.x = (to_draw.a.normal.x * a) + (to_draw.b.normal.x * b) + (to_draw.c.normal.x * c);
                pixel_normal.y = (to_draw.a.normal.y * a) + (to_draw.b.normal.y * b) + (to_draw.c.normal.y * c);
                pixel_normal = vector3_normalized(pixel_normal);
                vector3_t pixel_light_vector = vector3_new(0.6, 0.6, -1);
                float lambert = clampf(vector3_dot(pixel_normal, pixel_light_vector), 0, 1);

                red *= lambert + 0.3;
                green *= lambert;
                blue *= lambert;

                put_pixel(ctx, x_pos, y_pos, red * lambert, green * lambert, blue * lambert);
            }
        }
    }
}