#include "dictionary.h"
#include "aiv_unit_test.h"

int tests_succeded = 0;
int tests_failed = 0;
int tests_executed = 0;

TEST(dumb){
    ASSERT_THAT(1);
}

TEST(init_dict){
    dictionary_t* dict = new_dictionary(10);
    ASSERT_THAT(dict != NULL);
    ASSERT_THAT(dict->entries != NULL);
    ASSERT_THAT(dict->size == 10);
}

TEST(new_entry){
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938);
    ASSERT_THAT((char*)entry->key == "gino");
    ASSERT_THAT((int)entry->value == 1938);
    ASSERT_THAT(entry->next == NULL);
}

TEST(add_entry){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938);
    register_key_value(dict, entry);
    ASSERT_THAT(dict->entries != NULL);
    ASSERT_THAT((char*)dict->entries[2]->key == "gino"); //knowing that hash will return 2
    ASSERT_THAT((int)dict->entries[2]->value == 1938); //knowing that hash will return 2
}

TEST(add_multiple_entries){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry1 = new_key_value((void *)"gino", (void *)1938);
    register_key_value(dict, entry1);
    key_value_t* entry2 = new_key_value((void *)"pino", (void *)748);
    register_key_value(dict, entry2);
    ASSERT_THAT(dict->entries != NULL);
    ASSERT_THAT((char*)dict->entries[2]->key == "gino"); //knowing that hash will return 2
    ASSERT_THAT((int)dict->entries[5]->value == 748); //knowing that hash will return 5
}

TEST(get_value){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938);
    register_key_value(dict, entry);
    void* value = get_value(dict, (void*)"gino");
    ASSERT_THAT((int)value == 1938);
}

TEST(get_multiple_value){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry1 = new_key_value((void *)"gino", (void *)1938);
    register_key_value(dict, entry1);
    void* gino_value = get_value(dict, (void*)"gino");
    key_value_t* entry2 = new_key_value((void *)"motumbo", (void *)4664);
    register_key_value(dict, entry2);
    void* motumbo_value = get_value(dict, (void*)"motumbo");
    ASSERT_THAT((int)gino_value == 1938);
    ASSERT_THAT((int)motumbo_value == 4664);
}

TEST(get_key_value){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938);
    register_key_value(dict, entry);
    key_value_t* key_value = get_key_value(dict, (void*)"gino");
    ASSERT_THAT((char *)key_value->key == "gino");
    ASSERT_THAT((int)key_value->value == 1938);
    ASSERT_THAT(key_value->next == NULL);
}

TEST(replace_value){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938);
    register_key_value(dict, entry);
    key_value_t* entry2 = new_key_value((void *)"gino", (void *)1111);
    register_key_value(dict, entry2);
    void* value = get_value(dict, (void*)"gino");
    ASSERT_THAT((int)value == 1111);
}

TEST(remove_key_value){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938);
    register_key_value(dict, entry);
    remove_key_value(dict, (void *)"gino");
    void* value = get_value(dict, (void*)"gino");
    ASSERT_THAT(value == NULL);
}

TEST(get_value_on_collision){
    dictionary_t* dict = new_dictionary(1);
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938);
    register_key_value(dict, entry);
    key_value_t* entry2 = new_key_value((void *)"pino", (void *)1111);
    register_key_value(dict, entry2);
    void* value = get_value(dict, (void*)"gino");
    ASSERT_THAT((int)value == 1938);
    void* value2 = get_value(dict, (void*)"pino");
    ASSERT_THAT((int)value2 == 1111);
}

TEST(get_value_with_wrong_key){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938);
    register_key_value(dict, entry);
    void* value = get_value(dict, (void*)"pino");
    ASSERT_THAT(value == NULL);
    key_value_t* key_val = get_key_value(dict, (void*)"pino");
    ASSERT_THAT(key_val == NULL);
}

// TEST(get_value_with_int_key){
//     dictionary_t* dict = new_dictionary(10);
//     key_value_t* entry = new_key_value((void *)1938, (void *)"gino");
//     printf("sizeof: %llu\n", sizeof(entry->value));
//     register_key_value(dict, entry);
//     void* value = get_value(dict, (void*)1938);
//     ASSERT_THAT((char*)value == "gino");
// }

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
    //RUN_TEST(get_value_with_int_key);
    PRINT_TEST_RESULTS();
    return 0;
}
