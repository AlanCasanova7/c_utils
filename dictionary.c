#include "dictionary.h"

dictionary_t* new_dictionary(/*const size_t size*/){
    dictionary_t* to_return = malloc(sizeof(dictionary_t));
    if(!to_return)
        return NULL;

    // memset(to_return, 0, sizeof(key_value_t) * size + sizeof(size_t));
    // to_return->size = size;
    to_return->entries = NULL;
    return to_return;
}

key_value_t* new_key_value(void* key, void* value){
    key_value_t* to_return = malloc(sizeof(key_value_t));
    if(!to_return)
        return NULL;

    to_return->key = key;
    to_return->value = value;
    to_return->next = NULL;
    return to_return;
}

int add_key_value(dictionary_t* dict, key_value_t* key_val){
    if(!dict | !key_val)
        return -1;
        
    if(dict->entries == NULL){
        dict->entries = key_val;
        return 0;
    }

    key_value_t* current = dict->entries;
    while(current->next){
        current = current->next;
    }
    current->next = key_val;
    return 0;
}

void* get_value(dictionary_t* dict, void* key){
    if(!dict | !dict->entries)
        return NULL;

    key_value_t* current = dict->entries;
    size_t len = strlen(key);

    while(current){
        if(len == strlen(current->key) && !memcmp(current->key, key, len)){
            return current->value;
        }
        current = current->next;
    }

    return NULL;
}