#include <stdio.h>
#include "rasterizer.h"

typedef struct data_buffer{
    vector3_t* pos_array;
    size_t number_of_pos;

    vector2_t* uv_array;
    size_t number_of_uv;

    vector3_t* normals_array;
    size_t number_of_normals;

    Vertex_t* vertex_array;
    size_t number_of_vertex;

    Triangle_t* triangle_array;
    size_t number_of_triangles;

    char* current_token;
    size_t token_size;

    void(*current_update)(char* data, struct data_buffer* data_buffer, int* index);
} data_buffer_t;

char* read_file(const char* filename, size_t *file_size);
obj_model_t* load_obj(char* filename);

void unkown_state(char* data, data_buffer_t* data_buffer, int* index);
void create_vector_state(char* data, data_buffer_t* data_buffer, int* index);
void create_triangle_state(char* data, data_buffer_t* data_buffer, int* index);
void create_normal_state(char *data, data_buffer_t *data_buffer, int *index);
void create_uv_state(char *data, data_buffer_t *data_buffer, int *index);
