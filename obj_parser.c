#include "obj_parser.h"

char* read_file(const char* filename, size_t *file_size){
    FILE *file_reader=fopen(filename,"rb");
    if(!file_reader)
    return NULL;

    fseek(file_reader,0,SEEK_END);
    *file_size=ftell(file_reader);
    fseek(file_reader,0,SEEK_SET);

    char *data=malloc(*file_size);

    if(!data)
    {
        fclose(file_reader);
        return NULL;
    }

    fread (data,1,*file_size,file_reader);
    fclose(file_reader);
    return data;
}