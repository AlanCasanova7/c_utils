#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct key_value{
    void* key;
    void* value;
    struct key_value* next;
} key_value_t;

typedef struct dictionary{
    size_t size;
    key_value_t** entries;
} dictionary_t;

key_value_t* new_key_value(void* key, void* value);
int register_key_value(dictionary_t* dict, key_value_t* key_val);
void* get_value(dictionary_t* dict, void* key);

dictionary_t* new_dictionary(const size_t size);