typedef union vector2{
    struct
    {
        float x, y;
    };
    
    struct
    {
        float width, height;
    };
} vector2_t;

static inline vector2_t screen_point_to_pixel_range(vector2_t screen_point, vector2_t screen_size)
{
    vector2_t to_return;
    to_return.x = (screen_point.x / (screen_size.width / 2)) -1;
    to_return.y = (screen_point.y / (screen_size.height / 2)) -1;

    if(to_return.x > 1) to_return.x = 1;
    else if(to_return.x < -1) to_return.x = -1;
    if(to_return.y > 1) to_return.y = 1;
    else if(to_return.y < -1) to_return.y = -1;

    return (to_return);
}

static inline vector2_t pixel_range_to_screen_point(vector2_t pixel_range, vector2_t screen_size)
{
    vector2_t to_return;
    to_return.x = ((pixel_range.x + 1) / 2) * screen_size.width;
    to_return.y = ((pixel_range.y + 1) / 2) * screen_size.height;

    return (to_return);
}