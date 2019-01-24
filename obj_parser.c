#include "obj_parser.h"

#define RESET_TOKEN                   \
    free(data_buffer->current_token); \
    data_buffer->token_size = 0;      \
    data_buffer->current_token = NULL;

#define CLOSE_TOKEN                                                                              \
    data_buffer->token_size += 1;                                                                \
    resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size); \
    if (!resized_token)                                                                          \
    {                                                                                            \
        printf("COULDN'T RESIZE TOKEN. PANIC \n");                                               \
        return;                                                                                  \
    }                                                                                            \
    data_buffer->current_token = resized_token;                                                  \
    data_buffer->current_token[data_buffer->token_size - 1] = '\0';

char *read_file(const char *filename, size_t *file_size)
{
    FILE *file_reader = fopen(filename, "rb");
    if (!file_reader)
        return NULL;
    fseek(file_reader, 0, SEEK_END);
    *file_size = ftell(file_reader);
    fseek(file_reader, 0, SEEK_SET);

    char *data = malloc(*file_size);

    if (!data)
    {
        fclose(file_reader);
        return NULL;
    }

    fread(data, 1, *file_size, file_reader);
    fclose(file_reader);
    return data;
}

obj_model_t *load_obj(char *filename)
{
    size_t size;
    char *data = read_file(filename, &size);
    int index = 0;

    data_buffer_t *data_buffer = malloc(sizeof(data_buffer_t));
    memset(data_buffer, 0, sizeof(data_buffer_t));
    data_buffer->current_update = &unkown_state;

    obj_model_t *obj_model = malloc(sizeof(obj_model_t));
    memset(obj_model, 0, sizeof(obj_model_t));

    while (index <= size)
    {
        (*data_buffer->current_update)(data, data_buffer, &index);
    }


    obj_model->number_of_triangles = data_buffer->number_of_triangles;
    Triangle_t *resized_triangles = realloc(obj_model->triangle_array, sizeof(Triangle_t) * obj_model->number_of_triangles);
    if (!resized_triangles)
    {
        printf("COULDN'T RESIZE resized_triangles. PANIC \n");
        return NULL;
    }
    obj_model->triangle_array = resized_triangles;
    for(int i = 0; i < data_buffer->number_of_triangles; i++){
        obj_model->triangle_array[i] = data_buffer->triangle_array[i];
    }
    printf("Created data, number of triangles: %llu\n", obj_model->number_of_triangles * 9);
    return obj_model;
}

void unkown_state(char *data, data_buffer_t *data_buffer, int *index)
{
    if (data[*index] == 'v' && data[*index + 1] == ' ')
    {
        data_buffer->current_update = &create_vector_state;
        *index += 2;
        return;
    }
    else if (data[*index] == 'v' && data[*index + 1] == 't')
    {
        data_buffer->current_update = &create_uv_state;
        *index += 3;
        return;
    }
    else if (data[*index] == 'v' && data[*index + 1] == 'n')
    {
        data_buffer->current_update = &create_normal_state;
        *index += 3;
        return;
    }
    else if (data[*index] == 'f' && data[*index + 1] == ' ')
    {
        data_buffer->current_update = &create_triangle_state;
        *index += 2;
        return;
    }
    else
    {
        *index += 1;
        return;
    }
}

