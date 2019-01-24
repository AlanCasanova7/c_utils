#include <math.h>

typedef union vector2{
    struct{
        float x, y;
    };

    struct{
        float width, height;
    };
    struct{
        float u, v;
    };
} vector2_t;

typedef union vector3{
    struct{
        float x, y, z;
    };
    struct{
        float width, height, depth;
    };
    struct{
        float r, g, b;
    };
} vector3_t;

static inline vector2_t screen_point_to_pixel_range(vector2_t screen_point, vector2_t screen_size){
    vector2_t to_return;
    to_return.x = (screen_point.x / (screen_size.width / 2)) -1;
    to_return.y = (screen_point.y / (screen_size.height / 2)) -1;

    if(to_return.x > 1) to_return.x = 1;
    else if(to_return.x < -1) to_return.x = -1;
    if(to_return.y > 1) to_return.y = 1;
    else if(to_return.y < -1) to_return.y = -1;

    return (to_return);
}

static inline vector2_t pixel_range_to_screen_point(vector2_t pixel_range, vector2_t screen_size){
    vector2_t to_return;
    to_return.x = ((pixel_range.x + 1) / 2) * screen_size.width;
    to_return.y = ((pixel_range.y + 1) / 2) * screen_size.height;

    return (to_return);
}

static inline vector3_t screen_point_to_pixel_range_v3(vector3_t screen_point, vector2_t screen_size){
    vector3_t to_return;
    to_return.x = (screen_point.x / (screen_size.width / 2)) -1;
    to_return.y = (screen_point.y / (screen_size.height / 2)) -1;
    to_return.z = 0;

    if(to_return.x > 1) to_return.x = 1;
    else if(to_return.x < -1) to_return.x = -1;
    if(to_return.y > 1) to_return.y = 1;
    else if(to_return.y < -1) to_return.y = -1;

    return (to_return);
}

static inline vector3_t pixel_range_to_screen_point_v3(vector3_t pixel_range, vector2_t screen_size){
    vector3_t to_return;
    to_return.x = ((pixel_range.x + 1) / 2) * screen_size.width;
    to_return.y = ((pixel_range.y + 1) / 2) * screen_size.height;
    to_return.z = 0;
    return (to_return);
}

static inline float lerp(float start, float end, float gradient){
    return start + (end - start) * gradient;
}

static inline float inversed_slope(float x0, float y0, float x1, float y1){
    return (x1-x0) / (y1 - y0);
}

static inline vector3_t vector3_new (float x, float y, float z){
    vector3_t toReturn = {.x = x, .y = y, .z = z};
    return toReturn;
}

static inline vector3_t vector3_zero(){
    vector3_t toReturn = {.x = 0, .y = 0, .z = 0};
    return toReturn;
}

static inline float calculate_min(float a, float b){
    return a < b ? a : b;
}

static inline float calculate_max(float a, float b){
    return a > b ? a : b;
}

static inline float linear_convert(float value, float old_min, float old_max, float new_min, float new_max)
{
    return (value - old_min) * (new_max - new_min) / (old_max - old_min) + new_min;
}

static inline float vector3_length(vector3_t a)
{
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

static inline vector3_t vector3_mul(vector3_t a, float b)
{
    vector3_t v;
    v.x = a.x * b;
    v.y = a.y * b;
    v.z = a.z * b;

    return v;
}

static inline vector3_t vector3_normalized(vector3_t a)
{
    float length = 1.0 / vector3_length(a);

    return vector3_mul(a, length);
}

static inline float clampf(float value, float _min, float _max)
{
    if (value < _min)
        return _min;

    if (value > _max)
        return _max;

    return value;
}

static inline float vector3_dot(vector3_t a, vector3_t b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}