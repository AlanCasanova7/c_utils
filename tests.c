#include "dictionary.h"
#include "aiv_unit_test.h"

int tests_succeded = 0;
int tests_failed = 0;
int tests_executed = 0;

typedef struct random_struct{
    char* name;
    int age;
} random_struct_t;

TEST(dumb){
    ASSERT_THAT(1);
}

TEST(init_dict){
    dictionary_t* dict = new_dictionary(10);
    ASSERT_THAT(dict != NULL);
    ASSERT_THAT(dict->entries != NULL);
    ASSERT_THAT(dict->size == 10);
    free(dict);
}

TEST(new_entry){
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938, sizeof("gino"));
    ASSERT_THAT((char*)entry->key == "gino");
    ASSERT_THAT((int)entry->value == 1938);
    ASSERT_THAT(entry->next == NULL);
    free(entry);
}

TEST(add_entry){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938, sizeof("gino"));
    register_key_value(dict, entry);
    ASSERT_THAT(dict->entries != NULL);
    //ASSERT_THAT((char*)dict->entries[0]->key == "gino"); //knowing that hash will return 2
    //ASSERT_THAT((int)dict->entries[0]->value == 1938); //knowing that hash will return 2
    free(dict);
    free(entry);
}

TEST(add_multiple_entries){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry1 = new_key_value((void *)"gino", (void *)1938, sizeof("gino"));
    register_key_value(dict, entry1);
    key_value_t* entry2 = new_key_value((void *)"pino", (void *)748, sizeof("pino"));
    register_key_value(dict, entry2);
    ASSERT_THAT(dict->entries != NULL);
    //ASSERT_THAT((char*)dict->entries[0]->key == "gino"); //knowing that hash will return 2
    //ASSERT_THAT((int)dict->entries[9]->value == 748); //knowing that hash will return 5
    free(dict);
    free(entry1);
    free(entry2);
}

TEST(get_value){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938, sizeof("gino"));
    register_key_value(dict, entry);
    void* value = get_value(dict, (void*)"gino", sizeof("gino"));
    ASSERT_THAT((int)value == 1938);
    free(dict);
    free(entry);
}

TEST(get_multiple_value){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry1 = new_key_value((void *)"gino", (void *)1938, sizeof("gino"));
    register_key_value(dict, entry1);
    void* gino_value = get_value(dict, (void*)"gino", sizeof("gino"));
    key_value_t* entry2 = new_key_value((void *)"motumbo", (void *)4664, sizeof("motumbo"));
    register_key_value(dict, entry2);
    void* motumbo_value = get_value(dict, (void*)"motumbo", sizeof("motumbo"));
    ASSERT_THAT((int)gino_value == 1938);
    ASSERT_THAT((int)motumbo_value == 4664);
    free(dict);
    free(entry1);
    free(entry2);
}

TEST(get_key_value){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938, sizeof("gino"));
    register_key_value(dict, entry);
    key_value_t* key_value = get_key_value(dict, (void*)"gino", sizeof("gino"));
    ASSERT_THAT((char *)key_value->key == "gino");
    ASSERT_THAT((int)key_value->value == 1938);
    ASSERT_THAT(key_value->next == NULL);
    free(dict);
    free(entry);
}

TEST(replace_value){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938, sizeof("gino"));
    register_key_value(dict, entry);
    key_value_t* entry2 = new_key_value((void *)"gino", (void *)1111, sizeof("gino"));
    register_key_value(dict, entry2);
    void* value = get_value(dict, (void*)"gino", sizeof("gino"));
    ASSERT_THAT((int)value == 1111);
    free(dict);
    free(entry);
    free(entry2);
}

TEST(remove_key_value){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938, sizeof("gino"));
    register_key_value(dict, entry);
    remove_key_value(dict, (void *)"gino", sizeof("gino"));
    void* value = get_value(dict, (void*)"gino", sizeof("gino"));
    ASSERT_THAT((int)value == 0);
    free(dict);
}

TEST(get_value_on_collision){
    dictionary_t* dict = new_dictionary(1);
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938, sizeof("gino"));
    register_key_value(dict, entry);
    key_value_t* entry2 = new_key_value((void *)"pino", (void *)1111, sizeof("pino"));
    register_key_value(dict, entry2);
    void* value = get_value(dict, (void*)"gino", sizeof("gino"));
    ASSERT_THAT((int)value == 1938);
    void* value2 = get_value(dict, (void*)"pino", sizeof("pino"));
    ASSERT_THAT((int)value2 == 1111);
}

TEST(get_value_with_wrong_key){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938, sizeof("gino"));
    register_key_value(dict, entry);
    void* value = get_value(dict, (void*)"pino", sizeof("pino"));
    ASSERT_THAT(value == NULL);
    key_value_t* key_val = get_key_value(dict, (void*)"pino", sizeof("pino"));
    ASSERT_THAT(key_val == NULL);
}

TEST(get_value_with_int_key){
    dictionary_t* dict = new_dictionary(10);
    int key = 19380;
    key_value_t* entry = new_key_value((void *)&key, (void *)"gino", sizeof(key));
    register_key_value(dict, entry);
    void* value = get_value(dict, (void*)&key, sizeof(key));
    ASSERT_THAT((char*)value == "gino");
    free(dict);
    free(entry);
}

TEST(get_value_with_struct_key){
    dictionary_t* dict = new_dictionary(10);
    random_struct_t key;
    key.age = 100;
    key.name = "gatto";
    key_value_t* entry = new_key_value((void *)&key, (void *)"gino", sizeof(key));
    register_key_value(dict, entry);
    void* value = get_value(dict, (void*)&key, sizeof(key));
    ASSERT_THAT((char*)value == "gino");
    free(dict);
    free(entry);
}

int main(int argc, char **argv)
{
    RUN_TEST(dumb);
    RUN_TEST(init_dict);
    RUN_TEST(new_entry);
    RUN_TEST(add_entry);
    RUN_TEST(add_multiple_entries);
    RUN_TEST(get_value);
    RUN_TEST(get_multiple_value);
    RUN_TEST(get_key_value);
    RUN_TEST(replace_value);
    RUN_TEST(remove_key_value);
    RUN_TEST(get_value_on_collision);
    RUN_TEST(get_value_with_wrong_key);
    RUN_TEST(get_value_with_int_key);
    RUN_TEST(get_value_with_struct_key);
    PRINT_TEST_RESULTS();
    return 0;
}
