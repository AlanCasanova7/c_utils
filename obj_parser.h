#include "rasterizer.h"
#include <stdio.h>

typedef struct obj_model{
    Triangle_t* array_of_vector;
    size_t array_of_vector_size;
} obj_model_t;

void append_vector(Triangle_t** array_of_vector, size_t* array_of_vector_size, Triangle_t value);

void parse_obj(char* data,size_t* file_size);
void parse_obj_faces(char* data,size_t* file_size);
char* read_file(const char* filename, size_t *file_size);
char* save_faces(char* data, int *index,size_t* max_size);
char* save_vertex(char* data, int *index);
//{
    // *array_of_vector_size += 1;
    // Triangle_t* resized_area = (Triangle_t*)realloc(*array_of_vector, sizeof(Triangle_t) * *array_of_vector_size);
    // if(!resized_area){
    //     printf("PANIC");
    //     return;
    // }
    // resized_area[*array_of_vector_size-1] = value;
    // *array_of_vector = resized_area;
//}