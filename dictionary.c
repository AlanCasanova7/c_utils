#include "dictionary.h"

static unsigned int DJBHash(char* key, unsigned int len){
   unsigned int hash = 5381;
   unsigned int i    = 0;

   for(i = 0; i < len; key++, i++)
   {   
      hash = ((hash << 5) + hash) + (*key);
   }   

   return hash;
}

dictionary_t* new_dictionary(const size_t size){
    dictionary_t* to_return = malloc(sizeof(dictionary_t));
    if(!to_return)
        return NULL;
    to_return->entries = malloc(sizeof(key_value_t)*size);
    memset(to_return->entries, 0, sizeof(key_value_t)*size);
    to_return->size = size;
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

int register_key_value(dictionary_t* dict, key_value_t* key_val){
    unsigned int hash = DJBHash(key_val->key, strlen(key_val->key));
    hash %= dict->size;
    key_value_t* current = dict->entries[hash];
    key_value_t* last = current;
    size_t len = strlen(key_val->key);

    while(current){
        if(len == strlen(current->key) && !memcmp(current->key, key_val->key, len)){
            current->value = key_val->value;
            return 0;
        }
        last = current;
        current = current->next;
    }

    current = malloc(sizeof(key_value_t));
    current = key_val;
    if(last != NULL){
        last->next = current;
    } else{
        dict->entries[hash] = current;
    }

    return 0;
}

void* get_value(dictionary_t* dict, void* key){
    unsigned int hash = DJBHash(key, strlen(key));
    hash %= dict->size;
    key_value_t* current = dict->entries[hash];
    size_t len = strlen(key);
    while(current){
        if(strlen(current->key) == len && !memcmp(current->key, key, len)){
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}