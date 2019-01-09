#include "dictionary.h"

dictionary_t* new_dictionary(const size_t size){
    dictionary_t* to_return = malloc(sizeof(dictionary_t));
    if(!to_return)
        return NULL;
    to_return->entries = malloc(sizeof(key_value_t)*size);
    memset(to_return->entries, 0, sizeof(key_value_t)*size);
    to_return->hash_size = size;
    to_return->size = 0;
    return to_return;
}

key_value_t* new_key_value(void* key, void* value, size_t key_length){
    key_value_t* to_return = malloc(sizeof(key_value_t));
    if(!to_return)
        return NULL;

    to_return->key = key;
    to_return->value = value;
    to_return->key_length = key_length;
    to_return->next = NULL;
    return to_return;
}

int register_key_value(dictionary_t* dict, key_value_t* key_val){
    unsigned int hash = DJBHash(key_val->key, key_val->key_length);
    hash %= dict->hash_size;
    key_value_t* current = dict->entries[hash];
    key_value_t* last = current;
    size_t len = key_val->key_length;

    while(current){
        if(len == current->key_length && !memcmp(current->key, key_val->key, len)){
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
    dict->size++;
    return 0;
}

void* get_value(dictionary_t* dict, void* key, size_t key_length){
    unsigned int hash = DJBHash(key, key_length);
    hash %= dict->hash_size;
    key_value_t* current = dict->entries[hash];
    while(current){
        if(current->key_length == key_length && !memcmp(current->key, key, key_length)){
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

key_value_t* get_key_value(dictionary_t* dict, void* key, size_t key_length){
    unsigned int hash = DJBHash(key, key_length);
    hash %= dict->hash_size;
    key_value_t* current = dict->entries[hash];

    while(current){
        if(current->key_length == key_length && !memcmp(current->key, key, key_length)){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void remove_key_value(dictionary_t* dict, void* key, size_t key_length){
    unsigned int hash = DJBHash(key, key_length);
    hash %= dict->hash_size;
    key_value_t* current = dict->entries[hash];
    key_value_t* last = current;
    while(current){
        if(current->key_length == key_length && !memcmp(current->key, key, key_length)){
            if(!memcmp(current, last, sizeof(key_value_t)) && current->next != NULL){
                dict->entries[hash] = current->next;
            }
            else if(!memcmp(current, last, sizeof(key_value_t)) && current->next == NULL){
                dict->entries[hash] = NULL;
            }

            if(current->next){
                last->next = current->next;
            } else{
                last->next = NULL;
            }
            free(current);
            memset(current, 0, sizeof(key_value_t));
            dict->size--;
            return;
        }
        last = current;
        current = current->next;
    }
}