void create_vector_state(char *data, data_buffer_t *data_buffer, int *index)
{
    vector3_t to_add;
    // vector3_t color1 = {.x = 255, .y = 255, .z = 255};
    // vector3_t color2 = {.x = 255, .y = 255, .z = 255};
    // vector3_t color3 = {.x = 0, .y = 0, .z = 125};
    // vector3_t colors[3] = {color1, color2, color3};

    char *resized_token = NULL;

    while (data[*index] != ' ')
    {
        //printf("%c", data[*index]);
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    to_add.x = (float)atof(data_buffer->current_token);
    RESET_TOKEN;

    *index += 1;
    while (data[*index] != ' ')
    {
        //printf("%c", data[*index]);
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    to_add.y = (float)atof(data_buffer->current_token);
    RESET_TOKEN;

    *index += 1;
    while (data[*index] != '\n')
    {
        //printf("%c", data[*index]);
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    to_add.z = -1 * (float)atof(data_buffer->current_token);
    RESET_TOKEN;

    //Vertex_t v = Vertex_new(to_add, colors[*index%3]);
    data_buffer->number_of_pos += 1;
    vector3_t *resized_pos = realloc(data_buffer->pos_array, sizeof(vector3_t) * data_buffer->number_of_pos);
    if (!resized_pos)
    {
        printf("COULDN'T RESIZE VERTEX. PANIC \n");
        return;
    }

    data_buffer->pos_array = resized_pos;
    data_buffer->pos_array[data_buffer->number_of_pos - 1] = to_add;
    // printf("Vertex number: %llu added as: x %f, y %f, z %f\n", data_buffer->number_of_vertex + 4, data_buffer->vertex_array[data_buffer->number_of_vertex - 1].position.x, data_buffer->vertex_array[data_buffer->number_of_vertex - 1].position.y, data_buffer->vertex_array[data_buffer->number_of_vertex - 1].position.z);
    *index += 1;
    data_buffer->current_update = &unkown_state;
    return;
}

void create_uv_state(char *data, data_buffer_t *data_buffer, int *index)
{
    vector2_t to_add;
    char *resized_token = NULL;

    while (data[*index] != ' ')
    {
        //printf("%c", data[*index]);
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    to_add.x = (float)atof(data_buffer->current_token);
    RESET_TOKEN;

    *index += 1;

    while (data[*index] != '\n')
    {
        //printf("%c", data[*index]);
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    to_add.y = -1 * (float)atof(data_buffer->current_token);
    RESET_TOKEN;

    data_buffer->number_of_uv += 1;
    vector2_t *resized_uv = realloc(data_buffer->uv_array, sizeof(vector2_t) * data_buffer->number_of_uv);
    if (!resized_uv)
    {
        printf("COULDN'T RESIZE VERTEX. PANIC \n");
        return;
    }

    data_buffer->uv_array = resized_uv;
    data_buffer->uv_array[data_buffer->number_of_uv - 1] = to_add;
    // printf("Vertex number: %llu added as: x %f, y %f, z %f\n", data_buffer->number_of_vertex + 4, data_buffer->vertex_array[data_buffer->number_of_vertex - 1].position.x, data_buffer->vertex_array[data_buffer->number_of_vertex - 1].position.y, data_buffer->vertex_array[data_buffer->number_of_vertex - 1].position.z);
    *index += 1;
    data_buffer->current_update = &unkown_state;
    return;
}

void create_normal_state(char *data, data_buffer_t *data_buffer, int *index)
{
    vector3_t to_add;

    char *resized_token = NULL;

    while (data[*index] != ' ')
    {
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    to_add.x = (float)atof(data_buffer->current_token);
    RESET_TOKEN;

    *index += 1;
    while (data[*index] != ' ')
    {
        //printf("%c", data[*index]);
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    to_add.y = (float)atof(data_buffer->current_token);
    RESET_TOKEN;

    *index += 1;
    while (data[*index] != '\n')
    {
        //printf("%c", data[*index]);
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    to_add.z = -1 * (float)atof(data_buffer->current_token);
    RESET_TOKEN;

    //Vertex_t v = Vertex_new(to_add, colors[*index%3]);
    data_buffer->number_of_normals += 1;
    vector3_t *resized_pos = realloc(data_buffer->normals_array, sizeof(vector3_t) * data_buffer->number_of_normals);
    if (!resized_pos)
    {
        printf("COULDN'T RESIZE VERTEX. PANIC \n");
        return;
    }

    data_buffer->normals_array = resized_pos;
    data_buffer->normals_array[data_buffer->number_of_normals - 1] = to_add;
    // printf("Vertex number: %llu added as: x %f, y %f, z %f\n", data_buffer->number_of_vertex + 4, data_buffer->vertex_array[data_buffer->number_of_vertex - 1].position.x, data_buffer->vertex_array[data_buffer->number_of_vertex - 1].position.y, data_buffer->vertex_array[data_buffer->number_of_vertex - 1].position.z);
    *index += 1;
    data_buffer->current_update = &unkown_state;
    return;
}

void create_triangle_state(char *data, data_buffer_t *data_buffer, int *index)
{
    Triangle_t to_add;
    Vertex_t v0, v1, v2;
    char *resized_token = NULL;
    while (data[*index] != '/')
    {
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    v0.position = data_buffer->pos_array[(int)atof(data_buffer->current_token) -1];
    *index += 1;
    RESET_TOKEN;

    while (data[*index] != '/')
    {
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    v0.uv = data_buffer->uv_array[(int)atof(data_buffer->current_token) -1];
    *index += 1;
    RESET_TOKEN;

    while (data[*index] != ' ')
    {
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        //to_add.a.color.r = data_buffer->vertex_array[(int)atof(data_buffer->current_token) -1].position.x * 255.0;
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    v0.normal = data_buffer->normals_array[(int)atof(data_buffer->current_token) -1];
    *index += 1;
    RESET_TOKEN;
    while (data[*index] != '/')
    {
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    v1.position = data_buffer->pos_array[(int)atof(data_buffer->current_token) -1];
    *index += 1;
    RESET_TOKEN;

    while (data[*index] != '/')
    {
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    v1.uv = data_buffer->uv_array[(int)atof(data_buffer->current_token) -1];
    *index += 1;
    RESET_TOKEN;

    while (data[*index] != ' ')
    {
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    v1.normal = data_buffer->normals_array[(int)atof(data_buffer->current_token) -1];
    *index += 1;
    RESET_TOKEN;

    while (data[*index] != '/')
    {
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    v2.position = data_buffer->pos_array[(int)atof(data_buffer->current_token) -1];
    *index += 1;
    RESET_TOKEN;

    while (data[*index] != '/')
    {
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    v2.uv = data_buffer->uv_array[(int)atof(data_buffer->current_token) -1];
    *index += 1;
    RESET_TOKEN;

    while (data[*index] != '\n')
    {
        data_buffer->token_size += 1;
        resized_token = realloc(data_buffer->current_token, sizeof(char) * data_buffer->token_size);
        if (!resized_token)
        {
            printf("COULDN'T RESIZE TOKEN. PANIC \n");
            return;
        }
        data_buffer->current_token = resized_token;
        data_buffer->current_token[data_buffer->token_size - 1] = data[*index];
        *index += 1;
    }
    CLOSE_TOKEN;
    v2.normal = data_buffer->normals_array[(int)atof(data_buffer->current_token) -1];
    *index += 1;
    RESET_TOKEN;

    to_add.a = v0;
    to_add.b = v1;
    to_add.c = v2;
    data_buffer->number_of_triangles += 1;
    Triangle_t* resized_triangles = realloc(data_buffer->triangle_array, sizeof(Triangle_t) * data_buffer->number_of_triangles);
    if (!resized_triangles)
    {
        printf("COULDN'T RESIZE TRIANGLES. PANIC \n");
        return;
    }

    data_buffer->triangle_array = resized_triangles;
    data_buffer->triangle_array[data_buffer->number_of_triangles - 1] = to_add;

    data_buffer->current_update = &unkown_state;
    return;
}